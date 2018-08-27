#!/usr/bin/env python


analisys = 'we detected 3 person|personss \
we detected 6 chair|chairss \
we detected 5 tv|tvs\
we detected one laptop|laptops\
we detected one mouse|mice\
we detected one refrigerator|refrigerators'




# clean string
str_arr = analisys.splitlines()

temp = ''
str_arr
for line in str_arr:
    #print(line)
    head, s, tail = line.partition('|')    
    temp = temp + head + ' ' 

''''
while tail is not '':
    temp = temp + head
    print(tail)
    #head, s, tail = tail.partition('\n')
    #print(tail)
    head, s, tail = tail.partition('|')
    print(head)
'''

print('*********************************')
print(temp)   
    

