# lab6_1.py, ICP, 2021

# a function, for simple summation and display
def dispSum(name1, num1, num2):
    print("  Hi, " + name1 + "!")
    print(f"  The sum of numbers {num1} and {num2} is {num1+num2}")

# the main function
def main():
    name = input('What is your name? ')
    n1 = int(input('Enter an Integer: '))
    n2 = float(input('Enter a Float number: '))
    dispSum(name, n1, n2)
    #print("\n........ By MyName; ICP 4020; 2021 ........\n")

    
main()  # call the main function
