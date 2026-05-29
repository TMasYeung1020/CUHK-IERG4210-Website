#Lab10_1.py, ICP, 2021

import random

def game():
    Set = set()
    print("Random picking starts...")
    while len(Set) < 6:
        ranNum = random.randint(1,49)
        Set.add(ranNum)
        print(ranNum)
    print(f"The final set of Mark 6 is: {Set}")

game()


