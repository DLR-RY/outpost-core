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

local data_field_header_definition = {
	name = "TC Data Field Header",
	description = "TC Data Field Header",
	structure = {
		{ name="CCSDS_Secondary_Header_Flag", type="fixed", length=1, value=0 },
		{ name="PUS_Version_Number", type="fixed", length=3, value=1 },
		{ name="ack", type="editable", length=4 },
		{ name="service_type", type="editable", length=8 },
		{ name="service_subtype", type="editable", length=8 },
		{ name="source_id", type="editable", length=8, value=0 },
	}
}

return {
	_VERSION = '0.1',
	_DESCRIPTION = [[ TODO ]],
	_LICENSE = [[2-clause BSD License]],
	
	-- variables
	data_field_header_definition = data_field_header_definition,
	
	-- constants
	ACK = {
		NONE = 0,
	
		ACCEPTANCE = 0x1,
		START = 0x2,
		PROGRESS = 0x4,
		COMPLETION = 0x8,
		
		ALL = 0xF
	},
}

