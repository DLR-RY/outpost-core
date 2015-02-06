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

local bitstream = require "bitstream"
local spp = require "packetgenerator.spp"

describe("packetgenerator.spp (Lua module) test", function()
	
	describe("Sequence generator", function()
		it("Should generate a simple sequence", function()
			a = spp.sequence_generator(6)
			
			for i = 0, 255 do
				local seq = i % 64
				assert.equals(seq, a.get())
			end
		end)
		
		it("Should accept initial values", function()
			a = spp.sequence_generator(7, 10)
			
			for i = 0, 255 do
				local seq = (i + 10) % 128
				assert.equals(seq, a.get())
			end
		end)
		
		it("Should generate multiple sequences", function()
			a = spp.sequence_generator(4)
			
			assert.equals(0, a.get(0))
			assert.equals(0, a.get(1))
			assert.equals(0, a.get(2))
			assert.equals(1, a.get(0))
			assert.equals(2, a.get(0))
			assert.equals(1, a.get(2))
			assert.equals(2, a.get(2))
			assert.equals(3, a.get(2))
			assert.equals(4, a.get(2))
			assert.equals(3, a.get(0))
			assert.equals(1, a.get(1))
		end)
		
		it("Should provide initial values for multiple sequences", function()
			a = spp.sequence_generator(7, { [0]=10, [1]=100, [2]=15 })
			
			assert.equals(10, a.get(0))
			assert.equals(100, a.get(1))
			assert.equals(15, a.get(2))
			assert.equals(11, a.get(0))
			assert.equals(12, a.get(0))
			assert.equals(16, a.get(2))
			assert.equals(17, a.get(2))
			assert.equals(18, a.get(2))
			assert.equals(19, a.get(2))
			assert.equals(13, a.get(0))
			assert.equals(101, a.get(1))
		end)
	end)
end)

