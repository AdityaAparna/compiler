org 100h
JMP start
sum DD 0
n DD 0

start:
MOV bx , 0
MOV sum,bx
MOV bx , 5
MOV n,bx

Label1:
MOV bx , n
MOV cx , 0
CMP bx , cx
JS Label2
MOV bx , sum
MOV cx , n
ADD bx , cx
MOV sum,bx
MOV bx , n
MOV cx , 1
SUB bx , cx
MOV n,bx
JMP Label1

Label2:

RET
