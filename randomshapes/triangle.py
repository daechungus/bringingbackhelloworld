# Goal: Create a function with an integer input and creates a traingle in the integer n columns, rows only as necessary, matrix with the base at the bottom and goes up accordingly decrementing by 1. inputs are always odd


'''
triangle(2)

.#.
###


triangle(3)

..#..
.###.
#####



'''


def triangle(n):
    for i in range(n):
        print(" "*(n-i-1) + "#"*(2*i+1))
    
triangle(5)