import matplotlib.pyplot as plt

import sys

lms, corr = [], []

for line in sys.stdin:
    a, b = map(float, line.split())
    lms.append(a)
    corr.append(b)

plt.title("f1 score")
plt.xlabel("number of iterations")
plt.ylabel("f1 score")
plt.plot(lms, label="Least Mean Squares")
plt.plot(corr, label="Correlation as cost function")
plt.legend()
plt.savefig('f1_score_highway.png')