# Lab8_2.py, ICP, 2021
# Handling circles, using OO approach (& Class and Objects)

import math  # import the math module

class Circle: # define the class of Circle
  def __init__(self, inRadius=0.0): # constructor
    self.radius = inRadius # a field / instance variable
    
  def circumference(self): # a method
    return 2*math.pi*self.radius
    
  def area(self):  # a method to find area
    return math.pi*self.radius*self.radius

    
def main(): # the main function
  radius = float(input("Enter a radius of Circle Object: "))
  aCircle = Circle(radius) # create/instantiate a new object
  print(f"Area of circle is {aCircle.area():.3f}")
  print(f"Circumference of circle is {aCircle.circumference():.3f}")
    
main()  # start execution