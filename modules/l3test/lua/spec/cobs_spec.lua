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

