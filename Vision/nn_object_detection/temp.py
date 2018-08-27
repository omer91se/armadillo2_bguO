#!/usr/bin/env python



analisys = 'we detected 3 person|personss \
we detected 6 chair|chairss \
we detected 5 tv|tvs\
we detected one laptop|laptops\
we detected one mouse|mice\
we detected one refrigerator|refrigerators'

# clean string
temp = '' 
head, s, tail = analisys.partition('|')
while tail is not '':
    temp = temp + head
    head, s, tail = tail.partition('\n')
    head, s, tail = head.partition('|')

print('*********************************')
print(temp)   
    

