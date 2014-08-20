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


Packet generator engine for Lua
===============================

Example::
	
	local generator = require "packetgenerator"
	
	local definition = {
		name = "CC002001",
		description = "CamSpwTcGen",
		detailed_description = "TC(2,134).02h Generic Camera SpW command",
		structure = {
			{ name="FID", type="fixed", length=8, value=2 },
			{ name="GroundId", type="editable", length=16 },
			{ name="SpwAslType", type="editable", length=8 },
			{ name="SpwAslLength", type="editable", length=16, group={
					{ name="SpwAslData", type="editable", length=8 },
				}
			},
		},
	}

	local p = generator.new(definition)
	p:set {
		GroundId = 12,
		SpwAslType = 1,
		SpwAslLength = {
			{ SpwAslData = 1 },
			{ SpwAslData = 2 },
			{ SpwAslData = 3 },
			{ SpwAslData = 4 },
		},
	}
	
	-- Render the packet and generate a table with one byte per entry
	t = p:render():bytes()
	for _, v in pairs(t) do
		io.write(string.format('0x%02x ', v))
	end

]]--

local bitstream = require "bitstream"
local inspect = require "inspect"

local Packet = {}
Packet.__index = Packet

-- Merge two tables
local function merge(t1, t2)
    for k, v in pairs(t2) do
    	if type(v) == "table" then
    		if type(t1[k] or false) == "table" then
    			merge(t1[k] or {}, t2[k] or {})
    		else
    			t1[k] = v
    		end
    	else
    		t1[k] = v
    	end
    end
    return t1
end

-- Fill variable entries in the database description.
--
-- Can be called multiple times with incomplete tables. Older entries are
-- overwritten when assigning a value to the same entry multiple times.
--
function Packet.set(self, value)
	self._values = self._values or {}
	merge(self._values, value)
	return self
end

-- forward declaration
local _get_length_group
local _get_length

-- Calculate length for every group entry
--
function _get_length_group(structure, values_group)
	local length = 0
	-- TODO allow for implicit groups without explicit empty table
	for i = 1, #values_group do
		local values = values_group[i]
		length = length + _get_length(structure, values)
	end
	
	return length
end

-- Get length of a sub-packet
--
function _get_length(structure, values)
	local length = 0
	
	for i = 1, #structure do
		local entry = structure[i]
		
		if entry.type == "deduced" then
			-- The length of deduced parameters are read directly from the
			-- length of the value. The length is in bytes not bits here.
			
			-- TODO does this update the top-level value?
			entry.length = #values[entry.name] * 8
		end
		length = length + entry.length
		
		if entry.group ~= nil then
			length = length + _get_length_group(entry.group, values[entry.name])
		end
	end
	return length
end

-- Get length of the finished packet
-- 
-- Requires the group values to be set correctly.
-- 
function Packet.get_length(self)
	return _get_length(self._packet.structure, self._values)
end

-- forward declaration
local _fill_group
local _fill

-- Fill a group definition with its values.
-- 
-- Recursivly used by _fill() to fill groups.
-- 
function _fill_group(packet, position, structure, values_group)
	local pos = position
	for i = 1, #values_group do
		local values = values_group[i]
		pos = _fill(packet, pos, structure, values)
	end
	
	return pos
end

-- Fill a packet (bitstream) with values given as a lua table.
--
function _fill(packet, position, structure, values)
	local pos = position
	
	for i = 1, #structure do
		local entry = structure[i]
		
		local value = nil
		if entry.type == "fixed" then
			value = entry.value
		elseif entry.type == "editable" or entry.type == "deduced" then
			value = values[entry.name]
		else
			error(string.format("Invalid type (%s) for element '%s'", entry.type, entry.name), 3)
		end
		
		-- If no value is given use a potential default value from the
		-- structure definition
		if value == nil then
			if entry.value == nil then
				error(string.format("Value for element '%s' required", entry.name), 3)
			end
			value = entry.value
		end
		
		if entry.group ~= nil then
			-- If the value is a group, use the group length as the value
			-- to put in the packet.
			value = #values[entry.name]
		end
		
		if entry.type == "deduced" then
			-- Value has to be of type 'bitstream'
			local t = value:bytes()
			
			for i = 1, #t do
				packet:insert(pos + (i - 1) * 8, 8, t[i])
			end
			
			-- The length of deduced parameters are read directly from the
			-- length of the value.
			pos = pos + value:bit_length()
		else
			packet:insert(pos, entry.length, value)
			pos = pos + entry.length
		end
		
		if entry.group ~= nil then
			pos = _fill_group(packet, pos, entry.group, values[entry.name])
		end
	end
	return pos
end

-- Render a packet into a bitstream.
--
-- Uses the values given via 'set(...)' to fill the variable entries in the
-- database. Raises an error if a entry is variable but no value was assigned.
--
function Packet.render(self)
	local length = self:get_length()
	
	if length % 8 ~= 0 then
		-- Messages must fit into byte orientated containers
		error(string.format("total length of the packet (here: %d) must be divisible by 8 without reminder", length), 2)
	end
	
	local packet = bitstream.new(length)
	_fill(packet, 1, self._packet.structure, self._values)
	return packet
end

function Packet.__tostring(self)
	local t = {}
	
	t[#t+1] = self._packet.name
	t[#t+1] = " (" .. self._packet.description .. ")"
	
	return table.concat(t, '')
end

-- ----------------------------------------------------------------------------
local module = {
	_VERSION = '0.1',
	_DESCRIPTION = '',
	_LICENSE = [[2-clause BSD License]],
	
	-- exported functions
	merge = merge,
}

function module.new(value)
	if value == nil then
		error(string.format("Valid packet definition required"), 2)
	end
	
	local self = { _packet=value }
	setmetatable(self, Packet)
	return self
end

function module.render(definition, values)
	local g = module.new(definition)
	g:set(values)
	return g:render()
end

return module

