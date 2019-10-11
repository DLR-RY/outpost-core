import json
import os

with open("compile_commands.json") as jfile:
    jdat = json.load(jfile)
    for p in jdat:
        absfile = (os.path.abspath(p['directory'] + "/" + p['file']))
        p['file'] = absfile

    with open("c.json","w") as ofile:
        json.dump(jdat, ofile, indent=4)

