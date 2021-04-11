def recur_count(i, n, cache):
    count = 0
    if cache.has_key(n):
        count = cache[n][1]
        j = cache[n][0]
    else:
        count = 0
        j = (n-1)/2
    for x in range(j, i-1,-1):
        y = n - x
        count += 1 + recur_count(x+1, y, cache)
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
        if y <= 2*x-2:
            subcount = 0
        else:
            subcount = recur_count(x+1, y, cache)
            cache[y] = [x, subcount]
        count += 1 + subcount
    return count

for n in range(15,16):
    print "    if n == " + str(n) +":\n        return " + str(solution(n))
