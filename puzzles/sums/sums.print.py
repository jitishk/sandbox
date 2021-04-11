

def recur_count(i, n, cache, tabspace, tier, tiers):
    if (n-i <= i):
        return 0
    count = 0
    j = (n-1)/2
    # print tabspace, "i:", i, "n:", n, "j:", j, "count:", count
    for x in range(j, i-1, -1):
        y = n - x
        # print tabspace, x, "+", y
        tiers[tier] += 1
        subcount = recur_count(x+1, y, cache, tabspace+"  ", tier+1, tiers)
        count += 1 + subcount
    return count

def solution (n):
    count = 0
    i = 1
    cache = {}
    tiers = [0]*30
    tier = 0
    for x in range((n-1)/2, i-1, -1):
        y = n - x
        # print x, "+", y
        tiers[tier] += 1
        subcount = recur_count(x+1, y, cache, "  ", tier+1, tiers) 
        if subcount>0:	
            cache[y] = [x, subcount]
        count += 1 + subcount
    for t in tiers:
        t_str = str(t)
    print "tiers ", tiers, "n:", n, "count:", count 
    return count

for n in range(1,201):
    solution(n)
   #  print "    if n == " + str(n) +":\n        return " + str(solution(n))

