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

local l3test = require "l3test"

describe("Lua modules", function()
	describe("l3test (C++) module", function()
		it("should create an empty channel", function()
			c = l3test.channel()
			
			assert.equals(0, c:getNumberOfPackets())
			assert.equals(false, c:hasPacket())
		end)
		
		it("should receive data", function()
			c = l3test.channel()
			
			c:append { 1, 2, 3, 0x12 }
			c:finishPacket()
			
			assert.equals(1, c:getNumberOfPackets())
			assert.equals(true, c:hasPacket())
		end)
		
		it("should receive the correct data", function()
			c = l3test.channel()
			
			c:append { 1, 2, 3, 0x12 }
			c:append(5)
			c:append("16")
			c:finishPacket()
			
			packet = c:get()
			
			assert.equals(6, #packet)
			assert.same(packet, { 1, 2, 3, 0x12, 5, 16 })
		end)
		
		it("should report wrong append type", function()
			c = l3test.channel()
			
			assert.has_error(function() c:append(true) end)
			assert.has_error(function() c:append("hello") end)
		end)
		
		it("should send data", function()
			c = l3test.channel()
			
			c:send { 1, 2, 3, 0x12 }
			
			assert.equals(1, c:getNumberOfPackets())
			
			packet = c:get()
			assert.same(packet, { 1, 2, 3, 0x12 })
			c:next()
		end)
		
		it("should receive multiple packets", function()
			c = l3test.channel()
			
			c:append { 1, 2, 3, 0x12 }
			c:finishPacket()
			
			c:append { 3, 4, 5, 6 }
			c:finishPacket()
			
			c:append { 7, 8 }
			c:finishPacket()
			
			assert.equals(3, c:getNumberOfPackets())
		end)
		
		it("should receive the correct data from multiple packets", function()
			c = l3test.channel()
			
			c:append { 1, 2, 3, 0x12 }
			c:finishPacket()
			
			c:append { 3, 4, 5, 6 }
			c:finishPacket()
			
			c:append { 7, 8 }
			c:finishPacket()
			
			packet = c:get()
			assert.same(packet, { 1, 2, 3, 0x12 })
			c:next()
			
			packet = c:get()
			assert.same(packet, { 3, 4, 5, 6 })
			c:next()
			
			packet = c:get()
			assert.same(packet, { 7, 8 })
			c:next()
			
			assert.equals(false, c:hasPacket())
		end)
	end)
end)
