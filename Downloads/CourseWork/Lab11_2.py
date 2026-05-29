#Lab11_2.py, ICP 2021

def dispRecur(aList):
    while len(aList) != 0:
        print(aList)
        del(aList[0])

def main():
    dispRecur( [20214020, "CHAN Siu Ming", 100, 90, 99, 100] )
    print()
    dispRecur( list("**********") )

main()
    
