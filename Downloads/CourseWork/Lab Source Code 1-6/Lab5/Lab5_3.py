# lab5_3.py, ICP, 2021

luckyNum = 88

while (True):
    uInt = int(input("Guess our lucky number <between 1 and 100>, then <ENTER>: "))
    if uInt == luckyNum :
        print("  Right! This is our LUCKY NUMBER [",uInt,"]. Well Done... QUIT!")
        break
    elif uInt > luckyNum :
        print("  The number you typed is [", uInt, "] - Too LARGE, guess the number again!")
    else :
        print("  The number you typed is [", uInt, "] - Too SMALL, guess the number again!")


#print("\n........ By MyName; ICP 4020; 2021 ........\n")

