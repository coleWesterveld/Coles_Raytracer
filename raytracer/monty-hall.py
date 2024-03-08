# simulating monty hall problem to test experimentally and learn about graphing and for fun cause im bored
# there are ways to do this that are technically more efficient I just wanted to do this very literally and 
#accurately to real life as possible so that results are indisputable

from random import randint
import matplotlib.pyplot as plt

ITERATIONS = 10_000_000
# randomize a number between 1 and 3 as the door with the car
# randmoize another number as "guess"

# take number that isnt guess and isnt car and allow for a 50/50 guess between the other two
# basically letting the person "switch" or "stay"
# whichever one is picked, determine if new guess is door with the car, and mark it as either success or fail under switch or stay
# display the results as percentage of success with switch or fail

# also, display each time car is at a certain number and guess is a certain number and number of 
# decisions to stay or switch to show no bias in randomization

no_switch = 0
switched = 0
total_stayed = 0
total_switched = 0

car_distro = [0, 0, 0]
guess_distro = [0, 0, 0]

for x in range(ITERATIONS):
    car = randint(1, 3)
    guess = randint(1, 3)
    car_distro[car - 1] += 1
    guess_distro[guess - 1] += 1
    #print("\ncar", car)
    #print("guess", guess)
    # show the other one that is not the car
    # a bit brute forced but im lazy and this isnt for school so stfu
    if (1 != car and 1 != guess):
        options = (2, 3)
    elif (2 != car and 2 != guess):
        options = (1, 3)
    elif (3 != car and 3 != guess):
        options = (1, 2)
    
    switch = randint(0, 1)

    choice = options[switch]

    if (choice == guess):
        stayed = True
        total_stayed += 1
    else:
        stayed = False
        total_switched += 1

    if (choice == car):
        if (stayed == True):
            no_switch += 1
        else:
            switched += 1

labels = ["switched", "no_switch"]

data = [((switched / total_switched) * 100), ((no_switch / total_stayed) * 100)]

print((no_switch / total_stayed) * 100)
print((switched / total_switched) * 100)

# creating the bar plot
plt.bar(labels, data, color = 'blue',
        width = 0.4)
 
plt.xlabel("Decision")
plt.ylabel("Success Rate (%)")
plt.title("Monty Hall Simulated")
plt.show()


# creating the bar plot
plt.bar([1, 2, 3], guess_distro, color = 'red',
        width = 0.4)
 
plt.xlabel("door")
plt.ylabel("guesses")
plt.title("Guess Distro (should be even)")
plt.show()

# creating the bar plot
plt.bar([1, 2, 3], car_distro, color = 'green',
        width = 0.4)
 
plt.xlabel("door")
plt.ylabel("car")
plt.title("Car Distro (should be even)")
plt.show()

print(car_distro)
print(guess_distro)