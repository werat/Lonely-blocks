import itertools as it, glob
import os
import re

name = "werat"

def multiple_file_types(*patterns):
    return it.chain.from_iterable(glob.glob(pattern) for pattern in patterns)

for file in multiple_file_types("*.cc", "*.h"):
    f = open(file, "r")
    lines = list(f.readlines())
    f.close()
    match = re.match("// @(.*)", lines[0])
    if match:
        print file, "is already marked by", match.group(1)
    else:
        print "***", file, "has been marked by", name, "***"
        with open(file, "w") as f:
            lines.insert(0, "\n")
            lines.insert(0, "\n")
            lines.insert(0, "// @" + name)
            f.writelines(lines)


