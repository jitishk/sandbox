

def inbounds(loc, r, c):
    if loc[0]>=0 and loc[0]<r and loc[1]>=0 and loc[1]<c:
        return True
    return False

def distance_if_removed (loc, src_dist, dest_dist, r, c, curr_dist):
    n1 = [loc[0]-1, loc[1]]
    n2 = [loc[0]+1, loc[1]]
    n3 = [loc[0], loc[1]-1]
    n4 = [loc[0], loc[1]+1]
    if inbounds(n1,r,c) and src_dist[n1[0]][n1[1]]>0:
        if inbounds(n2,r,c)  and dest_dist[n2[0]][n2[1]]>0 and src_dist[n1[0]][n1[1]]+dest_dist[n2[0]][n2[1]]+1<curr_dist:
            curr_dist = src_dist[n1[0]][n1[1]] + dest_dist[n2[0]][n2[1]] + 1
        if inbounds(n3,r,c) and dest_dist[n3[0]][n3[1]]>0 and src_dist[n1[0]][n1[1]]+dest_dist[n3[0]][n3[1]]+1<curr_dist:
            curr_dist = src_dist[n1[0]][n1[1]] + dest_dist[n3[0]][n3[1]] + 1
        if inbounds(n4,r,c) and dest_dist[n4[0]][n4[1]]>0 and src_dist[n1[0]][n1[1]]+dest_dist[n4[0]][n4[1]]+1<curr_dist:
            curr_dist = src_dist[n1[0]][n1[1]] + dest_dist[n4[0]][n4[1]] + 1
    if inbounds(n2,r,c) and src_dist[n2[0]][n2[1]]>0:
        if inbounds(n1,r,c)  and dest_dist[n1[0]][n1[1]]>0 and src_dist[n2[0]][n2[1]]+dest_dist[n1[0]][n1[1]]+1<curr_dist:
            curr_dist = src_dist[n2[0]][n2[1]] + dest_dist[n1[0]][n1[1]] + 1
        if inbounds(n3,r,c) and dest_dist[n3[0]][n3[1]]>0 and src_dist[n2[0]][n2[1]]+dest_dist[n3[0]][n3[1]]+1<curr_dist:
            curr_dist = src_dist[n2[0]][n2[1]] + dest_dist[n3[0]][n3[1]] + 1
        if inbounds(n4,r,c) and dest_dist[n4[0]][n4[1]]>0 and src_dist[n2[0]][n2[1]]+dest_dist[n4[0]][n4[1]]+1<curr_dist:
            curr_dist = src_dist[n2[0]][n2[1]] + dest_dist[n4[0]][n4[1]] + 1
    if inbounds(n3,r,c) and src_dist[n3[0]][n3[1]]>0:
        if inbounds(n1,r,c) and dest_dist[n1[0]][n1[1]]>0 and src_dist[n3[0]][n3[1]]+dest_dist[n1[0]][n1[1]]+1<curr_dist:
            curr_dist = src_dist[n3[0]][n3[1]] + dest_dist[n1[0]][n1[1]] + 1
        if inbounds(n2,r,c)  and dest_dist[n2[0]][n2[1]]>0 and src_dist[n3[0]][n3[1]]+dest_dist[n2[0]][n2[1]]+1<curr_dist:
            curr_dist = src_dist[n3[0]][n3[1]] + dest_dist[n2[0]][n2[1]] + 1
        if inbounds(n4,r,c) and dest_dist[n4[0]][n4[1]]>0 and src_dist[n3[0]][n3[1]]+dest_dist[n4[0]][n4[1]]+1<curr_dist:
            curr_dist = src_dist[n3[0]][n3[1]] + dest_dist[n4[0]][n4[1]] + 1
    if inbounds(n4,r,c) and src_dist[n4[0]][n4[1]]>0:
        if inbounds(n1,r,c) and dest_dist[n1[0]][n1[1]]>0 and src_dist[n4[0]][n4[1]]+dest_dist[n1[0]][n1[1]]+1<curr_dist:
            curr_dist = src_dist[n3[0]][n3[1]] + dest_dist[n1[0]][n1[1]] + 1
        if inbounds(n2,r,c) and dest_dist[n2[0]][n2[1]]>0 and src_dist[n4[0]][n4[1]]+dest_dist[n2[0]][n2[1]]+1<curr_dist:
            curr_dist = src_dist[n3[0]][n3[1]] + dest_dist[n2[0]][n2[1]] + 1
        if inbounds(n3,r,c) and dest_dist[n3[0]][n3[1]]>0 and src_dist[n4[0]][n4[1]]+dest_dist[n3[0]][n3[1]]+1<curr_dist:
            curr_dist = src_dist[n3[0]][n3[1]] + dest_dist[n3[0]][n3[1]] + 1
    return curr_dist

