# lab4_2.py, ICP, 2021

g = input("What is your gender, m/f? ")
a = int(input("How old are you? "))
if g=="m" and a<=18:
    print ("That's Great!")
elif g=="m" and a>18:
    print ("Working fine.")
elif g=="f" and a<=18:
    print ("Keep Enjoying!")
elif g=="f" and a>18:
    print ("You're doing fine.")
else:
    print ("Have a nice day!")

#print("\n........ By MyName; ICP 4020; 2021 ........\n")
