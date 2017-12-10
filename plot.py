import sys

import matplotlib.pyplot as plt

lms, corr = [], []

for line in sys.stdin:
    a, b = map(float, line.split())
    if a != a:
        continue
    lms.append(a)
    corr.append(b)

plt.title(sys.argv[1].capitalize())
plt.xlabel("number of iterations")
plt.ylabel("f1 score")
plt.plot(lms, label="LMS")
plt.plot(corr, label="Correntropy")
plt.legend()
plt.savefig("img/f1_score_%s.png" % sys.argv[1])
