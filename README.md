# CminCompiler
* 该项目是由[Tiny Compiler](http://www.cs.sjsu.edu/faculty/louden/cmptext/)改进而来，C语言子集文法的编译器。
* 亦是我们小组的编译原理课程设计（东北大学，物联网1701，余斐然，刘源，王理庚，李勐）
* 支持 函数、数组
### Tested Environment
	- Windows 10 

### Requirement
	- VS 2019

### How to use
1.分别建立complier、TinyMachine两个工程
2.TinyMachine工程将tm.cpp加入其中，编译运行
3.complier工程将其余代码加入其中，编译运行

### Notice 
与c语言文法不同，我们编译器在函数传递数组时需带“[]”
``` c
int x[10];
fun(x);  //wrong!
fun(x[]); //right!
```

 ### reference 
 1. Compiler Construction: Principles and Practice - Kenneth C.Louden   <-没有这本书就没有这个项目
 2. Parsing Techniques - Dick Grune; Ceriel J.H. Jacobs <- 从中学习了部分编译前端知识
 3. [isairz/cminus](https://github.com/isairz/cminus) <-从他的代码里学到了很多，感谢他
 4. [naveenrajm7/c--compiler](https://github.com/naveenrajm7/c--compiler) <- 从这个代码里，理解了内存中栈的分配，感谢他
