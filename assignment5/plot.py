from matplotlib import pyplot as plt
import sys
with open('rtt.txt') as f:
    lines = f.readlines()
x = []
y = []
for i in enumerate(lines):
	x.append(i[0])
	y.append(float(i[1].replace('\n','')))
#print(sys.argv)
average = sum(y)/len(y)
print("Average is: ",average)
plt.plot(x,y)
plt.xlabel('Packet no')
plt.ylabel('Fetch Time')
plt.savefig('RTT.png')
plt.show()
