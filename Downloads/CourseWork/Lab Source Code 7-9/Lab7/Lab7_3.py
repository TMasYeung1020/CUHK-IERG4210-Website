# Lab7_3.py, ICP, 2021

# Use main() function approach
def main(): # define the main() function
    message = input("Enter a string: ")
    print(f" First five characters are <{message[0:5]}>")
    end = len(message)
    text = message[end::-1]
    print(f" Reverse String is <{text}>")
    
main()  # start and run the program HERE