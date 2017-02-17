--[[
Copyright (c) 2014-2015, 2017, German Aerospace Center (DLR)

This file is part of the development version of OUTPOST.

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

Authors:
- 2014-2015, 2017, Fabian Greif (DLR RY-AVS)
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
