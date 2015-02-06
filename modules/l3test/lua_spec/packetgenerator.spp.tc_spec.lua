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

describe("packetgenerator.spp.tc (Lua module) test", function()
	
	-- The following tests are based on "CCSDS 230.1-G-2 TC Synchroniyation
	-- and Channel Coding", Annex F.
	describe("CCSDS 230.1-G-2 Tests", function()

		it("Example 1: type BC frame with command 'Unlock'", function()
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_BC,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Unlock command
				application_data = spp.tc.command.unlock()
			}
		
			assert.equals(8, #frame)
			assert.are.same({ 0x30, 0x1B, 0x00 ,0x07, 0x00, 0x00, 0x4C, 0xA9 }, frame:bytes())
		end)

		it("Example 2: type BC frame with command 'Set V(R)'", function()
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_BC,
					scid = 0x1B,
					vcid = 0,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Set V(R) command
				application_data = spp.tc.command.set_v(0)
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x30, 0x1B, 0x00 ,0x09, 0x00, 0x82, 0x00, 0x00, 0xF6, 0xF0 }, frame:bytes())
		end)

		it("Example 4: type BC frame with command 'Set V(R)'", function()
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_BC,
					scid = 0x1B,
					vcid = 1,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Set V(R) command
				application_data = spp.tc.command.set_v(0)
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x30, 0x1B, 0x04 ,0x09, 0x00, 0x82, 0x00, 0x00, 0xF0, 0x51 }, frame:bytes())
		end)

		it("Example 5: type BC frame with command 'Set V(R)'", function()
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_BC,
					scid = 0x1B,
					vcid = 1,
					sequence_count = 0	-- all type BC frame have sequence number 0
				},
				-- Set V(R) command
				application_data = spp.tc.command.set_v(16)
			}
		
			assert.equals(10, #frame)
			assert.are.same({ 0x30, 0x1B, 0x04 ,0x09, 0x00, 0x82, 0x00, 0x10, 0xE2, 0x60 }, frame:bytes())
		end)

		it("Example 6: type AD frame for Data on the Sequence-Controlled Service", function()
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_AD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_BD,
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
			local frame = spp.tc.transfer_frame {
				frame_header = {
					type = spp.tc.TYPE_BD,
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
	
	describe("generation of source packets", function()
		
		it("should a create packet with CRC by default", function()
			local packet = spp.tc.source_packet {
				apid = 100,
				sequence_count = 1234,
				application_data = bitstream.new('\x00'),
			}
			
			assert.equals(9, #packet)
			
			-- Note: CRC not checked but assumed to be correct
			assert.are.same( { 0x10, 0x64, 0xC4, 0xD2, 0x00, 0x02, 0x00, 6, 187 }, packet:bytes())
		end)
		
		it("should a be possible to disable CRC generation", function()
			local packet = spp.tc.source_packet {
				apid = 100,
				sequence_count = 1234,
				application_data = bitstream.new('\x00'),
				packet_error_control = false,
			}
			
			assert.equals(7, #packet)
			assert.are.same( { 0x10, 0x64, 0xC4, 0xD2, 0x00, 0x00, 0x00 }, packet:bytes())
		end)
		
		it("should support a data field header", function()
			local packet = spp.tc.source_packet {
				apid = 100,
				sequence_count = 1234,
				data_field_header = bitstream.new('\x13\x02\x03\x00'),
				application_data = bitstream.new('\x00'),
				packet_error_control = false,
			}
			
			assert.equals(11, #packet)
			assert.are.same( { 0x18, 0x64, 0xC4, 0xD2, 0x00, 0x04, 0x13, 0x02,
			                   0x03, 0x00, 0x00 }, packet:bytes())
		end)
	end)
end)

