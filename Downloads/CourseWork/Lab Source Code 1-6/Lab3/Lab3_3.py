# lab3_3.py, ICP, 2021

import math
print("Enter 2 points (x1, y1) and (x2, y2) for calculating their distance:")
x1 = float(input('  Enter x1: '))
y1 = float(input('  Enter y1: '))
x2 = float(input('  Enter x2: '))
y2 = float(input('  Enter y2: '))
distance = math.sqrt((x1-x2)**2 + (y1-y2)**2)
print(f"The distance between two points is {distance:.6f}")

print("\n........ By YourName; ICP 4020; 2021 ........\n")
