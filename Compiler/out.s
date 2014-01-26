org 100h
JMP start
sum DD 0
n DD 0

start:
MOV bx , 20
MOV sum,bx
MOV bx , 5
MOV n,bx
MOV bx , n
MOV cx , 5
CMP bx , cx
JNZ Label1
MOV bx , sum
MOV cx , 0
CMP bx , cx
JS Label2

Label3:
MOV bx , n
MOV cx , 0
CMP bx , cx
JS Label4
MOV bx , n
MOV cx , 2
SUB bx , cx
MOV n,bx
MOV bx , sum
MOV cx , 2
SUB bx , cx
MOV cx , 2
MOV ax , bx
MOV dx , 0
DIV cx
MOV bx , ax
MOV sum,bx
JMP Label3

Label4:

Label2:

Label1:

RET
