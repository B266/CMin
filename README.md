# CMin
* 该项目是由[Tiny Compiler](http://www.cs.sjsu.edu/faculty/louden/cmptext/)改进而来，C语言子集文法的编译器
* 亦是我们小组的编译原理课程设计（东北大学，物联网1701，[余斐然，刘源，王理庚，李勐]）
* 支持 函数、数组
### Tested Environment
	- Windows 10 

### Requirement
	- Visual Studio 2019

### How to use
1. 克隆该项目
2. 分别建立CMin、TinyMachine两个工程
3. TinyMachine工程将tm.cpp加入其中，编译运行
4. CMin工程将其余代码加入其中，编译运行

### Notice 
与c语言文法不同，我们编译器在函数传递数组时需带“[]”
``` c
int x[10];
fun(x);  //wrong!
fun(x[]); //right!
```

### Example
* 求最大公约数
``` c
/* A Program to perform Euclid`s
   Algorithm to computer gcd */

int gcd (int u, int v)
{
    if (v == 0) return u;
    else return gcd(v,u-u/v*v);
    /* u-u/v*v == u mod v */
}

void main(void)
{
    int x; int y;
    x = input(); y = input();
    output(gcd(x,y));
}
```
* 排序算法
```c
int x[10];

int minloc(int a[],int low,int high)
{
	int i;
	int j;
	int k;

	k=low;
	j=a[low];
	i=low+1;

	while(i<high){
		if(a[i]<j){
			j=a[i];
			k=i;
		}
		i=i+1;
	}
	return k;
}

void sort(int a[],int low,int high)
{
	int i;
	int k;
	int t;

	i=low;

	while(i<high-1)
	{
		k=minloc(a[],i,high);
		t=a[k];
		a[k]=a[i];
		a[i]=t;
		i=i+1;
	}
}

void main(void)
{
	int i;

	i=0;
	while(i<10){
		x[i]=input();
		i=i+1;
	}

	sort(x[],0,10);

	i=0;
	while(i<10){
		output(x[i]);
		i=i+1;
	}
}
```

 ### Reference 
 1. Compiler Construction: Principles and Practice - Kenneth C.Louden   <-没有这本书就没有这个项目
 2. Parsing Techniques - Dick Grune; Ceriel J.H. Jacobs <- 从中学习了部分编译前端知识
 3. [isairz/cminus](https://github.com/isairz/cminus) <-从他的代码里学到了很多，感谢他
 4. [naveenrajm7/c--compiler](https://github.com/naveenrajm7/c--compiler) <- 从这个代码里，理解了内存中栈的分配，感谢他
