#Lab11_1.py, ICP 2021

def countIntegers(para1, para2, *paraMore):
    totalInt = 0
    if isinstance(para1, int) == True:
        totalInt += 1
    if isinstance(para2, int) == True:
        totalInt += 1
    for num in paraMore:
        if isinstance(num, int) == True:
            totalInt += 1
    print("Total number of integer arguments is", totalInt)

def main():
    countIntegers(20214321, "CHAN Tai Man")
    countIntegers(20214020, "CHAN Siu Ming", [100,90,99,100])
    countIntegers(20214020, "CHAN Siu Ming", 100, 90, 99, 100)

main()
