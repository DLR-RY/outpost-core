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
local packetgenerator = require "packetgenerator"
local generator = require "packetgenerator.generator"

describe("packetgenerator.generator (Lua module) test", function()
	it("should generate packets with fixed values", function()
		local definition = {
			name = "fixed packet",
			structure = {
				{ name="a", type="fixed", length=3, value=2 },
				{ name="b", type="fixed", length=7, value=10 },
				{ name="c", type="fixed", length=6, value=0xC },
			}
		}
		
		local g = generator.new(definition)
		local t = g:render():bytes()
		
		assert.equals(2, #t)
		assert.equals(0x42, t[1])
		assert.equals(0x8C, t[2])
	end)
	
	it("should generate packets with editable values", function()
		local definition = {
			name = "fixed packet",
			structure = {
				{ name="a", type="editable", length=5 },
				{ name="b", type="editable", length=10 },
				{ name="c", type="editable", length=9 },
			}
		}
		
		local g = generator.new(definition)
		g:set {
			a = 18,
			b = 567,
			c = 413,
		}
		local t = g:render():bytes()
		
		assert.equals(3, #t)
		assert.equals(0x94, t[1])
		assert.equals(0x6F, t[2])
		assert.equals(0x9D, t[3])
	end)
	
	it("should generate packets with editable default values", function()
		local definition = {
			name = "fixed packet",
			structure = {
				{ name="a", type="editable", length=5 },
				{ name="b", type="editable", length=10, value=567 },
				{ name="c", type="editable", length=9, value=10 },
			}
		}
		
		local g = generator.new(definition)
		g:set {
			a = 18,
			c = 413,
		}
		local t = g:render():bytes()
		
		assert.equals(3, #t)
		assert.equals(0x94, t[1])
		assert.equals(0x6F, t[2])
		assert.equals(0x9D, t[3])
	end)
	
	it("should generate packets with mixed values", function()
		local definition = {
			name = "fixed packet",
			structure = {
				{ name="a", type="editable", length=5 },
				{ name="b", type="fixed", length=10, value=567 },
				{ name="c", type="editable", length=9 },
			}
		}
		
		local g = generator.new(definition)
		g:set {
			a = 18,
			c = 413,
		}
		local t = g:render():bytes()
		
		assert.equals(3, #t)
		assert.equals(0x94, t[1])
		assert.equals(0x6F, t[2])
		assert.equals(0x9D, t[3])
	end)
	
	it("should support deduced parameters", function()
		local definition = {
			name = "packet with deduced parameters",
			structure = {
				{ name="a", type="editable", length=8 },
				{ name="b", type="deduced" },
			}
		}
		
		local g = generator.new(definition)
		g:set {
			a = 18,
			b = bitstream.new('\xF5\x43\xAB\xCD\x9E'),
		}
		local t = g:render():bytes()
		
		assert.equals(6, #t)
		assert.equals(0x12, t[1])
		assert.equals(0xF5, t[2])
		assert.equals(0x43, t[3])
		assert.equals(0xAB, t[4])
		assert.equals(0xCD, t[5])
		assert.equals(0x9E, t[6])
	end)
	
	it("should report errors on unknown types", function()
		local definition = {
			name = "fixed packet",
			structure = {
				{ name="a", type="unknown", length=8 },
			}
		}
		local p = generator.new(definition)
		
		assert.error(function () p:render() end)
	end)
	
	it("should support editable groups", function()
		local definition = {
			name = "editable groups",
			structure = {
				{ name="n", type="editable", length=16, group={
						{ name="data", type="editable", length=8 },
					}
				},
			},
		}

		local p = generator.new(definition)
		p:set {
			n = {
				{ data = 5 },
				{ data = 7 },
			},
		}
		
		local t = p:render():bytes()
		
		assert.equals(4, #t)
		assert.equals(0x00, t[1])
		assert.equals(0x02, t[2])
		assert.equals(0x05, t[3])
		assert.equals(0x07, t[4])
	end)
	
	it("should support fixed length groups", function()
		local definition = {
			name = "fixed length groups",
			structure = {
				{ name="n", type="fixed", length=16, value=3, group={
						{ name="data", type="editable", length=8 },
					}
				},
			},
		}

		local p = generator.new(definition)
		p:set {
			n = {
				{ data = 1 },
				{ data = 2 },
				{ data = 3 },
			},
		}
		
		local t = p:render():bytes()
		
		assert.equals(5, #t)
		assert.equals(0x00, t[1])
		assert.equals(0x03, t[2])
		assert.equals(0x01, t[3])
		assert.equals(0x02, t[4])
		assert.equals(0x03, t[5])
	end)
	
	it("should support groups with multiple entries", function()
		local definition = {
			name = "Load Register",
			structure = {
				{ name="n", type="editable", length=16, group={
					{ name="address", type="editable", length=32 },
					{ name="data", type="deduced" },
					}
				}
			}
		}
		
		local p = generator.new(definition)
		p:set {
			n = {
				{ address=7,  data=bitstream.new('\x12\x45\xAB\x1E') },
                { address=10, data=bitstream.new('\xFE') },
            },
		}
		
		local b = p:render()
		local t = b:bytes()
		
		assert.equals(15, #t)
		assert.equals(0x00, t[1])
		assert.equals(0x02, t[2])
		
		assert.equals(0x00, t[3])
		assert.equals(0x00, t[4])
		assert.equals(0x00, t[5])
		assert.equals(0x07, t[6])
		assert.equals(0x12, t[7])
		assert.equals(0x45, t[8])
		assert.equals(0xAB, t[9])
		assert.equals(0x1E, t[10])
		
		assert.equals(0x00, t[11])
		assert.equals(0x00, t[12])
		assert.equals(0x00, t[13])
		assert.equals(0x0A, t[14])
		assert.equals(0xFE, t[15])
	end)
end)

