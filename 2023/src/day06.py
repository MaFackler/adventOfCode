import math
lines = [l[:-1] for l in open(0).readlines()]

times = list(map(int, [s for s in lines[0].split(":")[1].split(" ") if s]))
distances = list(map(int, [s for s in lines[1].split(":")[1].split(" ") if s]))

s = ""
for t in times:
    s += str(t)

ttime = int(s)
s = ""
for d in distances:
    s += str(d)

tdistance = int(s)

print(ttime)
print(tdistance)
a = -1
b = ttime
c = -tdistance

deter = b * b - 4 * a * c
assert deter > 0
print("a =", a)
print("b =", b)
print("c =", c)
print("b * b:", b * b)
print("4 * a * c:", 4 * a * c)
deter = math.sqrt(deter)
print("deter:", deter)

start = int((-b + deter) / 2.0 * a)
end = int((-b - deter) / 2.0 * a)
print("range:", start, end)
print(end - start)
