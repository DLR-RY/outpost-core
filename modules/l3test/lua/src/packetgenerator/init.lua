--[[
Copyright (c) 2014-2015, 2017, German Aerospace Center (DLR)

This file is part of the development version of OUTPOST.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Authors:
- 2014-2015, 2017, Fabian Greif (DLR RY-AVS)
--]]

local current_folder = (...):gsub('%.init$', '')

local generator = require(current_folder .. '.generator')

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

