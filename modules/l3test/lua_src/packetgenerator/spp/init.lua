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

--local current_folder = (...):gsub('%.[^%.]+$', '')
local current_folder = (...)

local tm = require(current_folder .. '.tm')
local tc = require(current_folder .. '.tc')

local bitstream = require('bitstream')
local generator = require('packetgenerator.generator')

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
-- Convert a time expressed as fractional seconds to the CUC 4.2 format
--
-- Does not contain the p-field, but only the t-field. The total length
-- of the generated bitstream is always 6 byte. 
--
-- @param time
--     Time in seconds
--
-- @returns
--     Bitstream with the time formated as CUC 4.2
local function cuc42(time)
	local definition = {
		name = "CUC 4.2",
		description = "CCSDS Unsegmented Time Code 4.2",
		structure = {
			{ name="coarse", type="editable", length=32 },
			{ name="fine", type="editable", length=16 },
		}
	}
	
	local seconds = math.floor(time)
	local fractional = math.floor((2^16 - 1) * (time - seconds))
	
	local values = {
		coarse = seconds,
		fine = fractional,
	}
	
	return generator.new(definition):set(values):render()
end

-- ----------------------------------------------------------------------------
return {
	_VERSION = '0.1',
	_DESCRIPTION = [[ TODO ]],
	_LICENSE = [[2-clause BSD License]],
	
	-- modules
	tm = tm,
	tc = tc,
	
	-- functions
	sequence_generator = sequence_generator,
	cuc42 = cuc42,
}

