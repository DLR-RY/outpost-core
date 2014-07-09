--[[
Copyright (c) 2013, German Aerospace Center (DLR)
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
--]]

local crc16 = require('crc16')
local bitstream = require('bitstream')

local current_folder = (...):gsub('%.[^%.]+$', '')
local generator = require(current_folder .. '.generator')



local transfer_frame_header_definition = {
	name = "TC Transfer Frame Header",
	description = "TC Transfer Frame Header",
	structure = {
		{ name="version_number", type="fixed", length=2, value=0 },
		-- Bypass_Flag = 0 -> Type-A
		-- Bypass_Flag = 1 -> Type-B
		{ name="bypass_flag", type="editable", length=1, value=0 },
		-- Control_Command_Flag = 0 -> Type-D (Data Unit)
		-- Control_Command_Flag = 1 -> Type-C (Command Information)
		{ name="control_command_flag", type="editable", length=1, value=0 },
		{ name="reserved", type="fixed", length=2, value=0 },
		{ name="spacecraft_id", type="editable", length=10 },
		{ name="virtual_channel_id", type="editable", length=6 },
		-- Frame_Length = Total Length of the Frame (including header and CRC) - 1
		{ name="frame_length", type="editable", length=10 },
		{ name="frame_sequence_number", type="editable", length=8 },
	}
}

local transfer_frame_error_control_definition = {
	name = "TC Transfer Frame Error Control Field",
	description = "TC Transfer Frame Error Control Field",
	structure = {
		{ name="frame_error_control_field", type="editable", length=16 },
	}
}

-- Type-BC (no Segment or source packet)
-- * Unlock
--    0x00
-- * Set V(R)
--    0x82 0x00 0x.. (Receiver_Frame_Sequence_Number, value for next frame sequence number for that channel)

-- Transfer Frame Header (5 byte)
-- Transfer Frame Data Field (0..1017)
-- Frame Error Control Field (16 bit)
-- - Calculated over Frame Header and Frame Data Field

local segment_header_definition = {
	name = "TC Segment Header",
	description = "TC Segment Header",
	structure = {
		-- Sequence_Flags = 0b00 -> No segmentation (one complete service data unit or multiple Packets)
		{ name="sequence_flags", type="fixed", length=2, value=3 },
		{ name="map_identifier", type="editable", length=6 },
	}
}

local source_packet_header_definition = {
	name = "TC Source Packet Header",
	description = "TC Source Packet Header",
	structure = {
		{ name="version_number", type="fixed", length=3, value=0 },
		{ name="type", type="fixed", length=1, value=1 },
		{ name="data_field_header_flag", type="editable", length=1, value=1 },
		{ name="application_process_id", type="editable", length=11 },
		{ name="sequence_flags", type="fixed", length=2, value=3 },
		{ name="sequence_count", type="editable", length=14 },
		{ name="packet_length", type="editable", length=16 },
	}
}

local source_packet_error_control_definition = {
	name = "TC Source Packet Error Control Field",
	description = "TC Source Packet Error Control Field",
	structure = {
		{ name="packet_error_control_field", type="editable", length=16 },
	}
}

local command_unlock_definition = {
	name = "Unlock command",
	description = "",
	structure = {
		{ name="command", type="fixed", length=8, value=0x00 },
	}
}

local command_set_v_definition = {
	name = "Set V(R) command",
	description = "",
	structure = {
		{ name="command", type="fixed", length=16, value=0x8200 },
		{ name="r", type="editable", length=8 },
	}
}

