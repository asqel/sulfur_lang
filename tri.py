from random import randint
from time import time

def new_list():
    return [randint(0,100)for i in range(100)]


def bul(l):#bulle
    for i in range(len(l)):
        for k in range(i,len(l)):
            if l[i]>l[k]:
                l[k],l[i]=l[i],l[k]
    return l


def sel(l):#selection
    for i in range(len(l)):
        p=float("inf")
        idx=-1
        for k in range(i,len(l)):
            if p>l[k]:
                p=l[k]
                idx=k
        l[i],l[idx]=l[idx],l[i]
    return l
            
def ins(l):#inserrtion
    for i in range(len(l)):
        for k in range(i,-1,-1):
            if l[i]<l[k]:
                l[i],l[k]=l[k],l[i]
                i-=1
    return l


def tri_fu(l:list):
    if len(l)<2:
        return l
    m=int(len(l)/2)
    l1=tri_fu(l[:m])
    l2=tri_fu(l[m:])
    
    le1=len(l1)
    le2=len(l2)
    if 0 in [le1,le2]:
        return le1 or le2
    r=[]
    i=0
    k=0
    while(len(r)<le1+le2):
        if l1[i]>l2[k]:
            r.append(l2[k])
            k+=1
        else:
            r.append(l1[i])
            i+=1
        if i==le1 or k==le2:
            r.extend(l1[i:] or l2[k:])
            break
    return r
    
    

def rapide(l:list):
    le=len(l)
    if not l:
        return list()
    p=l[0]
    if len(l)==1:
        return l
    t,g=[],[]
    for i in range(1,le):
        g.append(l[i]) if l[i]>p else t.append(l[i])
    return rapide(t)+[p]+rapide(g)

            
        

l=new_list()
t0=time()
n=tri_fu(l.copy())
print(n)
t1=time()
print(t1-t0)
l.sort()
print(n==l)



def gr(l:list):
    k=0
    for i in l:
        k=max(list(i)+[k])
    k+=1
    g=[[0 for i in range(k)] for k in range(k)]
    for i in l:
        g[i[1]][i[0]]=-1
    for y in range(k-1):
        for x in range(k-1):
            if( g[y][x]!=-1):
                g[y][x]=get_bomb(g,x,y)
    return g
            
def get_bomb(g,x,y):
    n=0
    for i in range(-1,2):
        for k in range(-1,2):
            if g[i+y][k+x]==-1:
                n+=1
    return n
    
print(gr([(1,2),(3,4)]))