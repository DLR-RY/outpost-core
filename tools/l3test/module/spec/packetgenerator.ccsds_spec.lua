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

describe("packetgenerator.ccsds (Lua module) test", function()
	
	describe("Sequence generator", function()
		it("Should generate a simple sequence", function()
			a = ccsds.sequence_generator(6)
			
			for i = 0, 255 do
				local seq = i % 64
				assert.equals(seq, a.get())
			end
		end)
		
		it("Should accept initial values", function()
			a = ccsds.sequence_generator(7, 10)
			
			for i = 0, 255 do
				local seq = (i + 10) % 128
				assert.equals(seq, a.get())
			end
		end)
		
		it("Should generate multiple sequences", function()
			a = ccsds.sequence_generator(4)
			
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
			a = ccsds.sequence_generator(7, { [0]=10, [1]=100, [2]=15 })
			
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
	
	-- The following tests are based on "CCSDS 230.1-G-2 TC Synchroniyation
	-- and Channel Coding", Annex F.
	describe("CCSDS 230.1-G-2 Tests", function()

		it("Example 1: type BC frame with command 'Unlock'", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_BC,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Unlock command
				application_data = ccsds.command.unlock()
			}
		
			assert.equals(8, #frame)
			assert.are.same({ 0x30, 0x1B, 0x00 ,0x07, 0x00, 0x00, 0x4C, 0xA9 }, frame:bytes())
		end)

		it("Example 2: type BC frame with command 'Set V(R)'", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_BC,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Set V(R) command
				application_data = ccsds.command.set_v(0)
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x30, 0x1B, 0x00 ,0x09, 0x00, 0x82, 0x00, 0x00, 0xF6, 0xF0 }, frame:bytes())
		end)

		it("Example 4: type BC frame with command 'Set V(R)'", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_BC,
					scid = 0x1B,
					vcid = 1,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Set V(R) command
				application_data = ccsds.command.set_v(0)
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x30, 0x1B, 0x04 ,0x09, 0x00, 0x82, 0x00, 0x00, 0xF0, 0x51 }, frame:bytes())
		end)

		it("Example 5: type BC frame with command 'Set V(R)'", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_BC,
					scid = 0x1B,
					vcid = 1,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Set V(R) command
				application_data = ccsds.command.set_v(16)
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x30, 0x1B, 0x04 ,0x09, 0x00, 0x82, 0x00, 0x10, 0xE2, 0x60 }, frame:bytes())
		end)

		it("Example 6: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 255
				},
				application_data = bitstream.new('\x01')
			}
		
			assert.equals(8, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x07, 0xFF, 0x01, 0x70, 0xFB }, frame:bytes())
		end)

		it("Example 7: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0
				},
				application_data = bitstream.new('\x01\x02')
			}
		
			assert.equals(9, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x08, 0x00, 0x01, 0x02, 0xBE, 0x58 }, frame:bytes())
		end)

		it("Example 8: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 1
				},
				application_data = bitstream.new('\x01\x02\x03')
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x09, 0x01, 0x01, 0x02, 0x03, 0xF2, 0x93 }, frame:bytes())
		end)

		it("Example 9: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 2
				},
				application_data = bitstream.new('\x01\x02\x03\x04')
			}
		
			assert.equals(11, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x0A, 0x02, 0x01, 0x02, 0x03,
			                  0x04, 0x3C, 0xEB }, frame:bytes())
		end)

		it("Example 10: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 6
				},
				application_data = bitstream.new('\x01\x02\x03\x04\x05\x06\x07\x08')
			}
		
			assert.equals(15, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x0E, 0x06, 0x01, 0x02, 0x03,
			                  0x04, 0x05, 0x06, 0x07, 0x08, 0x14, 0xBB }, frame:bytes())
		end)

		it("Example 11: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 7
				},
				application_data = bitstream.new('\x01\x02\x03\x04\x05\x06\x07\x08\x09')
			}
		
			assert.equals(16, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x0F, 0x07, 0x01, 0x02, 0x03,
			                  0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xCF, 0x90 }, frame:bytes())
		end)

		it("Example 12: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0
				},
				application_data = bitstream.new('\xC0\x10\x00\xC0\x00\x00\x03\x2E\xAF\x8A\x06')
			}
		
			assert.equals(18, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x11, 0x00, 0xC0, 0x10, 0x00,
			                  0xC0, 0x00, 0x00, 0x03, 0x2E, 0xAF, 0x8A, 0x06,
			                  0x9F, 0x71 }, frame:bytes())
		end)

		it("Example 13: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0
				},
				application_data = bitstream.new('\xC1\x10\x00\xFF\xFF\x00\x04\x01\x02\x03\x11\x82')
			}
		
			assert.equals(19, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x12, 0x00, 0xC1, 0x10, 0x00,
			                  0xFF, 0xFF, 0x00, 0x04, 0x01, 0x02, 0x03, 0x11,
			                  0x82, 0x8D, 0x80 }, frame:bytes())
		end)

		it("Example 14: type AD frame for Data on the Sequence-Controlled Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_AD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0
				},
				application_data = bitstream.new('\xC1\x11\x04\xC0\x00\x00\x04\x01\x02\x03\x72\x17')
			}

			assert.equals(19, #frame)
			assert.are.same({ 0x00, 0x1B, 0x00 ,0x12, 0x00, 0xC1, 0x11, 0x04,
			                  0xC0, 0x00, 0x00, 0x04, 0x01, 0x02, 0x03, 0x72,
			                  0x17, 0x8D, 0x80 }, frame:bytes())
		end)
	
		it("Example 15: type BD frame for Data on the Expedited Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_BD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0	-- all type BD frame should have sequence number 0
				},
				application_data = bitstream.new('\xE1')
			}
		
			assert.equals(8, #frame)
			assert.are.same({ 0x20, 0x1B, 0x00 ,0x07, 0x00, 0xE1, 0xBB, 0x22 }, frame:bytes())
		end)
	
		it("Example 16: type BD frame for Data on the Expedited Service", function()
			frame = ccsds.transfer_frame {
				frame_header = {
					type = ccsds.TYPE_BD,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0	-- all type BD frame should have sequence number 0
				},
				application_data = bitstream.new('\xC0\x10\x00\xC0\x00\x00\x03\x2E\xAF\x8A\x06')
			}
		
			assert.equals(18, #frame)
			assert.are.same({ 0x20, 0x1B, 0x00 ,0x11, 0x00, 0xC0, 0x10, 0x00,
			                  0xC0, 0x00, 0x00, 0x03, 0x2E, 0xAF, 0x8A, 0x06,
			                  0xD9, 0x65 }, frame:bytes())
		end)
	end)
end)

