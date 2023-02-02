from random import randint
from time import time

def new_list():
    return [randint(0,100)for i in range(1_000)]


def bul(l):
    for i in range(len(l)):
        for k in range(i,len(l)):
            if l[i]>l[k]:
                l[k],l[i]=l[i],l[k]
    return l


def sel(l):
    for i in range(len(l)):
        p=float("inf")
        idx=-1
        for k in range(i,len(l)):
            if p>l[k]:
                p=l[k]
                idx=k
        l[i],l[idx]=l[idx],l[i]
    return l
            
def ins(l):
    for i in range(len(l)):
        for k in range(i,-1,-1):
            if l[i]<l[k]:
                l[i],l[k]=l[k],l[i]
                i-=1
    return l
            
            

l=new_list()
t0=time()
n=ins(l.copy())
print(n)
t1=time()
print(t1-t0)
l.sort()
print(n==l)