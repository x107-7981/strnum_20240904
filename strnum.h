/*
	This is a big C++ program which can help you to solve very big numbers.
	It is writen by x107, a Chinese high school student.
	You can download it on github.
	The recent update : 2024.9.4
	I promise it don't have bugs.
*/
#ifndef STRNUM_H
#define STRNUM_H
#include <iostream>
#ifndef __cplusplus
#error strnum is only for C++
#endif
namespace xxx{
//Do not to much , or may error
const int strnum_inf = 1e3+5;

extern bool isErrorOpen;
class strnum_bitaddress;
class strint;
class strflt;
class strnum;

//father class
class strnum{
public:
	void *ptrhead;
	int lenth;// the number of bytes
	bool sign;// true -> x>=0 , false -> x<=0
	virtual void clean()=0;
	virtual void gain(int numSystemMode = -1){}
	virtual void sout(int numSystemMode = 10){}
	void soutln(int numSystemMode = 10);
	inline unsigned char& operator[](int x);
	[[nodiscard]] strnum_bitaddress& operator()(int x,int bit);
	strnum();
	strnum(void *ptr):ptrhead(ptr){}
	~strnum();
	class iterator{
	private:
		unsigned char *itptr;
	public:
		iterator() = default;
		iterator(const iterator &x):itptr(x.itptr){}
		iterator(iterator &&x):itptr(x.itptr){}
		iterator(unsigned char *x):itptr(x){}
		void operator=(const iterator &x){this->itptr = x.itptr;return;}
		void operator=(iterator &&x){this->itptr = x.itptr;return;}
		void operator=(unsigned char *x){this->itptr = x;return;}
		unsigned char& operator*(){return *(this->itptr);}
		iterator& operator++(){this->itptr++;return *this;}
		iterator operator++(int){iterator x = *this;this->itptr++;return x;}
		iterator& operator--(){this->itptr--;return *this;}
		iterator operator--(int){iterator x = *this;this->itptr--;return x;}
		bool operator==(const iterator &x){return (this->itptr == x.itptr);}
		bool operator==(unsigned char *x){return (this->itptr == x);}
		bool operator!=(const iterator &x){return (this->itptr != x.itptr);}
		bool operator!=(unsigned char *x){return (this->itptr != x);}
		~iterator() = default;
	};
	iterator begin(){return iterator((unsigned char*)this->ptrhead);}
	iterator end(){return iterator((unsigned char*)this->ptrhead + this->lenth);}
	iterator rbegin(){return iterator((unsigned char*)this->ptrhead + this->lenth - 1);}
	iterator rend(){return iterator((unsigned char*)this->ptrhead - 1);}
};

//strnum's bitaddress
class strnum_bitaddress{
	strnum_bitaddress() = delete;
	strnum_bitaddress(const strnum_bitaddress &x) = delete;
	strnum_bitaddress(strnum_bitaddress &&x) = delete;
	void operator=(const strnum_bitaddress &x) = delete;
	void operator=(strnum_bitaddress &&x) = delete;
	friend class strnum;
private:
	unsigned char *byteptr;
	int bit;
	strnum_bitaddress(unsigned char *ptr,int b):byteptr(ptr),bit(b)
	{if(b < 0 || b >= 8){printf("Error! the bit must in range[0,7]!");exit(0);}}
public:
	operator bool(){bool b = (bool)(*byteptr & (1 << bit));delete this;return b;}
	void operator=(bool b)
	{if(b){*byteptr |= (1 << bit);}else{*byteptr &= ~(1 << bit);};delete this;return;}
	~strnum_bitaddress() = default;
};

//iostream (cin and cout)
#pragma {
//give something to cin or cout (please do not use it in your code)
class strnum_stream_io{
	friend class strnum_io;
	friend std::istream& operator>>(std::istream &ifs,strnum_stream_io *ssi_ptr);
	friend std::ostream& operator<<(std::ostream &ofs,strnum_stream_io *ssi_ptr);
private:
	strnum *ptr;
	unsigned char numsys;
	strnum_stream_io() = delete;
	strnum_stream_io(const strnum_stream_io &x) = delete;
	strnum_stream_io(strnum_stream_io &&x) = delete;
	strnum_stream_io(strnum *x,unsigned char nms):ptr(x),numsys(nms){}
	~strnum_stream_io() = default;
};
//iostream tool class (please do not new it's object in your code,but use the defined objects in cin or cout)
class strnum_io{
public:
	friend class strnum_stream_io;
	unsigned char numsys;
	strnum_io(unsigned char nms):numsys(nms){}
	[[nodiscard]] strnum_stream_io* operator[](strnum &x);
};

extern strnum_io strnum_io2,strnum_io8,strnum_io10,strnum_io16;

std::istream& operator>>(std::istream &ifs,strnum_stream_io *ssi_ptr);
std::ostream& operator<<(std::ostream &ofs,strnum_stream_io *ssi_ptr);

std::istream& operator>>(std::istream &ifs,strnum &x);
std::ostream& operator<<(std::ostream &ofs,strnum &x);

#pragma }

//son class 1 : int
class strint:public strnum{
public:
	void clean();
	void gain(int numSystemMode = -1);
	void sout(int numSystemMode = 10);
	strflt strc_strflt();
	long long stoi();
	operator long long(){return this->stoi();}
	operator int(){return (int)this->stoi();}
	operator bool(){return !(this->lenth == 1 && *(char*)this->ptrhead == 0);}
	int log2(bool ignoreSign = false); 
	void operator=(const strint &x);
	void operator=(strint &&x);
	void operator=(long long x);
	strint():strnum(){}
	strint(void *ptr):strnum(ptr){}
	strint(const strint &x);
	strint(strint &&x);
};

strint operator-(const strint &x);

//bit operations :
void operator<<=(strint &x,int y);
void operator>>=(strint &x,int y);
strint operator<<(strint &x,int y);
strint operator>>(strint &x,int y);

//lojic operations :
bool operator==(const strint &x,const strint &y);
bool operator!=(const strint &x,const strint &y);
bool operator>(const strint &x,const strint &y);
bool operator<(const strint &x,const strint &y);
bool operator>=(const strint &x,const strint &y);
bool operator<=(const strint &x,const strint &y);

//maths operations :
strint operator+(const strint &x,const strint &y);
strint operator-(const strint &x,const strint &y);
strint operator*(const strint &x,const strint &y);
strint operator/(const strint &x,const strint &y);
strint operator%(const strint &x,const strint &y);
void operator+=(strint &x,const strint &y);
void operator-=(strint &x,const strint &y);
void operator*=(strint &x,const strint &y);
void operator/=(strint &x,const strint &y);
void operator%=(strint &x,const strint &y);

//------------------------------------beautiful-cutline--------------------------------------//

//son class 2 : float
class strflt:public strnum{
public:
	const static int precision; // max byte number , must small than inf
	const static int deviation; // 1*2^(-x) , if a number sub this small than this number ,they are equal
	bool is_0;
	long long index; // ...... * 2^x
	void clean();
	void gain(int numSystemMode = -1);
	void sout(int numSystemMode = 10);
	strint strc_strint();
	long long stoi();
	operator long long(){return this->stoi();}
	operator int(){return (int)this->stoi();}
	float stof();
	operator float(){return this->stof();}
	double stod();
	operator double(){return this->stod();}
	operator bool(){return !this->is_0;}
	void operator=(const strflt &x);
	void operator=(strflt &&x);
	void operator=(long long x);
	void operator=(float x);
	void operator=(double x);
	strflt():strnum(){}
	strflt(void *ptr):strnum(ptr){}
	strflt(const strflt &x);
	strflt(strflt &&x);
};

strflt operator-(const strflt &x);

//lojic operations :
bool operator==(const strflt &x,const strflt &y);
bool operator!=(const strflt &x,const strflt &y);
bool operator>(const strflt &x,const strflt &y);
bool operator<(const strflt &x,const strflt &y);
bool operator>=(const strflt &x,const strflt &y);
bool operator<=(const strflt &x,const strflt &y);

//maths operations :
strflt operator+(const strflt &x,const strflt &y);
strflt operator-(const strflt &x,const strflt &y);
strflt operator*(const strflt &x,const strflt &y);
strflt operator/(const strflt &x,const strflt &y);
void operator+=(strflt &x,const strflt &y);
void operator-=(strflt &x,const strflt &y);
void operator*=(strflt &x,const strflt &y);
void operator/=(strflt &x,const strflt &y);

}
#endif 