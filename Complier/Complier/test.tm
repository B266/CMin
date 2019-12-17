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
*-> Call
*-> Op
  7:     ST  0,-3(6) 	op: push left
  8:     LD  1,-3(6) 	op: load left
*BUG: Unknown operator 
*<- Op
  9:     ST  0,-5(6) 	call: push argument
 10:     LD  0,-5(6) 	load arg to ac
 11:     OUT   0,0,0	write ac
*<- Call
*<-compound
 12:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,7(7)	func: unconditional jump to next declaration
*-> Function (main)
 13:    LDC  0,-1(0) 	init: load globalOffset
 14:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 15:     ST  6,0(6) 	call: store current mp
 16:    LDA  6,0(6) 	call: push new frame
 17:    LDA  0,1(7) 	call: save return in ac
 18:    LDC  7,6(0) 	call: unconditional jump to main() entry
 19:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 20:    HALT   0,0,0	
