org 100h
JMP start
sum DD 0
n DD 0

start:
MOV bx , 20
MOV sum,bx
MOV bx , 4
MOV n,bx

Label1:
MOV bx , n
PUSH bx
MOV bx , 0
POP cx
CMP cx , bx
JNA Label2

Label3:
MOV bx , sum
PUSH bx
MOV bx , 0
POP cx
CMP cx , bx
JNA Label4
MOV bx , n
PUSH bx
MOV bx , 2
POP cx
MOV ax , cx
MOV dx , 0
DIV bx
MOV cx , ax
MOV sum,cx
MOV bx , n
PUSH bx
MOV bx , 1
POP cx
SUB cx , bx
MOV n,cx
JMP Label3

Label4:
MOV bx , sum
PUSH bx
MOV bx , 0
POP cx
CMP cx , bx
JNA Label5
MOV bx , n
PUSH bx
MOV bx , 1
POP cx
SUB cx , bx
MOV n,cx

Label5:
JMP Label1

Label2:

RET
