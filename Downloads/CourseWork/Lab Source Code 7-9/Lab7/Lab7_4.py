# Lab7_4.py, ICP, 2021
# Use main() function approach

def dispStrWord(msg):
    print("One word per line is:")
    i = 0
    for ch in msg:
        if ch == " ":
            print()
        else:
            print(ch, end="")

def main():
    message = input("Enter a string: ")
    dispStrWord(message)
    
main()

    
