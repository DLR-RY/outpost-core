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
		local b = bitstream.new(45)
		assert.equals(45, b:bit_length())
	end)
	
	it("should return the length in bytes", function()
		local b = bitstream.new(48)
		assert.equals(6, b:length())
	end)
	
	it("should provide a length operator", function()
		local b = bitstream.new(32)
		assert.equals(4, #b)
	end)
	
	it("should fail for non byte sizes", function()
		local b = bitstream.new(25)
		assert.error(function() b:length() end)
	end)

	it("should be initialized to zero", function()
		local b = bitstream.new(10)
		for i = 1, 10 do
			assert.equals(0, b:get(i))
		end
	end)
	
	it("should support bitstreams of size 0", function()
		local b = bitstream.new(0)
		local t = b:bytes()
		assert.equals(0, #t)
	end)
	
	it("should support empty bitstreams", function()
		local b = bitstream.empty()
		local t = b:bytes()
		assert.equals(0, #t)
	end)
	
	it("should store bits", function()
		local b = bitstream.new(10)
		b:set(3, 0)
		b:set(5, true)
		b:set(6, false)
		b:set(7, 1)
		b:set(9, 10)
		
		for i = 1, 10 do
			local value
			if i == 5 or i == 7 or i == 9 then
				value = 1
			else
				value = 0
			end
			
			assert.equals(value, b:get(i))
		end
	end)
	
	it("should fail on out-of-bounds access", function()
		local b = bitstream.new(10)
		
		assert.error(function() b:set(0,   0) end)
		assert.error(function() b:set(11,  1) end)
		assert.error(function() b:set(100, 1) end)
	end)
	
	it("should store fields", function()
		local expected = {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			0, 1, 1, 1
		}
		
		local b = bitstream.new(20)
		b:insert(1, 5, 19)
		b:insert(6, 3, 4)
		b:insert(11, 2, 3)
		b:insert(13, 8, 0x67)
		
		for i = 1, 20 do
			assert.equals(expected[i], b:get(i))
		end
	end)
	
	it("should extract fields", function()
		local b = create_bitstream_from_bit_table {
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
		local input = {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			0, 1, 1, 1
		}
		local a = create_bitstream_from_bit_table(input)
		
		local b = a:copy()
		
		a:set(2, 1)
		a:set(20, 0)
		
		for i = 1, #input do
			assert.equals(input[i], b:get(i))
		end
		
		assert.equals(1, a:get(2))
		assert.equals(0, a:get(20))
	end)
	
	it("should create byte tables", function()
		local b = create_bitstream_from_bit_table {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
		}
		
		local t = b:bytes()
		
		assert.equals(2, #t)
		assert.equals(0x9C, t[1])
		assert.equals(0x36, t[2])
	end)
	
	it("shoud support creating bitstreams from strings", function()
		local b = bitstream.new('\x12\x23\xab\xcd')
		
		assert.equals(4, #b)
		
		local t = b:bytes()
		
		assert.equals(4, #t)
		assert.equals(0x12, t[1])
		assert.equals(0x23, t[2])
		assert.equals(0xab, t[3])
		assert.equals(0xcd, t[4])
	end)
	
	it("should output hexadezimal strings", function()
		local b = bitstream.new('\x12\x23\xab\xcd')
		assert.equals('12 23 AB CD', b:to_hex())
	end)
	
	it("should output hexadezimal strings for inputs with a length not divable by 8", function()
		local b = create_bitstream_from_bit_table {
			1, 0, 0, 1, 1, 1, 0, 0,
			0, 0, 1, 1, 0, 1, 1, 0,
			1, 1, 1
		}
		
		assert.equals('9C 36 E0', b:to_hex())
	end)
	
	it("should fail on byte tables with invalid size", function()
		local b = bitstream.new(23)
		
		-- size must be devidable by 8 to use the bytes function
		assert.error(function() b:bytes() end)
	end)
	
	it("should append bitstreams", function()
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
		
		b2:append(b3)
		
		assert.equals(11, b2:bit_length())
		
		b1:append(b2)
		
		assert.equals(24, b1:bit_length())
		
		local t = b1:bytes()
		
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
		
		local t = b5:bytes()
		
		assert.equals(3, #t)
		assert.equals(0x9C, t[1])
		assert.equals(0x36, t[2])
		assert.equals(0x9A, t[3])
	end)
	
	it("should append strings", function()
		local b = bitstream.new(8)
		
		b:append('\x12\x23')
		
		assert.equals(3, #b)
		
		local t = b:bytes()
		assert.equals(0x00, t[1])
		assert.equals(0x12, t[2])
		assert.equals(0x23, t[3])
	end)
	
	it("should be convertiable to a string", function()
		local input = '\xC2\xF5\x45\x01\x8B'
		local b = bitstream.new(input)
		
		assert.equals(5, #b)
		assert.equals(input, b:to_string())
		assert.equals(input, tostring(b))
	end)
	
	it("should be able to store a lot of data", function()
		local b = bitstream.new(1024)
		local t = b:bytes()
	end)
end)

