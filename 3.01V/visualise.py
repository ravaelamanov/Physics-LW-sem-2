import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np

fig = plt.figure()
ax = fig.add_subplot()

f = open("E:\\ifmo\\1 course\\physics\\labs done\\lab 3.01v\\lab\\cmake-build-debug\\out_3_6.txt", "r")
lines = f.readlines()
f.close()
x = []
y = []
z = []
for line in lines:
    nums = line.split()
    x.append(float(nums[0]))
    y.append(float(nums[1])
    z.append(float(nums[2]))

cmap = mcolors.LinearSegmentedColormap.from_list("", ["#00008b", "blue", "cyan", "violet", "red", "#8b0000"])
plt.scatter(x, y, s=25, c=z, cmap=cmap, marker='^')
plt.colorbar()

ax.set_xlabel('X')
ax.set_ylabel('Y')

plt.xticks(np.arange(-8, 9, 1))
plt.yticks(np.arange(-5, 6, 1))
plt.show()
