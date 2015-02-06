--[[
Copyright (c) 2014, German Aerospace Center (DLR)
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
local inspect = require('inspect')

local current_folder = (...):gsub('%.[^%.]+$', '')
local generator = require('packetgenerator.generator')

local transfer_frame_header_definition = {
	name = "TM Transfer Frame Header",
	description = "TM Transfer Frame Header",
	structure = {
		{ name="version_number", type="fixed", length=2, value=0 },
		{ name="spacecraft_id", type="editable", length=10 },
		{ name="virtual_channel_id", type="editable", length=3 },
		
		-- Signals whether a CLCW is added ('1') or not ('0').
		{ name="operational_control_field_flag", type="editable", length=1, value=1 },
		{ name="master_channel_frame_count", type="editable", length=8 },
		{ name="virtual_channel_frame_count", type="editable", length=8 },
		
		-- Indicate presence ('1') or absence ('0') of Secondary Frame Header.
		{ name="secondary_header_flag", type="fixed", length=1, value=0 },
		
		-- Source Packet insertion mode. '0': synchronously inserted packets.
		{ name="sync_flag", type="fixed", length=1, value=0 },
		
		-- Provides the order (forward, '0' or reverse '1') in which source packets
		-- are inserted into the Transfer Frame Data Field.
		{ name="packet_order_flag", type="fixed", length=1, value=0 },
		
		-- Identifies maximum data field length. '11': no segmentation.
		{ name="segment_length_id", type="fixed", length=2, value=3 },
		
		-- 0x7FE: Only idle data
		-- 0x7FF: No source packet starts in this frame
		{ name="first_header_pointer", type="editable", length=11 },
	}
}

-- Possible values for the first_header field:
local IDLE = 0x7FE
local NO_HEADER = 0x7FF

local idle_data_pattern = 0x55

local transfer_frame_clcw_definition = {
	name = "TM Transfer Frame CLCW",
	description = "TM Transfer Frame Communication Link Control Word",
	structure = {
		{ name="type", type="fixed", length=1, value=0 },
		{ name="version_number", type="fixed", length=2, value=0 },
		-- Application of the status field is mission specified,
		-- not part of the COP
		{ name="status", type="editable", length=3, value=0 },
		{ name="cop_in_effect", type="fixed", length=2, value=1 },
		{ name="vcid", type="editable", length=6 },
		{ name="spare", type="editable", length=2, value=0 },
		{ name="no_rf_avail", type="editable", length=1 },
		{ name="no_bit_lock", type="editable", length=1 },
		-- Indicates the FARM lockout status, 1: lockout, 0: not in lockout
		{ name="lockout", type="editable", length=1 },
		{ name="wait", type="editable", length=1 },
		{ name="retransmit", type="editable", length=1 },
		{ name="farm_b_counter", type="editable", length=2 },
		{ name="spare2", type="fixed", length=1, value=0 },
		-- Contains the next expected frame sequence number (N(R))
		{ name="report_value", type="editable", length=8 },
	}
}

local transfer_frame_error_control_definition = {
	name = "Transfer Frame Error Control Field",
	description = "Transfer Frame Error Control Field",
	structure = {
		{ name="frame_error_control_field", type="editable", length=16 },
	}
}

local source_packet_header_definition = {
	name = "TM Source Packet Header",
	description = "TC Source Packet Header",
	structure = {
		{ name="version_number", type="fixed", length=3, value=0 },
		{ name="type", type="fixed", length=1, value=0 },
		{ name="data_field_header_flag", type="editable", length=1, value=1 },
		{ name="application_process_id", type="editable", length=11 },
		{ name="sequence_flags", type="editable", length=2, value=3 },
		{ name="sequence_count", type="editable", length=14 },
		{ name="packet_data_length", type="editable", length=16 },
	}
}

local function idle_source_packet(length)
	if length < 7 then
		error(string.format([[Packet to short! An idle packet has always to be
 7 or more octets in length (currently: %i)]], length))
 	end
 	
 	-- Length of the packet data field in bytes
 	local data_length = length - 6
 	
 	local packet_header_generator = generator.new(source_packet_header_definition)
 	packet_header_generator:set {
 		application_process_id = 0x7FF,
 		data_field_header_flag = 0,
 		sequence_count = 0,
 		packet_data_length = data_length - 1
 	}
 	local packet = packet_header_generator:render()
	
	local packet_data = bitstream.new(data_length * 8)
	for i = 1,data_length do
		local offset = (i - 1) * 8 + 1
		packet_data:insert(offset, 8, idle_data_pattern)
	end
	packet:append(packet_data)
	
	return packet
end

-- ----------------------------------------------------------------------------
-- Create a TM transfer frame.
-- 
-- @param values
--     Table containing all the information needed to build a transfer frame:
-- 
--     transfer_frame {
--         length = [int] (in byte),
--         frame_header = {
--             scid = [int],
--             vcid = [int],
--             mcfc = [int],
--             vcfc = [int],
--             first_header_pointer = [int],
--         },
--         data = [bitstream],
--         clcw = [bitstream],
--         frame_error_control = [bool] (default: true),
--     }
--    
local function transfer_frame(values)
	local frame = bitstream.new(0)
	local frame_length = 0
	
	-- append the application data field
	frame:append(values.data)
	frame_length = frame_length + #values.data
	
	-- Add CLCW length if contained in the definition
	if values.clcw then
		frame_length = frame_length + #values.clcw
	end

	local frame_header_generator = generator.new(transfer_frame_header_definition)
	local error_control_generator = generator.new(transfer_frame_error_control_definition)
	
	values.frame_error_control = values.frame_error_control or true
	if values.frame_error_control then
		frame_length = frame_length + error_control_generator:get_byte_length()
	end
	
	-- Add length of the frame header
	frame_length = frame_length +
	               frame_header_generator:get_byte_length()
	
	values.length = values.length or 0
	if values.length ~= 0 then
		if frame_length > values.length then
			error(string.format([[The provided data (%i byte) for the data field
does not fit in the given length for the frame (%i byte).]], #values.data, values.length), 2)
		end
		
		-- fill the remaining space inside the frame with data
		remaining = values.length - frame_length
		if remaining ~= 0 then
			if remaining < 7 then
				error(string.format([[An idle packet with fewer than 7 byte length
	(%i byte) neededs to be added. This is currently not supported.]], remaining), 2)
			end

			-- Add idle packet to fill the remaining space
			local packet = idle_source_packet(remaining)
			frame:append(packet)
			frame_length = frame_length + #packet
		end
	end
	
	-- Append CLCW data
	if values.clcw then
		frame:append(values.clcw)
	end
	
	frame_header_generator:set {
		-- The operational control field indicates whether a CLCW is included (1)
		-- into the TM frame or not (0).
		operational_control_field_flag = values.clcw and 1 or 0,
		spacecraft_id = values.frame_header.scid,
		virtual_channel_id = values.frame_header.vcid,
		master_channel_frame_count = values.frame_header.mcfc,
		virtual_channel_frame_count = values.frame_header.vcfc,
		-- Frame length is the length of the total frame in bytes - 1
		frame_length = frame_length - 1,
		frame_sequence_number = values.frame_header.sequence_count,
		first_header_pointer = values.frame_header.first_header_pointer,
	}
	
	-- prepend header to the partially constructed frame
	frame = frame_header_generator:render():append(frame)

	-- calculate the CRC over the packet so far and append it to the end
	error_control_generator:set {
		frame_error_control_field = crc16.new():calculate(frame:bytes()):get(),
	}
	frame:append(error_control_generator:render())

	return frame
end

-- Transfer Frame containing only idle data
--     transfer_frame {
--         length = [int] (in byte),
--         frame_header = {
--             scid = [int],
--             vcid = [int],
--             mcfc = [int],
--             vcfc = [int],
--         },
--         clcw = [bitstream],
--         frame_error_control = [bool] (default: true),
--     }
local function transfer_frame_oid(values)
	if not values.length then
		error("No frame length given!")
	end
	
	-- Calculate the total overheader of headers, CLCW and error control field
	local length_overhead = 6
	if values.clcw then
		length_overhead = length_overhead + 4
	end
	values.frame_error_control = values.frame_error_control or true
	if values.frame_error_control then
		length_overhead = length_overhead + 2
	end
	
	if length_overhead > values.length then
		error(string.format([[Frame is to small. Must be at least %i octets
 (currently: %i octets)!]], length_overhead + 1, values.length))
	end
	
	local data_length = values.length - length_overhead
	
	local packet_data = bitstream.new(data_length * 8)
	for i = 1,data_length do
		local offset = (i - 1) * 8 + 1
		packet_data:insert(offset, 8, idle_data_pattern)
	end
	
	-- overwrite the first header pointer, thus indicating that this frame
	-- only contains idle data
	values.frame_header.first_header_pointer = IDLE
	values.data = packet_data
	
	return transfer_frame(values)
end

-- ----------------------------------------------------------------------------
-- Generate a Communication Link Control Word (CLCW)
--
-- See CCSDS 232.0-B-2
-- 
--     clcw {
--         status = [int] (default: 0),
--         vcid = [int],
--         flags = {
--             no_rf_available = [bool],
--             no_bit_lock = [bool],
--             lockout = [bool],
--             wait = [bool],
--             retransmit = [bool],
--         },
--         farm_b_counter = [int],
--         nr = [int],
--     }
local function clcw(values)
	local clcw_generator = generator.new(transfer_frame_clcw_definition)
	
	clcw_generator:set {
		status = values.status,
		vcid = values.vcid,
		
		no_rf_avail = values.flags.no_rf_available and 1 or 0,
		no_bit_lock = values.flags.no_bit_lock and 1 or 0,
		lockout = values.flags.lockout and 1 or 0,
		wait = values.flags.wait and 1 or 0,
		retransmit = values.flags.retransmit and 1 or 0,
		
		farm_b_counter = values.farm_b_counter,
		report_value  = values.nr,
	}
	
	return clcw_generator:render()
end

-- ----------------------------------------------------------------------------
-- Create a TM source packet.
-- 
-- @param values
--     Table containing all the information needed to build a source packet:
-- 
--     source_packet {
--         apid = [int],
--         sequence_count = [int],
--         data_field_header = [bitstream],
--         application_data = [bitstream]
--     }
--    
local function source_packet(values)
	local packet = bitstream.empty()
	local packet_length = 0
	
	-- length needed to calculate the packet length
	local data_field_header_length = 0
	if values.data_field_header ~= nil then
		packet = values.data_field_header .. packet
		data_field_header_length = #values.data_field_header
		
		packet_length = packet_length + data_field_header_length
	end

	local header = generator.new(source_packet_header_definition)
	local packet_length_field = data_field_header_length + #values.application_data - 1
	
	header:set {
		data_field_header_flag = values.data_field_header and 1 or 0,
		application_process_id = values.apid,
		sequence_count = values.sequence_count,
		packet_data_length = packet_length_field,
	}
	local source_packet_header = header:render()
	
	packet_length = packet_length + source_packet_header:length()
	packet = source_packet_header:append(packet)
	
	-- append the application data field
	packet:append(values.application_data)
	packet_length = packet_length + #values.application_data

	return packet
end

-- ----------------------------------------------------------------------------
return {
	_VERSION = '0.1',
	_DESCRIPTION = [[ TODO ]],
	_LICENSE = [[2-clause BSD License]],
	
	-- constants
	grouping = {
		CONTINUATIONl = 0,
		FIRST = 1,
		LAST = 2,
		STANDALONE = 3,
	},
	
	-- Possible values for the first_header field:
	IDLE = IDLE,
	NO_HEADER = NO_HEADER,
	
	-- functions
	transfer_frame = transfer_frame,
	transfer_frame_oid = transfer_frame_oid,
	clcw = clcw,
	source_packet = source_packet,
}

