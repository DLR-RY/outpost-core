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

function parseOffset(str)
	if str:sub(-1)=="Z" then
		return 0,0
	end -- ends with Z, Zulu time

	-- matches ±hh:mm, ±hhmm or ±hh; else returns nils 
	local sign, oh, om = str:match("([-+])(%d%d):?(%d?%d?)$") 
	sign, oh, om = sign or "+", oh or "00", om or "00"

	return tonumber(sign .. oh), tonumber(sign .. om)
end

-- Example string 1980-01-06T00:00:00,000+00:00
local function parseDateTime(str)
	local Y,M,D =   parseDate(str)
	local h,m,s =   parseTime(str)
	local oh,om =   parseOffset(str)
	local loh,lom = getLocalUTCOffset()
	
	return os.time({year=Y, month=M, day=D, hour=(h+oh-loh), min=(m+om-lom), sec=s})
end
