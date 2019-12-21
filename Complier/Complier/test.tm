*TINY Compilation to TM Code
*File: test.tm
*Standard prelude:
  0:     LD  5,0(0) 	load gp with maxaddress
  1:    LDA  6,-256(5) 	copy gp to mp
  2:     ST  0,0(0) 	clear location 0
*End of standard prelude.
*-> Function (a)
  4:     ST  1,-2(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
  3:    LDC  1,6(0) 	func: load function location
  6:     ST  0,-1(6) 	func: store return address
*-> param
*x
*<- param
*->compound
*-> Call
*-> Id (x)
  7:    LDC  0,-2(0) 	id: load varOffset
  8:     ADD   0,6,0	id: calculate the address found in current frame
  9:    LDA  0,0(0) 	load id address
*<- Id
 10:     ST  0,-5(6) 	call: push argument
 11:     LD  0,-5(6) 	load arg to ac
 12:     OUT   0,0,0	write ac
*<- Call
*<-compound
 13:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,8(7)	func: unconditional jump to next declaration
* frameoffset (-3) 
*-> Function (main)
 15:     ST  1,-3(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
 14:    LDC  1,17(0) 	func: load function location
 17:     ST  0,-1(6) 	func: store return address
*->compound
*-> assign
*-> Id (z)
 18:    LDC  0,-3(0) 	id: load varOffset
 19:     ADD   0,6,0	id: calculate the address found in current frame
 20:    LDA  0,0(0) 	load id address
*<- Id
 21:     ST  0,-6(6) 	assign: push left (address)
*-> Const
 22:    LDC  0,0(0) 	load const
*<- Const
 23:     LD  1,-6(6) 	assign: load left (address)
 24:     ST  0,0(1) 	assign: store value
*<- assign
*-> iter.
*while: jump after body comes back here
*-> Op
*-> Id (z)
 25:    LDC  0,-3(0) 	id: load varOffset
 26:     ADD   0,6,0	id: calculate the address found in current frame
 27:     LD  0,0(0) 	load id value
*<- Id
 28:     ST  0,-6(6) 	op: push left
*-> Const
 29:    LDC  0,3(0) 	load const
*<- Const
 30:     LD  1,-6(6) 	op: load left
 31:     SUB   0,1,0	op <
 32:    JLT  0,2(7) 	br if true
 33:    LDC  0,0(0) 	false case
 34:    LDA  7,1(7) 	unconditional jmp
 35:    LDC  0,1(0) 	true case
*<- Op
*while: jump to end belongs here
*->compound
*-> assign
*-> Id (z)
 37:    LDC  0,-3(0) 	id: load varOffset
 38:     ADD   0,6,0	id: calculate the address found in current frame
 39:    LDA  0,0(0) 	load id address
*<- Id
 40:     ST  0,-6(6) 	assign: push left (address)
*-> Op
*-> Id (z)
 41:    LDC  0,-3(0) 	id: load varOffset
 42:     ADD   0,6,0	id: calculate the address found in current frame
 43:     LD  0,0(0) 	load id value
*<- Id
 44:     ST  0,-7(6) 	op: push left
*-> Const
 45:    LDC  0,1(0) 	load const
*<- Const
 46:     LD  1,-7(6) 	op: load left
 47:     ADD   0,1,0	op +
*<- Op
 48:     LD  1,-6(6) 	assign: load left (address)
 49:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (x)
 50:    LDC  0,-2(0) 	id: load varOffset
 51:     ADD   0,6,0	id: calculate the address
 52:     ST  0,-6(6) 	id: push base address
 53:     LD  1,-6(6) 	id: pop base address
 54:     SUB   0,1,0	id: calculate element address with index
 55:    LDA  0,0(0) 	load id address
*<- Id
 56:     ST  0,-6(6) 	assign: push left (address)
*-> Call
 57:      IN   0,0,0	read integer value
*<- Call
 58:     LD  1,-6(6) 	assign: load left (address)
 59:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
 60:     LDA 7,-36(7)	while: jmp back to test
 36:     JEQ 0,24(7)	while: jmp to end
*<- iter.
*-> Call
*-> Id (x)
 61:    LDC  0,-2(0) 	id: load varOffset
 62:     ADD   0,6,0	id: calculate the address found in current frame
 63:    LDA  0,0(0) 	load id address
*<- Id
 64:     ST  0,-8(6) 	call: push argument
 65:     ST  6,-6(6) 	call: store current mp
 66:    LDA  6,-6(6) 	call: push new frame
 67:    LDA  0,1(7) 	call: save return in ac
 68:     LD  7,-2(5) 	call: relative jump to function entry
 69:     LD  6,0(6) 	call: pop current frame
*<- Call
*<-compound
 70:     LD  7,-1(6) 	func: load pc with return address
 16:     LDA 7,54(7)	func: unconditional jump to next declaration
* frameoffset (-10) 
 71:    LDC  0,-2(0) 	init: load globalOffset
 72:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 73:     ST  6,0(6) 	call: store current mp
 74:    LDA  6,0(6) 	call: push new frame
 75:    LDA  0,1(7) 	call: save return in ac
 76:    LDC  7,17(0) 	call: unconditional jump to main() entry
 77:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 78:    HALT   0,0,0	
