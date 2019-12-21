*TINY Compilation to TM Code
*File: test.tm
*Standard prelude:
  0:     LD  5,0(0) 	load gp with maxaddress
  1:    LDA  6,-256(5) 	copy gp to mp
  2:     ST  0,0(0) 	clear location 0
*End of standard prelude.
*-> var. decl.
*<- var. decl.
*-> var. decl.
*<- var. decl.
*-> Function (gcd)
  4:     ST  1,-13(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
  3:    LDC  1,6(0) 	func: load function location
  6:     ST  0,-1(6) 	func: store return address
*-> param
*y
*<- param
*->compound
*-> assign
*-> Id (x)
  7:    LDC  0,-3(0) 	id: load varOffset
  8:     ADD   0,6,0	id: calculate the address found in current frame
  9:    LDA  0,0(0) 	load id address
*<- Id
 10:     ST  0,-4(6) 	assign: push left (address)
*-> Id (y)
 11:    LDC  0,-2(0) 	id: load varOffset
 12:     ADD   0,6,0	id: calculate the address found in current frame
 13:     LD  0,0(0) 	load id value
*<- Id
 14:     LD  1,-4(6) 	assign: load left (address)
 15:     ST  0,0(1) 	assign: store value
*<- assign
*-> Call
*-> Id (x)
 16:    LDC  0,-3(0) 	id: load varOffset
 17:     ADD   0,6,0	id: calculate the address found in current frame
 18:     LD  0,0(0) 	load id value
*<- Id
 19:     ST  0,-6(6) 	call: push argument
 20:     LD  0,-6(6) 	load arg to ac
 21:     OUT   0,0,0	write ac
*<- Call
*<-compound
 22:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,17(7)	func: unconditional jump to next declaration
* frameoffset (-5) 
*-> Function (main)
 24:     ST  1,-14(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
 23:    LDC  1,26(0) 	func: load function location
 26:     ST  0,-1(6) 	func: store return address
*->compound
*-> assign
*-> Id (z)
 27:    LDC  0,-12(0) 	id: load varOffset
 28:     ADD   0,5,0	id: calculate the address found in global scope
 29:    LDA  0,0(0) 	load id address
*<- Id
 30:     ST  0,-2(6) 	assign: push left (address)
*-> Const
 31:    LDC  0,0(0) 	load const
*<- Const
 32:     LD  1,-2(6) 	assign: load left (address)
 33:     ST  0,0(1) 	assign: store value
*<- assign
*-> iter.
*while: jump after body comes back here
*-> Op
*-> Id (z)
 34:    LDC  0,-12(0) 	id: load varOffset
 35:     ADD   0,5,0	id: calculate the address found in global scope
 36:     LD  0,0(0) 	load id value
*<- Id
 37:     ST  0,-2(6) 	op: push left
*-> Const
 38:    LDC  0,10(0) 	load const
*<- Const
 39:     LD  1,-2(6) 	op: load left
 40:     SUB   0,1,0	op <
 41:    JLT  0,2(7) 	br if true
 42:    LDC  0,0(0) 	false case
 43:    LDA  7,1(7) 	unconditional jmp
 44:    LDC  0,1(0) 	true case
*<- Op
*while: jump to end belongs here
*->compound
*-> assign
*-> Id (x)
 46:    LDC  0,-2(0) 	id: load varOffset
 47:     ADD   0,5,0	id: calculate the address
 48:     ST  0,-2(6) 	id: push base address
*-> Id (z)
 49:    LDC  0,-12(0) 	id: load varOffset
 50:     ADD   0,5,0	id: calculate the address found in global scope
 51:     LD  0,0(0) 	load id value
*<- Id
 52:     LD  1,-2(6) 	id: pop base address
 53:     SUB   0,1,0	id: calculate element address with index
 54:    LDA  0,0(0) 	load id address
*<- Id
 55:     ST  0,-2(6) 	assign: push left (address)
*-> Call
 56:      IN   0,0,0	read integer value
*<- Call
 57:     LD  1,-2(6) 	assign: load left (address)
 58:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (z)
 59:    LDC  0,-12(0) 	id: load varOffset
 60:     ADD   0,5,0	id: calculate the address found in global scope
 61:    LDA  0,0(0) 	load id address
*<- Id
 62:     ST  0,-2(6) 	assign: push left (address)
*-> Op
*-> Id (z)
 63:    LDC  0,-12(0) 	id: load varOffset
 64:     ADD   0,5,0	id: calculate the address found in global scope
 65:     LD  0,0(0) 	load id value
*<- Id
 66:     ST  0,-3(6) 	op: push left
*-> Const
 67:    LDC  0,1(0) 	load const
*<- Const
 68:     LD  1,-3(6) 	op: load left
 69:     ADD   0,1,0	op +
*<- Op
 70:     LD  1,-2(6) 	assign: load left (address)
 71:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
 72:     LDA 7,-39(7)	while: jmp back to test
 45:     JEQ 0,27(7)	while: jmp to end
*<- iter.
*-> Call
*-> Id (x)
 73:    LDC  0,-2(0) 	id: load varOffset
 74:     ADD   0,5,0	id: calculate the address
 75:     ST  0,-2(6) 	id: push base address
*-> Const
 76:    LDC  0,2(0) 	load const
*<- Const
 77:     LD  1,-2(6) 	id: pop base address
 78:     SUB   0,1,0	id: calculate element address with index
 79:     LD  0,0(0) 	load id value
*<- Id
 80:     ST  0,-4(6) 	call: push argument
 81:     ST  6,-2(6) 	call: store current mp
 82:    LDA  6,-2(6) 	call: push new frame
 83:    LDA  0,1(7) 	call: save return in ac
 84:     LD  7,-13(5) 	call: relative jump to function entry
 85:     LD  6,0(6) 	call: pop current frame
*<- Call
*<-compound
 86:     LD  7,-1(6) 	func: load pc with return address
 25:     LDA 7,61(7)	func: unconditional jump to next declaration
* frameoffset (-2) 
 87:    LDC  0,-13(0) 	init: load globalOffset
 88:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
 89:     ST  6,0(6) 	call: store current mp
 90:    LDA  6,0(6) 	call: push new frame
 91:    LDA  0,1(7) 	call: save return in ac
 92:    LDC  7,26(0) 	call: unconditional jump to main() entry
 93:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
 94:    HALT   0,0,0	
