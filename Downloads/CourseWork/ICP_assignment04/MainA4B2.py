# MainA4B2.py, ICP, 2021, for basic running and testing. 
# * DO NOT modify this given test file.
# Main Testing Program

from A4B2 import *

def main():
    print("--- MainA4B2: Testing File I/O---")
    rFName = "a4b2IN.txt"
    rList = readRunnerFile(rFName)
    print(f">1. Data read from file {rFName} BELOW:")
    print(rList)
    print(f" .. Average PBT is {avgPBT(rList):.3f}")
     
    appendRunnerRecord(rList, 20090816, "Bolt", 1.5966)
    print(f"\n>>2. UPDATED Data, after appendRunnerRecord() BELOW:")
    print(rList)

    wFName = "a4b2OUT.txt"
    r1 = '20214020,CHAN Siu Ming,8.50\n'
    rList.append(r1)
    writeRunnerFile(wFName, rList)
    rList = readRunnerFile(wFName)
    print(f"\n>>>3. Data read from WRITTEN file {wFName}, after append() BELOW:")
    print(rList)
    
    
if __name__ == "__main__": main()
