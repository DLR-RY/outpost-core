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

require("busted")

local crc16 = require "crc16"

describe("Lua modules", function()
	describe("crc16 (C++) module", function()
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 1", function()
			c = crc16.new()
			
			c:update(0x00)
			c:update(0x00)
			
			assert.equals(0x1D0F, c:get())
		end)
		
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 2", function()
			c = crc16.new()
			
			c:update(0x00)
			c:update(0x00)
			c:update(0x00)
			
			assert.equals(0xCC9C, c:get())
		end)
		
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 3", function()
			c = crc16.new()
			
			c:update(0xAB)
			c:update(0xCD)
			c:update(0xEF)
			c:update(0x01)
			
			assert.equals(0x04A2, c:get())
		end)
		
		-- based on ECSS-E-70-41A Annex A (Version from January 2003).
		it("should pass PUS test 4", function()
			c = crc16.new()
			
			c:update(0x14)
			c:update(0x56)
			c:update(0xF8)
			c:update(0x9A)
			c:update(0x00)
			c:update(0x01)
			
			assert.equals(0x7FD5, c:get())
		end)
		
		it("should get zero when including the CRC value", function()
			c = crc16.new()
			
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
			c = crc16.new()
			
			t = {
				0x10, 0x82, 0x8F, 0x7D, 0x01,
				0x7E, 0x10, 0x81, 0x08, 0x34, 0x7E, 0xE5, 0xC3,
			}
			
			c:calculate(t)
			
			assert.equals(0x8541, c:get())
		end)
		
		it("should work on a subset", function()
			c = crc16.new()
			
			t = {
				0x14, 0x56, 0xF8, 0x9A, 0x00, 0x01, 0x7F, 0xD5
			}
			
			assert.equals(0x0000, c:reset():calculate(t):get())
			assert.equals(0x7FD5, c:reset():calculate(t, 1, -3):get())
			assert.equals(0x7FD5, c:reset():calculate(t, 1, 6):get())
			assert.equals(0x3197, c:reset():calculate(t, 3, -3):get())
		end)
		
		it("should work with strings", function()
			
		end)
	end)
end)

