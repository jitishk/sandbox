def recur_count(i, n, cache):
    if (n-i <= i):
        return 0
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
    for x in range((n-1)/2, i-1, -1):
        y = n - x
        subcount = recur_count(x+1, y, cache)
        if subcount>0:
            cache[y] = [x, subcount]
        count += 1 + subcount
    return count

print solution(20)
