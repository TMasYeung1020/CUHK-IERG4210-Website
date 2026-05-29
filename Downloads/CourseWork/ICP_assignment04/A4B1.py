# A4B1.py, ICP, 2021 
# FINISHED by Yeung Man, 20189102, CL13

class Runner: 
    def __init__(self, rid, rname, rpbt):
        self.rID = rid
        self.rName = rname
        self.rPBT = rpbt

    def setPBT(tList):
        self.rPBT = min(self.tList)

    def isPro():
        for record in rPBT:
            if rPBT < 4:
                print(True)
            else:
                print(False)
            return record
        
    def __str__(self):
        return(f"Runner:<{self.rID}>, Name:<{self.rName}>, Best Time:<{round(self.rPBT,2)}>")

def main():
    print("-- A4, Self Testing for class Runner --")
    b = Runner(20090816, "Bolt", 1.5966)
    Me = Runner(20189102, "Yeung Man", 6.789)
    print(b)
    print(Me)

if __name__ == "__main__": main()            
