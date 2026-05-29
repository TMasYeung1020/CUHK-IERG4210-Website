# lab5_2.py, ICP, 2021

print("Listing divisible integers between 1 and 500")
num1 = int( input("Enter the first Integer, then <ENTER>: ") )
num2 = int( input("Enter the second Integer, then <ENTER>: ") )

print(f'Integers (1 to 500) divisible by {num1} or {num2} are:')
for elt in range(1, 501):
    if (elt%num1==0 or elt%num2==0):
        print(elt, end=' ')

#print("\n........ By MyName; ICP 4020; 2021 ........\n")

