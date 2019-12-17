*TINY Compilation to TM Code
*File: test.tm
*Standard prelude:
  0:     LD  5,0(0) 	load gp with maxaddress
  1:    LDA  6,0(5) 	copy gp to mp
  2:     ST  0,0(0) 	clear location 0
*End of standard prelude.
*-> Function (main)
  4:     ST  1,-2(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
  3:    LDC  1,6(0) 	func: load function location
  6:     ST  0,-1(6) 	func: store return address
*->compound
*-> assign
*-> Id (x)
  7:    LDC  0,-2(0) 	id: load varOffset
  8:     ADD   0,6,0	id: calculate the address
  9:    LDA  0,0(0) 	load id address
*<- Id
 10:     ST  0,-4(6) 	assign: push left (address)
*-> Call
 11:      IN   0,0,0	read integer value
*<- Call
 12:     LD  1,-4(6) 	assign: load left (address)
 13:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (y)
 14:    LDC  0,-3(0) 	id: load varOffset
 15:     ADD   0,6,0	id: calculate the address
 16:    LDA  0,0(0) 	load id address
*<- Id
 17:     ST  0,-4(6) 	assign: push left (address)
*-> Call
 18:      IN   0,0,0	read integer value
*<- Call
 19:     LD  1,-4(6) 	assign: load left (address)
 20:     ST  0,0(1) 	assign: store value
*<- assign
*-> Call
*-> Op
*-> Id (x)
 21:    LDC  0,-2(0) 	id: load varOffset
 22:     ADD   0,6,0	id: calculate the address
 23:     LD  0,0(0) 	load id value
*<- Id
 24:     ST  0,-4(6) 	op: push left
*-> Id (y)
 25:    LDC  0,-3(0) 	id: load varOffset
 26:     ADD   0,6,0	id: calculate the address
 27:     LD  0,0(0) 	load id value
*<- Id
 28:     LD  1,-4(6) 	op: load left
 29:     ADD   0,1,0	op +
*<- Op
 30:     ST  0,-6(6) 	call: push argument
 31:     LD  0,-6(6) 	load arg to ac
 32:     OUT   0,0,0	write ac
*<- Call
*<-compound
 33:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,28(7)	func: unconditional jump to next declaration
*-> Function (main)
 34:    LDC  0,-1(0) 	init: load globalOffset
 35:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 36:     ST  6,0(6) 	call: store current mp
 37:    LDA  6,0(6) 	call: push new frame
 38:    LDA  0,1(7) 	call: save return in ac
 39:    LDC  7,6(0) 	call: unconditional jump to main() entry
 40:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 41:    HALT   0,0,0	
