org 100h
JMP start
sum DD 0
n DD 0

start:
MOV bx , 20
MOV sum,bx
MOV bx , 5
MOV n,bx

Label1:
MOV bx , n
MOV cx , 0
CMP bx , cx
JNA Label2

Label3:
MOV bx , sum
MOV cx , 0
CMP bx , cx
JNA Label4
MOV bx , n
MOV cx , 2
MOV ax , bx
MOV dx , 0
DIV cx
MOV bx , ax
MOV sum,bx
MOV bx , n
MOV cx , 1
SUB bx , cx
MOV n,bx
JMP Label3

Label4:
MOV bx , sum
MOV cx , 0
CMP bx , cx
JNA Label5
MOV bx , n
MOV cx , 2
SUB bx , cx
MOV n,bx

Label5:
JMP Label1

Label2:

RET
