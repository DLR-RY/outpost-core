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

local current_folder = (...):gsub('%.init$', '')

local generator = require(current_folder .. '.generator')
local spp = require(current_folder .. '.spp')
local pus = require(current_folder .. '.pus')

-- ----------------------------------------------------------------------------
-- Print a packet represented by a bitstream
--
-- @param packet
--     Bitstream to print
-- @param linebreak
--     Number of bytes after which a linebreak is added. Default is 8, set to
--     zero to disable it.
--
local function tostring(packet, linebreak)
	local t = {}
	local b = packet:bytes()
	local linebreak = linebreak or 8
	
	for i = 1, #b do
		t[#t + 1] = string.format('0x%02X', b[i])
		
		if i ~= #b then
			t[#t + 1] = ', '
		end
		if linebreak ~= 0 then
			if (i - 1) % linebreak == (linebreak - 1) then
				t[#t + 1] = '\n'
			end
		end
	end
	return table.concat(t)
end

local function tostring_pattern(pattern, packet, linebreak)
	local t = {}
	local b = packet:bytes()
	local linebreak = linebreak or 8
	
	for i = 1, #b do
		t[#t + 1] = string.format(pattern, b[i])
		
		if i ~= #b then
			t[#t + 1] = ', '
		end
		if linebreak ~= 0 then
			if (i - 1) % linebreak == (linebreak - 1) then
				t[#t + 1] = '\n'
			end
		end
	end
	return table.concat(t)
end

return {
	-- functions	
	tostring = tostring,
	tostring_pattern = tostring_pattern,

	-- modules
	generator = generator,
	spp = spp,
	pus = pus,
}

