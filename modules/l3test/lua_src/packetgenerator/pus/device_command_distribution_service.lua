--[[2-clause BSD License

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
POSSIBILITY OF SUCH DAMAGE. --]]

local definition_distribute_on_off_commands = {
	name = "Distribute On-Off Commands",
	description = "",
	header = {
		service_type = 2,
		service_subtype = 1,
	},
	structure = {
		{ name="n", type="editable", length=16, group= {
				{ name="address", type="editable", length=32 },
			}
		},
	}
}

local definition_distribute_register_load_commands = {
	name = "Distribute Register Load Commands",
	description = "",
	header = {
		service_type = 2,
		service_subtype = 2,
	},
	structure = {
		{ name="n", type="editable", length=16, group= {
				{ name="address", type="editable", length=16 },
				-- Data length is implicitly known
				{ name="data", type="deduced" },
			}
		},
	}
}

local definition_distribute_cpdu_commands = {
	name = "Distribute CPDU commands",
	description = "",
	header = {
		service_type = 2,
		service_subtype = 3,
	},
	structure = {
		{ name="output_line_id", type="editable", length=8 },
		{ name="duration", type="editable", length=8 },
	}
}

return {
	_VERSION = '0.1',
	_DESCRIPTION = [[ TODO ]],
	_LICENSE = [[2-clause BSD License]],
	
	distribute_cpdu_commands = definition_distribute_cpdu_commands,
	distribute_register_load_commands = definition_distribute_register_load_commands,
	distribute_on_off_commands = definition_distribute_on_off_commands,
}

