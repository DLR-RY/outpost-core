--[[
Copyright (c) 2014-2015, 2017, German Aerospace Center (DLR)

This file is part of the development version of OUTPOST.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Authors:
- 2014-2015, 2017, Fabian Greif (DLR RY-AVS)
--]]

require("busted")

local cobs = require "cobs"

describe("Lua modules", function()

	-- The general functionality of the COBS module is covered in the C++
	-- COBS module unittest as part of outpost.
	-- 
	-- These tests only verify that basic functionality for the interfacing
	-- with Lua is available, not the correctness of the algorithm.
	
	describe("COBS (C++) module", function()
		it("should encode an empty string", function()
			local c = cobs.encode('')
			
			assert.equals('\x01', c)
		end)
		
		it("should encode a string containing a single zero", function()
			local c = cobs.encode('\x00')
			
			assert.equals('\x01\x01', c)
		end)
		
		it("should decode a string", function()
			local c = cobs.decode('\x01\x01')
			
			assert.equals('\x00', c)
		end)
		
		it("should encode a string with a zero at end", function()
			local c = cobs.encode('\x45\x00\x00\xAB\x2A\x2C\x4C\x79\x00')
			
			assert.equals('\x02\x45\x01\x06\xAB\x2A\x2C\x4C\x79\x01', c)
		end)
		
		it("should do a full round trip", function()
			local i = 'Hello World!\x00This is a abitrary string.'
			
			local e = cobs.encode(i)
			local o = cobs.decode(e)
			assert.equals(i, o)
		end)
	end)
end)