def setdistances(layout):
    r = len(layout);
    c = len(layout[0]);
    min_dist = r + c - 1
    curr_dist = r*c
    buckets = [[0 for col in range(c)] for row in range(r)]
    inque = [[0 for col in range(c)] for row in range(r)]
    que = [[0, 0, 1]];
    while(len(que)):
        loc = que.pop(0)
        buckets[loc[0]][loc[1]] = loc[2]
        newloc = [loc[0]-1, loc[1]]
        if inbounds(newloc,r,c) == True and buckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

        newloc = [loc[0]+1, loc[1]]
        if inbounds(newloc,r,c) == True and buckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

        newloc = [loc[0], loc[1]-1]
        if inbounds(newloc,r,c) == True and buckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

        newloc = [loc[0], loc[1]+1]
        if inbounds(newloc,r,c) == True and buckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

    # optimum path already found. return.
    if buckets[r-1][c-1] == min_dist:
        return min_dist

    if buckets[r-1][c-1] > 0:
        curr_dist = buckets[r-1][c-1]


    # Check reverse path distances.
    reversebuckets = [[0 for col in range(c)] for row in range(r)]
    inque = [[0 for col in range(c)] for row in range(r)]
    inwallque = [[0 for col in range(c)] for row in range(r)]
    que = [[r-1, c-1, 1]];
    wall_que = []
    while(len(que)):
        loc = que.pop(0)
        reversebuckets[loc[0]][loc[1]] = loc[2]
        newloc = [loc[0]-1, loc[1]]
        if inbounds(newloc,r,c) and layout[newloc[0]][newloc[1]] == 1 and inwallque[newloc[0]][newloc[1]] == 0:
            wall_que.append(newloc)
            inwallque[newloc[0]][newloc[1]] = 1
        if inbounds(newloc,r,c) == True and reversebuckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

        newloc = [loc[0]+1, loc[1]]
        if inbounds(newloc,r,c) and layout[newloc[0]][newloc[1]] == 1 and inwallque[newloc[0]][newloc[1]] == 0:
            wall_que.append(newloc)
            inwallque[newloc[0]][newloc[1]] = 1
        if inbounds(newloc,r,c) == True and reversebuckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

        newloc = [loc[0], loc[1]-1]
        if inbounds(newloc,r,c) and layout[newloc[0]][newloc[1]] == 1 and inwallque[newloc[0]][newloc[1]] == 0:
            wall_que.append(newloc)
            inwallque[newloc[0]][newloc[1]] = 1
        if inbounds(newloc,r,c) == True and reversebuckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

        newloc = [loc[0], loc[1]+1]
        if inbounds(newloc,r,c) and layout[newloc[0]][newloc[1]] == 1 and inwallque[newloc[0]][newloc[1]] == 0:
            wall_que.append(newloc)
            inwallque[newloc[0]][newloc[1]] = 1
        if inbounds(newloc,r,c) == True and reversebuckets[newloc[0]][newloc[1]] == 0 and inque[newloc[0]][newloc[1]] == 0 and layout[newloc[0]][newloc[1]] == 0:
            que.append([newloc[0], newloc[1], loc[2]+1])
            inque[newloc[0]][newloc[1]] = 1

    for loc in wall_que:
        curr_dist = distance_if_removed(loc, buckets, reversebuckets, r, c, curr_dist)
    return curr_dist

def solution(layout):
    return setdistances(layout)

layout = [  [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 1, 1, 0, 1, 1, 1], 
            [1, 1, 0, 1, 0, 1, 1, 1],
            [1, 1, 0, 1, 0, 0, 0, 0], 
            [1, 1, 1, 1, 1, 0, 1, 0], 
            [1, 0, 0, 0, 0, 0, 0, 0], 
            [1, 0, 1, 1, 1, 1, 1, 1], 
            [0, 0, 0, 0, 0, 0, 0, 0]]

layout = [  [0, 0, 0, 0, 0, 0],
            [1, 1, 1, 1, 1, 1], 
            [0, 0, 0, 0, 0, 0],
            [0, 1, 1, 1, 1, 1], 
            [0, 1, 1, 1, 1, 1], 
            [0, 0, 0, 0, 0, 0]]

layout = [[0, 0, 0, 0, 0, 0], [1, 1, 1, 1, 1, 0], [0, 0, 0, 0, 0, 0], [0, 1, 1, 1, 1, 1], [0, 1, 1, 1, 1, 1], [0, 0, 0, 0, 0, 0]]
print solution(layout)
