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

local definition_define_new_parameter_report = {
	name = "Defining new diagnostic parameter reports",
	description = "Part of the Diagnostic Data Reporting Service",
	header = {
		service_type = 3,
		service_subtype = 2,
	},
	structure = {
		{ name="sid", type="editable", length=16 },
		{ name="collection_interface", type="editable", length=32, value=0 },
		{ name="npar1", type="editable", length=16, group={
				{ name="parameter", type="editable", length=16 },
			}
		},
		{ name="nfa", type="editable", length=16, value=0, group={
				{ name="nrep", type="editable", length=16 },
				{ name="npar2", type="editable", length=16, group={
						{ name="parameter", type="editable", length=16 },
					}
				},
			},
		},
	}
}

local definition_clear_parameter_report_definitions = {
	name = "Clear diagnostic parameter reports definitions",
	description = "Part of the Diagnostic Data Reporting Service",
	header = {
		service_type = 3,
		service_subtype = 4,
	},
	structure = {
		{ name="nsid", type="editable", length=16, group={
				{ name="sid", type="editable", length=16 },
			}
		},
	}
}

local definition_enable_parameter_report_generation = {
	name = "Enable diagnostic parameter report generation",
	description = "Part of the Diagnostic Data Reporting Service",
	header = {
		service_type = 3,
		service_subtype = 7,
	},
	structure = {
		{ name="nsid", type="editable", length=16, group={
				{ name="sid", type="editable", length=16 },
			}
		},
	}
}

local definition_disable_parameter_report_generation = {
	name = "Disable diagnostic parameter report generation",
	description = "Part of the Diagnostic Data Reporting Service",
	header = {
		service_type = 3,
		service_subtype = 8,
	},
	structure = {
		{ name="nsid", type="editable", length=16, group={
				{ name="sid", type="editable", length=16 },
			}
		},
	}
}

return {
	_VERSION = '0.1',
	_DESCRIPTION = [[ TODO ]],
	_LICENSE = [[2-clause BSD License]],
	
	define_new_parameter_report = definition_define_new_parameter_report,
	clear_parameter_report_definitions = definition_clear_parameter_report_definitions,
	enable_parameter_report_generation = definition_enable_parameter_report_generation,
	disable_parameter_report_generation = definition_disable_parameter_report_generation,
}

