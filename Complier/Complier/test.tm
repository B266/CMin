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
*<-compound
  7:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,2(7)	func: unconditional jump to next declaration
*-> Function (main)
  8:    LDC  0,-1(0) 	init: load globalOffset
  9:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 10:     ST  6,0(6) 	call: store current mp
 11:    LDA  6,0(6) 	call: push new frame
 12:    LDA  0,1(7) 	call: save return in ac
 13:    LDC  7,6(0) 	call: unconditional jump to main() entry
 14:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 15:    HALT   0,0,0	
