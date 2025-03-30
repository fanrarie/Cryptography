import random

# 初始扑克牌状态
poker = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]

# 置换群G1，G2，G3
G1 = [10, 12, 1, 5, 2, 6, 13, 3, 8, 9, 7, 11, 4]
G2 = [3, 6, 7, 1, 10, 13, 4, 2, 11, 12, 9, 5, 8]
G3 = [4, 1, 3, 13, 11, 10, 6, 12, 8, 9, 5, 7, 2]

# 洗牌次数（根据学号的最后3位）
r1 = 1
r2 = 1
r3 = 4

# 洗牌函数
def shuffle_deck(deck, permutation):
    shuffled_deck = [None] * len(deck)
    for i in range(len(deck)):
        shuffled_deck[permutation[i] - 1] = deck[i]
    return shuffled_deck

# 执行洗牌操作
for i in range(r1):
    poker = shuffle_deck(poker, G1)
for i in range(r2):
    poker = shuffle_deck(poker, G2)
for i in range(r3):
    poker = shuffle_deck(poker, G3)

# 输出洗牌后的结果
# 遍历数组并输出数字，
for num in poker:
    if num == 1:
        print("A", end=" ")
    elif num == 11:
        print("J", end=" ")
    elif num == 12:
        print("Q", end=" ")
    elif num == 13:
        print("K", end=" ")
    else:
        print(num, end=" ")


# 复原操作
# 逆序洗牌
def reverse_shuffle(deck, permutation):
    reversed_deck = [None] * len(deck)
    for i in range(len(deck)):
        reversed_deck[i] = deck[permutation[i] - 1]
    return reversed_deck

for i in range(r3):
    poker = reverse_shuffle(poker, G3)
for i in range(r2):
    poker = reverse_shuffle(poker, G2)
for i in range(r1):
    poker = reverse_shuffle(poker, G1)

# 输出复原后的结果
print()
for num in poker:
    if num == 1:
        print("A", end=" ")
    elif num == 11:
        print("J", end=" ")
    elif num == 12:
        print("Q", end=" ")
    elif num == 13:
        print("K", end=" ")
    else:
        print(num, end=" ")