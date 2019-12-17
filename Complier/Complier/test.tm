*TINY Compilation to TM Code
*File: test.tm
*Standard prelude:
  0:     LD  5,0(0) 	load gp with maxaddress
  1:    LDA  6,0(5) 	copy gp to mp
  2:     ST  0,0(0) 	clear location 0
*End of standard prelude.
*-> Function (cout)
  4:     ST  1,-2(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
  3:    LDC  1,6(0) 	func: load function location
  6:     ST  0,-1(6) 	func: store return address
*->compound
*-> Call
*-> Const
  7:    LDC  0,1(0) 	load const
*<- Const
  8:     ST  0,-4(6) 	call: push argument
  9:     LD  0,-4(6) 	load arg to ac
 10:     OUT   0,0,0	write ac
*<- Call
*<-compound
 11:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,6(7)	func: unconditional jump to next declaration
*-> Function (cout)
*-> Function (main)
 13:     ST  1,-3(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
 12:    LDC  1,15(0) 	func: load function location
 15:     ST  0,-1(6) 	func: store return address
*->compound
*-> assign
*-> Id (x)
 16:    LDC  0,-2(0) 	id: load varOffset
 17:     ADD   0,6,0	id: calculate the address
 18:    LDA  0,0(0) 	load id address
*<- Id
 19:     ST  0,-4(6) 	assign: push left (address)
*-> Call
 20:      IN   0,0,0	read integer value
*<- Call
 21:     LD  1,-4(6) 	assign: load left (address)
 22:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (y)
 23:    LDC  0,-3(0) 	id: load varOffset
 24:     ADD   0,6,0	id: calculate the address
 25:    LDA  0,0(0) 	load id address
*<- Id
 26:     ST  0,-4(6) 	assign: push left (address)
*-> Call
 27:      IN   0,0,0	read integer value
*<- Call
 28:     LD  1,-4(6) 	assign: load left (address)
 29:     ST  0,0(1) 	assign: store value
*<- assign
*-> Call
 30:     ST  6,-4(6) 	call: store current mp
 31:    LDA  6,-4(6) 	call: push new frame
 32:    LDA  0,1(7) 	call: save return in ac
 33:     LD  7,-2(5) 	call: relative jump to function entry
 34:     LD  6,0(6) 	call: pop current frame
*<- Call
*-> Call
*-> Op
*-> Id (x)
 35:    LDC  0,-2(0) 	id: load varOffset
 36:     ADD   0,6,0	id: calculate the address
 37:     LD  0,0(0) 	load id value
*<- Id
 38:     ST  0,-4(6) 	op: push left
*-> Id (y)
 39:    LDC  0,-3(0) 	id: load varOffset
 40:     ADD   0,6,0	id: calculate the address
 41:     LD  0,0(0) 	load id value
*<- Id
 42:     LD  1,-4(6) 	op: load left
 43:     ADD   0,1,0	op +
*<- Op
 44:     ST  0,-6(6) 	call: push argument
 45:     LD  0,-6(6) 	load arg to ac
 46:     OUT   0,0,0	write ac
*<- Call
*<-compound
 47:     LD  7,-1(6) 	func: load pc with return address
 14:     LDA 7,33(7)	func: unconditional jump to next declaration
*-> Function (main)
 48:    LDC  0,-2(0) 	init: load globalOffset
 49:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 50:     ST  6,0(6) 	call: store current mp
 51:    LDA  6,0(6) 	call: push new frame
 52:    LDA  0,1(7) 	call: save return in ac
 53:    LDC  7,15(0) 	call: unconditional jump to main() entry
 54:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 55:    HALT   0,0,0	
