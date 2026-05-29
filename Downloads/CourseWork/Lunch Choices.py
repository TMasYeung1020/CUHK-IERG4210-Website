#What to eat?

import time
import random
import os

os.system("六合彩BGM.mp3") 

restaurant = ["chinese", "Pizza", "Japanese", "Korean", "Thai",]

for x in range(20):
    time.sleep(0.2)
    print(restaurant[random.randint(0, (len(restaurant) - 1))])

final = random.randint(0, (len(restaurant) - 1))
print("Final Decision:", restaurant[final])

exit()

