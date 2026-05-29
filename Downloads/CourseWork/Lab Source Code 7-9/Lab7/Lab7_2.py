# Lab7_2.py, Lab 7, ICP, 2021
# Use main() function approach

def main(): # define the main() function
    index = 0
    myList2 = [0] * 6
    print("Enter 6 integers for the myList2:")
    while index < 6:
        print("Enter #", index + 1,": ", end = "")
        myList2[index] = int(input())
        index +=1       
    print("The values stored in myList2 are:")
    for value in myList2:
        print(value, end =" ")

main()  # start and run the program HERE