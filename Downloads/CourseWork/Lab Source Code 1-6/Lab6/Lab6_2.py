# Lab6_2.py, ICP, 2021

from Lab6_sec import * # import (our own created) module

# the conventional main() function; as the entry point (starting point) of a program
def main():
    hr = int(input('Enter the number of hours: '))
    min = int(input('Enter the number of minutes: '))
    sec = int(input('Enter the number of seconds: '))
    print(" The total number of seconds is", totsecs(hr,min,sec))

    print("\n........ By MyName; ICP 4020; 2021 ........")

# Below start running main()
main()
