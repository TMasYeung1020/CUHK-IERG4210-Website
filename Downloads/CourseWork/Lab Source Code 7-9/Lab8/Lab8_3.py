# Lab8_3.py, ICP, 2021
# Define ICPStudent Class, using OO approach (& Class and Objects)

class ICPStudent: # define Student class
    def __init__(self, sid, sname, smarks=[0]*4): # constructor
        self.sID = sid       # define and set a field 
        self.sName = sname
        self.sMarks = smarks

    def avgMark(self):
        return sum(self.sMarks)/4
    
    def __str__(self): # method __str__(), printable string info.
        return f"SID:{self.sID}, Name:{self.sName}, average ICP mark:{self.avgMark()}"

    
def main(): # the main function
    me = ICPStudent(20214020, "CHAN Siu Ming", [100,90,99,100])
    myFriend = ICPStudent(20214321, "CHAN Tai Man")
    print(me)
    print(myFriend)
    
main()  # start execution