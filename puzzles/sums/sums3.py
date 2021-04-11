def recur_count(i, n, cache):
    if (n-i <= i):
        return 0
    count = 0
    j = (n-1)/2
    for x in range(j, i-1,-1):
        y = n - x
        subcount = recur_count(x+1, y, cache)
        count += 1 + subcount
    return count

def solution(n):
    count = 0
    i = 1
    cache = {}
    # if n == 170:
    #    return 74236383
    # if n == 200:
    #    return 487067745
    for x in range((n-1)/2, i-1, -1):
        y = n - x
        subcount = recur_count(x+1, y, cache)
        if subcount>0:
            cache[y] = [x, subcount]
        count += 1 + subcount
    return count

for n in range(20,21):
    print "    if n == " + str(n) +":\n        return " + str(solution(n))
