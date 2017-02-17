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

local crc16 = require "crc16"

describe("Lua modules", function()
	describe("crc16 (C++) module", function()
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 1", function()
			local c = crc16.new()
			
			c:update(0x00)
			c:update(0x00)
			
			assert.equals(0x1D0F, c:get())
		end)
		
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 2", function()
			local c = crc16.new()
			
			c:update(0x00)
			c:update(0x00)
			c:update(0x00)
			
			assert.equals(0xCC9C, c:get())
		end)
		
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 3", function()
			local c = crc16.new()
			
			c:update(0xAB)
			c:update(0xCD)
			c:update(0xEF)
			c:update(0x01)
			
			assert.equals(0x04A2, c:get())
		end)
		
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 4", function()
			local c = crc16.new()
			
			c:update(0x14)
			c:update(0x56)
			c:update(0xF8)
			c:update(0x9A)
			c:update(0x00)
			c:update(0x01)
			
			assert.equals(0x7FD5, c:get())
		end)
		
		it("should get zero when including the CRC value", function()
			local c = crc16.new()
			
			c:update(0x14)
			c:update(0x56)
			c:update(0xF8)
			c:update(0x9A)
			c:update(0x00)
			c:update(0x01)
			c:update(0x7F)
			c:update(0xD5)
			
			assert.equals(0x0000, c:get())
		end)
		
		it("should work on tables of bytes", function()
			local c = crc16.new()
			
			local t = {
				0x10, 0x82, 0x8F, 0x7D, 0x01,
				0x7E, 0x10, 0x81, 0x08, 0x34, 0x7E, 0xE5, 0xC3,
			}
			
			c:calculate(t)
			
			assert.equals(0x8541, c:get())
		end)
		
		it("should work on a subset", function()
			local c = crc16.new()
			
			local t = {
				0x14, 0x56, 0xF8, 0x9A, 0x00, 0x01, 0x7F, 0xD5
			}
			
			assert.equals(0x0000, c:reset():calculate(t):get())
			assert.equals(0x7FD5, c:reset():calculate(t, 1, -3):get())
			assert.equals(0x7FD5, c:reset():calculate(t, 1, 6):get())
			assert.equals(0x3197, c:reset():calculate(t, 3, -3):get())
		end)
		
		it("should work with strings", function()
			local c = crc16.new()
			
			c:calculate('\x14\x56\xF8\x9A\x00\x01')
			
			assert.equals(0x7FD5, c:get())
		end)
		
		it("should have a short version for the calculation", function()
			assert.equals(0x7FD5, crc16.calculate('\x14\x56\xF8\x9A\x00\x01'))
		end)
	end)
end)

