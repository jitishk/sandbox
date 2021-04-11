

def recur_count(i, n, cache, tabspace):
    if (n-i <= i):
        return 0
    count = 0
    if cache[n] != 0 and cache[n][0]>=i:
        count = cache[n][1]
        j = cache[n][0]
        #print tabspace, "i:", i, "n:", n, "cache:", cache[n]
    else:
        count = 0
        j = (n-1)/2
        # print tabspace, "i:", i, "n:", n, "j:", j, "count:", count
    x = j
    #for x in range(j, i-1, -1):
    while x > i-1:
        y = n - x
        #print tabspace, x, "+", y
        subcount = recur_count(x+1, y, cache, tabspace+"  ")
        if subcount>0 and cache[y] == 0:
            cache[y] = [x, subcount]
        count += 1 + subcount
        x -= 1
    return count

def solution (n):
    count = 0
    i = 1
    newcache = [0] * n
    for x in range((n-1)/2, i-1, -1):
        y = n - x
        #print x, "+", y
        subcount = recur_count(x+1, y, newcache, "  ") 
        if subcount>0:	
            newcache[y] = [x, subcount]
        count += 1 + subcount
    return count
ans = [0]*20
diff = [0]*20
diff2 = [0]*20
diff3 = [0]*20
i = 0
for n in range(19,20):
    ans[i] = solution(n)
    if i>0:
        diff[i] = ans[i] - ans[i-1]
        diff2[i] = diff[i] - diff[i-1]
        diff3[i] = diff2[i] - diff2[i-1]
    i += 1
print ans
print diff
print diff2
print diff3
# for n in range(200,201):
#     print "    if n == " + str(n) +":\n        return " + str(solution(n))
# 