-- ----------------------------------------------------------------------------
local function transfer_frame(values)
	local data_field = values.data_field
	
	local frame = bitstream.new(0)
	local frame_length = 0
	
	-- length needed to calculate the packet length
	local data_field_header_length = 0
	if values.data_field_header ~= nil then
		frame = values.data_field_header .. frame
		data_field_header_length = #values.data_field_header
		
		frame_length = frame_length + data_field_header_length
	end

	if values.packet_header ~= nil then
		local generator = generator.new(source_packet_header_definition)
		generator:set {
			data_field_header_flag = values.data_field_header and 1 or 0,
			application_process_id = values.packet_header.apid,
			sequence_count = values.packet_header.sequence_count,
			packet_length = data_field_header_length + #values.application_data - 1,
		}
		local source_packet_header = generator:render()
		
		frame_length = frame_length + source_packet_header:length()
		frame = source_packet_header:append(frame)
	end

	if values.segment_header ~= nil then
		local generator = generator.new(segment_header_definition)
		generator:set {
			-- use the default value of 0 if no MAP Identifier is given
			map_identifier = values.segment_header.map_id or 0
		}
		local segment_header = generator:render()
		
		frame_length = frame_length + segment_header:length()
		frame = segment_header:append(frame)
	end

	-- append the application data field
	frame:append(values.application_data)
	frame_length = frame_length + #values.application_data

	if (values.packet_header ~= nil and values.packet_error_control == nil) or
	    values.packet_error_control then
		local generator = generator.new(source_packet_error_control_definition)
		generator:set {
			packet_error_control_field = crc16.new():calculate(frame:bytes()):get(),
		}
		packet_error_control = generator:render()
		
		frame_length = frame_length + packet_error_control:length()
		frame:append(packet_error_control)
	end
	
	-- 5 byte transfer frame header
	-- 2 byte frame error control field
	frame_length = frame_length + 5 + 2
	
	local header_generator = generator.new(transfer_frame_header_definition)
	header_generator:set {
		bypass_flag = values.frame_header.type.bypass,
		control_command_flag = values.frame_header.type.cc,
		spacecraft_id = values.frame_header.scid,
		virtual_channel_id = values.frame_header.vcid,
		-- Frame length is the length of the total frame in bytes - 1
		-- see CCSDS 232.0-B-2
		frame_length = frame_length - 1,
		frame_sequence_number = values.frame_header.sequence_count,
	}
	
	-- prepend header to the partially constructed frame
	frame = header_generator:render():append(frame)

	-- calculate the CRC over the packet so far and append it to the end
	local error_control_generator = generator.new(transfer_frame_error_control_definition)
	error_control_generator:set {
		frame_error_control_field = crc16.new():calculate(frame:bytes()):get(),
	}
	frame:append(error_control_generator:render())

	return frame
end

-- ----------------------------------------------------------------------------
--- CCSDS FARM-1 commands.
--
-- Must only be send in an transfer frame from type BC and without any
-- packet oder data field header.
local command = {
	unlock = function ()
		return generator.new(command_unlock_definition):render()
	end,
	
	set_v = function (r)
		return generator.new(command_set_v_definition):set({ r=r }):render()
	end
}

-- ----------------------------------------------------------------------------
--- Sequence generator factory.
--
-- The sequence generators are intended to generate the sequence numbers in
-- the CCSDS/PUS packet generation. Therein a sequence generator per APID is
-- needed.
--
-- The information about the last sequence number is stored in the function
-- closure.
--
-- @param bits
--     Number of bits before the generated numbers overflow to zero
-- @param initial
--     Initial value of the sequence. Can either be number when using only one
--     sequence or a table with an entry per sequence. See the 'spec' folder
--     for examples.
--
local function sequence_generator(bits, initial)
	local maxvalue = (2 ^ bits) - 1
	
	local sequence = nil
	if type(initial) == "number" then
		sequence = {
			[-1] = initial
		}
	else
		sequence = initial or {}
	end
	
	local function set(id, value)
		if value == nil then
			value = id
			id = -1
		end
		sequence[id] = value
	end
	
	local function peek(id)
		id = id or -1
		local current = sequence[id] or 0
		return current
	end
	
	local function get(id)
		id = id or -1
		local current = sequence[id] or 0
		local nextvalue = current + 1
		if nextvalue > maxvalue then
			nextvalue = 0
		end
		sequence[id] = nextvalue
		return current
	end

	return {
		set = set,
		peek = peek,
		get = get,
	}
end

-- ----------------------------------------------------------------------------
return {
	_VERSION = '0.1',
	_DESCRIPTION = [[ TODO ]],
	_LICENSE = [[2-clause BSD License]],
	
	-- functions
	transfer_frame = transfer_frame,
	command = command,
	
	sequence_generator = sequence_generator,
	
	-- constants
	TYPE_AD = { bypass = 0, cc = 0 },
	TYPE_BD = { bypass = 1, cc = 0 },
	TYPE_BC = { bypass = 1, cc = 1 },
}

