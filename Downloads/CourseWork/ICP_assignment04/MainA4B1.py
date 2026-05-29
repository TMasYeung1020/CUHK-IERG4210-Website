# MainA4B1.py, ICP, 2021, for basic running and testing. 
# * DO NOT modify this given test file.
# Main Testing Program

from A4B1 import Runner

def main():
    print("--- MainA4B1: Testing Runner Class, OOP ---")
    r1 = Runner(12341234, "AU A.A.", 1.2341234)
    print(r1)  
    if r1.isPro:
        print(f"{r1.rName} runs VERY fast!")
    else:
        print(f"{r1.rName} is NOT a Pro.")

        
if __name__ == "__main__": main()
