

def recur_count(i, n, cache, tabspace):
    if (n-i <= i):
        return 0
    count = 0
    if cache.has_key(n):
        count = cache[n][1]
        j = cache[n][0]
        #print tabspace, "i:", i, "n:", n, "cache:", cache[n]
    else:
        count = 0
        j = (n-1)/2
        # print tabspace, "i:", i, "n:", n, "j:", j, "count:", count
    for x in range(j, i-1, -1):
        y = n - x
        #print tabspace, x, "+", y
        subcount = recur_count(x+1, y, cache, tabspace+"  ")
        count += 1 + subcount
    return count

def solution (n):
    count = 0
    i = 1
    cache = {}
    for x in range((n-1)/2, i-1, -1):
        y = n - x
        #print x, "+", y
        subcount = recur_count(x+1, y, cache, "  ") 
        if subcount>0:	
            cache[y] = [x, subcount]
        count += 1 + subcount
    return count

for n in range(18,19):
    print "    if n == " + str(n) +":\n        return " + str(solution(n))


