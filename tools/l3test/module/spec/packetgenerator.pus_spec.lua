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
local ccsds = require "packetgenerator.ccsds"
local pus   = require "packetgenerator.pus"

describe("packetgenerator.pus (Lua module) test", function()

	it("", function()
		frame = ccsds.transfer_frame {
			frame_header = {
				type = ccsds.TYPE_BD,
				scid = 0x1B,
				vcid = 0,
				sequence_count = 0
			},
			application_data = bitstream.new('\xC0\x10\x00\xC0\x00\x00\x03\x2E\xAF\x8A\x06')
		}
	
		assert.equals(18, #frame)
		assert.are.same({ 0x20, 0x1B, 0x00 ,0x11, 0x00, 0xC0, 0x10, 0x00,
		                  0xC0, 0x00, 0x00, 0x03, 0x2E, 0xAF, 0x8A, 0x06,
		                  0xD9, 0x65 }, frame:bytes())
	end)
end)

