*TINY Compilation to TM Code
*File: test.tm
*Standard prelude:
  0:     LD  5,0(0) 	load gp with maxaddress
  1:    LDA  6,-256(5) 	copy gp to mp
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
  9:     ST  0,-5(6) 	id: push base address
 10:     LD  1,-5(6) 	id: pop base address
 11:     SUB   0,1,0	id: calculate element address with index
 12:    LDA  0,0(0) 	load id address
*<- Id
 13:     ST  0,-5(6) 	assign: push left (address)
*-> Call
 14:      IN   0,0,0	read integer value
*<- Call
 15:     LD  1,-5(6) 	assign: load left (address)
 16:     ST  0,0(1) 	assign: store value
*<- assign
*-> Call
*-> Id (x)
 17:    LDC  0,-2(0) 	id: load varOffset
 18:     ADD   0,6,0	id: calculate the address
 19:     ST  0,-5(6) 	id: push base address
 20:     LD  1,-5(6) 	id: pop base address
 21:     SUB   0,1,0	id: calculate element address with index
 22:     LD  0,0(0) 	load id value
*<- Id
 23:     ST  0,-7(6) 	call: push argument
 24:     LD  0,-7(6) 	load arg to ac
 25:     OUT   0,0,0	write ac
*<- Call
*<-compound
 26:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,21(7)	func: unconditional jump to next declaration
* frameoffset (-8) 
 27:    LDC  0,-1(0) 	init: load globalOffset
 28:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 29:     ST  6,0(6) 	call: store current mp
 30:    LDA  6,0(6) 	call: push new frame
 31:    LDA  0,1(7) 	call: save return in ac
 32:    LDC  7,6(0) 	call: unconditional jump to main() entry
 33:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 34:    HALT   0,0,0	
