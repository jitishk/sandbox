
def print_tiers(tiers):
    for n in range(1,num_n):
        print  tiers[n], "n:", n

def solution(n):
    num_tiers = 30
    num_n = 201
    tiers = [0]*num_n
    for i in range(1,num_n):
        tiers[i] = [0]*num_tiers

    # num of combinations in the first tier directly depends on the # of
    # bricks.
    c = 0
    for i in range(1, num_n):
        tiers[i][c] = (i-1)/2

    base_skip = 2
    skip = 3  
    c += 1
    for count in range(1,30):
        for i in range(base_skip+skip, num_n):
            tiers[i][c] = tiers[i-skip][c-1]  + tiers[i-skip][c]
        base_skip += skip
        skip += 1
        c += 1

    return sum(tiers[n])    

# print_tiers(tiers)

n = 170
print "n:", n, "value:", solution(n)
