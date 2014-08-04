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

local bitstream = require "bitstream"

local function create_bitstream_from_bit_table(input)
	local length = #input
	local b = bitstream.new(length)
	
	-- fill bitstream with data
	for i = 1, length do
		b:set(i, input[i])
	end
	
	return b
end

describe("bitstream (C++) module test", function()

	it("should have the correct size", function()
		b = bitstream.new(45)
		assert.equals(45, b:bit_length())
	end)
	
	it("should return the length in bytes", function()
		b = bitstream.new(48)
		assert.equals(6, b:length())
	end)
	
	it("should provide a length operator", function()
		b = bitstream.new(32)
		assert.equals(4, #b)
	end)
	
	it("should fail for non byte sizes", function()
		b = bitstream.new(25)
		assert.error(function() b:length() end)
	end)

	it("should be initialized to zero", function()
		b = bitstream.new(10)
		for i = 1, 10 do
			assert.equals(0, b:get(i))
		end
	end)
	
	it("should support empty bitstreams", function()
		b = bitstream.new(0)
		t = b:bytes()
		assert.equals(0, #t)
	end)
	
	it("should store bits", function()
		b = bitstream.new(10)
		b:set(3, 0)
		b:set(5, true)
		b:set(6, false)
		b:set(7, 1)
		b:set(9, 10)
		
		for i = 1, 10 do
			if i == 5 or i == 7 or i == 9 then
				value = 1
			else
				value = 0
			end
			
			assert.equals(value, b:get(i))
		end
	end)
	
	it("should fail on out-of-bounds access", function()
		b = bitstream.new(10)
		
		assert.error(function() b:set(0,   0) end)
		assert.error(function() b:set(11,  1) end)
		assert.error(function() b:set(100, 1) end)
	end)
	
	it("should store fields", function()
		expected = {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			0, 1, 1, 1
		}
		
		b = bitstream.new(20)
		b:insert(1, 5, 19)
		b:insert(6, 3, 4)
		b:insert(11, 2, 3)
		b:insert(13, 8, 0x67)
		
		for i = 1, 20 do
			assert.equals(expected[i], b:get(i))
		end
	end)
	
	it("should extract fields", function()
		b = create_bitstream_from_bit_table {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			0, 1, 1, 1
		}
		
		assert.equals(19,   b:extract(1, 5))
		assert.equals(4,    b:extract(6, 3))
		assert.equals(3,    b:extract(11, 2))
		assert.equals(0x67, b:extract(13, 8))
	end)
	
	it("should copy bitstreams", function()
		input = {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			0, 1, 1, 1
		}
		a = create_bitstream_from_bit_table(input)
		
		b = a:copy()
		
		a:set(2, 1)
		a:set(20, 0)
		
		for i = 1, #input do
			assert.equals(input[i], b:get(i))
		end
		
		assert.equals(1, a:get(2))
		assert.equals(0, a:get(20))
	end)
	
	it("should create byte tables", function()
		b = create_bitstream_from_bit_table {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
		}
		
		t = b:bytes()
		
		assert.equals(2, #t)
		assert.equals(0x9C, t[1])
		assert.equals(0x36, t[2])
	end)
	
	it("shoud support creating bitstreams from strings", function()
		b = bitstream.new('\x12\x23\xab\xcd')
		
		assert.equals(4, #b)
		
		t = b:bytes()
		
		assert.equals(4, #t)
		assert.equals(0x12, t[1])
		assert.equals(0x23, t[2])
		assert.equals(0xab, t[3])
		assert.equals(0xcd, t[4])
	end)
	
	it("should output hexadezimal strings", function()
		b = bitstream.new('\x12\x23\xab\xcd')
		assert.equals('12 23 AB CD', b:to_hex())
	end)
	
	it("should output hexadezimal strings for inputs with a length not divable by 8", function()
		b = create_bitstream_from_bit_table {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			1, 1, 1
		}
		
		assert.equals('9C 36 E0', b:to_hex())
	end)
	
	it("should fail on byte tables with invalid size", function()
		b = bitstream.new(23)
		
		-- size must be devidable by 8 to use the bytes function
		assert.error(function() b:bytes() end)
	end)
	
	it("should append bitstreams", function()
		input1 = {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0,
		}
		
		input2 = {
			1, 1, 0, 1, 0,
		}

		input3 = {
			0, 1, 1, 0, 1, 0,
		}
					
		b1 = bitstream.new(13)
		b2 = bitstream.new(5)
		b3 = bitstream.new(6)
		
		-- fill bitstream with data
		for i = 1, #input1 do
			b1:set(i, input1[i])
		end
		
		for i = 1, #input2 do
			b2:set(i, input2[i])
		end
		
		for i = 1, #input3 do
			b3:set(i, input3[i])
		end
		
		b2:append(b3)
		
		assert.equals(11, b2:bit_length())
		
		b1:append(b2)
		
		assert.equals(24, b1:bit_length())
		
		t = b1:bytes()
		
		assert.equals(3, #t)
		assert.equals(0x9C, t[1])
		assert.equals(0x36, t[2])
		assert.equals(0x9A, t[3])
	end)
	
	it("should concatenate bitstreams", function()
		local input1 = {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0,
		}
		
		local input2 = {
			1, 1, 0, 1, 0,
		}

		local input3 = {
			0, 1, 1, 0, 1, 0,
		}
					
		local b1 = bitstream.new(13)
		local b2 = bitstream.new(5)
		local b3 = bitstream.new(6)
		
		-- fill bitstream with data
		for i = 1, #input1 do
			b1:set(i, input1[i])
		end
		
		for i = 1, #input2 do
			b2:set(i, input2[i])
		end
		
		for i = 1, #input3 do
			b3:set(i, input3[i])
		end
		
		local b4 = b2 .. b3
		
		assert.equals(5, b2:bit_length())
		assert.equals(6, b3:bit_length())
		assert.equals(11, b4:bit_length())
		
		local b5 = b1 .. b4
		
		assert.equals(13, b1:bit_length())
		assert.equals(11, b4:bit_length())
		assert.equals(24, b5:bit_length())
		
		t = b5:bytes()
		
		assert.equals(3, #t)
		assert.equals(0x9C, t[1])
		assert.equals(0x36, t[2])
		assert.equals(0x9A, t[3])
	end)
	
	it("should append strings", function()
		b = bitstream.new(8)
		
		b:append('\x12\x23')
		
		assert.equals(3, #b)
		
		t = b:bytes()
		assert.equals(0x00, t[1])
		assert.equals(0x12, t[2])
		assert.equals(0x23, t[3])
	end)
end)

