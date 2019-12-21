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
  8:     ADD   0,6,0	id: calculate the address found in current frame
  9:    LDA  0,0(0) 	load id address
*<- Id
 10:     ST  0,-5(6) 	assign: push left (address)
*-> Call
 11:      IN   0,0,0	read integer value
*<- Call
 12:     LD  1,-5(6) 	assign: load left (address)
 13:     ST  0,0(1) 	assign: store value
*<- assign
*-> Call
*-> Id (x)
 14:    LDC  0,-2(0) 	id: load varOffset
 15:     ADD   0,6,0	id: calculate the address found in current frame
 16:     LD  0,0(0) 	load id value
*<- Id
 17:     ST  0,-7(6) 	call: push argument
 18:     LD  0,-7(6) 	load arg to ac
 19:     OUT   0,0,0	write ac
*<- Call
*<-compound
 20:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,15(7)	func: unconditional jump to next declaration
* frameoffset (-8) 
 21:    LDC  0,-1(0) 	init: load globalOffset
 22:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 23:     ST  6,0(6) 	call: store current mp
 24:    LDA  6,0(6) 	call: push new frame
 25:    LDA  0,1(7) 	call: save return in ac
 26:    LDC  7,6(0) 	call: unconditional jump to main() entry
 27:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 28:    HALT   0,0,0	
