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
local spp = require "packetgenerator.spp"
local pus   = require "packetgenerator.pus"

describe("packetgenerator.pus (Lua module) test", function()

	it("should generate a data field header entry", function()
		local header = pus.tc.data_field_header {
			ack = pus.tc.ack.ACCEPTANCE + pus.tc.ack.START,
			service_type = 2,
			service_subtype = 3,
			source_id = 12
		}
		
		assert.equals(4, #header)
		assert.are.same({ 0x13, 0x02, 0x03, 0x0C }, header:bytes())
	end)
	
	it("source_id should be optional and default to zero", function()
		local header = pus.tc.data_field_header {
			ack = pus.tc.ack.ACCEPTANCE + pus.tc.ack.START,
			service_type = 2,
			service_subtype = 3
		}
		
		assert.equals(4, #header)
		assert.are.same({ 0x13, 0x02, 0x03, 0x00 }, header:bytes())
	end)
end)

