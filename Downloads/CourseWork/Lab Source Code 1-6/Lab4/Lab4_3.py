# lab4_3.py, ICP, 2021

gNum = float(input('Enter the student\'s numerical grade: '))
if 100.0 < gNum or gNum < 0.0:
    gLetter = "U"
elif gNum >=90.0:
    gLetter = "A"
elif gNum >=80.0:
    gLetter = "B"
elif gNum >=70.0:
    gLetter = "C"
elif gNum >=60.0:
    gLetter = "D"
elif gNum >=50.0:
    gLetter = "E"
else:              # elif gNum >=0.0: # 
    gLetter = "F"

print("  The student receives a grade of ", gLetter)

#print("\n........ By MyName; ICP 4020; 2021 ........\n")
