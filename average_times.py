import sys
import numpy

data = {}

for line in sys.stdin:
    name, val = line.split()
    l = data.get(name, [])
    l.append(float(val))
    data[name] = l

for k, v in data.items():
    print(k, numpy.mean(v), numpy.std(v))
