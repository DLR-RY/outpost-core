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
