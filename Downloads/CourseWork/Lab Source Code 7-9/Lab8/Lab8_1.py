# Lab8_1.py, ICP, 2021
# Handling circles, using procedural approach (functions)

import math  # import the math module

def circumference(radius): # a function
    return 2*math.pi*radius

def area(radius):  # a function to find area
    return math.pi*radius*radius

def main(): # the main function
    radius = float(input("Enter a radius: "))
    print(f"Area of circle is {area(radius):.3f}")
    print(f"Circumference of circle is {circumference(radius):.3f}")
    
main()  # start execution