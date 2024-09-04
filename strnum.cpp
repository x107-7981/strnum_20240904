/*
	This is a big C++ program which can help you to solve very big numbers.
	I use bits and bytes to make it fast and small.
	Here are how it record the big number.
	A byte is 8 bit, I use unsigned char* to record the bytes' address .(Because char means that byte.)
	for int: (This is number 185397(10) -> 10,11010010,00110101(2))
		  byte 2                            byte 1                              byte 0
		00000010 -> 2 * 2^(8*2) -> 131072| 11010010 -> 212 * 2^(8*1) -> 54272| 00110101 -> 53 * 2^(8*0) -> 53
	void* ptrhead is in byte 0 's head.
	for float: in C++:
		float: 32 bits , 1 sign , 8 index(+ 127 = 0b1111111) , 23 mantissa
			3.14(10) -> 3 + 0.14 -> 0b11 + _0b00100011110100...... -> 0b1.100100011110100...e-1
			-> 0,1111110,100100011110100........
		double: 64 bits , 1 sign , 11 index(+ 1023 = 0b1111111111) , 52 mantissa
			3.14(10) -> 0,1111111110,100100011110100.....................................
	in strflt : 3.14(10) -> 0b1.100100011110100...e1 -> index = 1 , in bytes:
		byte 0     byte 1     byte 2     byte ...
		10010001 , 1110100. , ........ , ........
*/
#ifndef _GLIBCXX_VECTOR
#include <bits/stdc++.h>
#endif
#include "strnum.h"
#ifdef byte
#undef byte
#endif
#define byte unsigned char
namespace xxx{
using namespace std;

//print error
bool isErrorOpen = true;
strnum_io strnum_io2(2),strnum_io8(8),strnum_io10(10),strnum_io16(16);
[[noreturn]] static void error(int mode){
	if(isErrorOpen) switch(mode){
		case 1: printf("Error! The number system is not include in the support list.");break;
		case 2: printf("Error! The inf is too big that make no mobile.");break;
		case 3: printf("Error! The ptr of wherebegin is a cross address.");break;
		case 4: printf("Error! You can not log to 0 or minus.");break;
		case 5: printf("Error! You can not devide or mod to 0.");break;
	}
	exit(1);
}

//some little fuctions which can change the type of number or check if it is number
static int num_char_to_int(char x){
	if('0'<=x && x<='9')
		return x-'0';
	else
		return x-'a'+10;
}
static char num_int_to_char(int x){
	if(x<10)
		return x+'0';
	else
		return x-10+'a';
}
static bool is_number(int nms,char chr){
	if(nms!=2&&nms!=8&&nms!=10&&nms!=16)
		error(1);
	if(nms == 2){
		if(chr == '0' || chr == '1')
			return true;
		else
			return false;
	}
	if(nms == 8){
		if('0'<=chr && chr<='7')
			return true;
		else
			return false;
	}
	if(nms == 10){
		if('0'<=chr && chr<='9')
			return true;
		else
			return false;
	}
	if(nms == 16){
		if('0'<=chr && chr<='9')
			return true;
		else if('a'<=chr && chr<='f')
			return true;
		else
			return false;
	}
	return false;
}

//get number system and sign
struct RtStru{
	bool RtSign;
	int RtNms;
	char RtChr;
	RtStru(){
		RtSign = false;
		RtNms = 2;
		RtChr = '0';
	}
};
//auto get
static RtStru getSystemMode(bool isPoint = false){ //All of directly return rtstc is mean the number is 0.
	RtStru rtstc;
	bool nsflag = false;
	bool sign = false;
	char chr;
	while(1){
		chr = getchar();
		if(chr == '0' && (isPoint == false || nsflag == false)){
			nsflag = true;
			continue;
		}
		if(nsflag){
			if(chr == 'x'){                   // 0x** -> 16
				chr = getchar();
				if(chr == '0' && isPoint == true){
					chr = getchar();
					if(chr == '.'){
						rtstc.RtSign = sign;
						rtstc.RtNms = 16;
						rtstc.RtChr = chr;
						return rtstc;
					}else{
						return rtstc;
					}
				}else if(!(('1'<=chr && chr<='9') || ('a'<=chr && chr<='f'))){
					return rtstc;
				}
				rtstc.RtSign = sign;
				rtstc.RtNms = 16;
				rtstc.RtChr = chr;
				return rtstc; 
			}else if(chr == 'b'){              // 0b** -> 2
				chr = getchar();
				if(chr == '0' && isPoint == true){
					chr = getchar();
					if(chr == '.'){
						rtstc.RtSign = sign;
						rtstc.RtNms = 2;
						rtstc.RtChr = chr;
						return rtstc;
					}else{
						return rtstc;
					}
				}else if(chr!='1'){
					return rtstc;
				}
				rtstc.RtSign = sign;
				rtstc.RtNms = 2;
				rtstc.RtChr = chr;
				return rtstc;
			}else if(chr == '0' && isPoint == true){ // point : 00.x -> 8
				chr = getchar();
				if(chr == '.'){
					rtstc.RtSign = sign;
					rtstc.RtNms = 8;
					rtstc.RtChr = chr;
					return rtstc;
				}else{
					return rtstc;
				}
			}else if('1'<=chr && chr<='7'){    // 0*** -> 8
				rtstc.RtSign = sign;
				rtstc.RtNms = 8;
				rtstc.RtChr = chr;
				return rtstc;
			}else if(chr == '.' && isPoint == true){			// point : 0.xx -> 10
				rtstc.RtSign = sign;
				rtstc.RtNms = 10;
				rtstc.RtChr = chr; 
				return rtstc;
			}else{
				return rtstc;
			}
		}
		if('1'<= chr && chr<='9'){			  // **** -> 10
			rtstc.RtSign = sign;
			rtstc.RtNms = 10;
			rtstc.RtChr = chr; 
			return rtstc;
		}
		if(chr == '-'){
			sign = true;
			continue;
		}
	}
}
//unauto get
static pair<bool,char> getSign(int nms,bool isPoint = false){
	pair<bool,char> rtpir;
	bool sign = false;
	bool flag1 = false,flag2 = false;
	char chr;
	while(1){
		chr = getchar();
		if(chr == '-'){
			sign = true;
			continue;
		}
		if(chr == '0'){
			flag1 = true;
			continue;
		}
		if(flag1){
			if((((nms == 2)&&(chr == 'b')) || ((nms == 16)&&(chr == 'x'))) && (!flag2)){
				flag2 == true;
				continue;
			}else if(chr == '0' && isPoint == true){
				chr = getchar();
				if(chr == '.'){
					rtpir.first = sign;
					rtpir.second = chr;
					return rtpir;
				}else{
					rtpir.first = false;
					rtpir.second = '0';
					return rtpir;
				}
			}else if(is_number(nms,chr) && chr!='0'){
				rtpir.first = sign;
				rtpir.second = chr;
				return rtpir;
			}else if(chr == '.' && isPoint == true){
				rtpir.first = sign;
				rtpir.second = chr;
				return rtpir;
			}else{
				rtpir.first = false;
				rtpir.second = '0';
				return rtpir;
			}
		}
		if(is_number(nms,chr) && chr!='0'){
			rtpir.first = sign;
			rtpir.second = chr;
			return rtpir;
		}
	} 
}

//print from vector
static void printVector(vector<int> x){
	for(int i=0;i<x.size();i++)
		printf("%c",num_int_to_char(x[i]));
	return;
}

//iostream
[[nodiscard]] strnum_stream_io* strnum_io::operator[](strnum &x){
	strnum_stream_io *ssi_ptr = new strnum_stream_io(&x,this->numsys);
	return ssi_ptr;
}

//gain bit
[[nodiscard]] strnum_bitaddress& strnum::operator()(int x,int bit){
	return *(new strnum_bitaddress((unsigned char*)this->ptrhead + x,bit));
}

//give and free from cpu
strnum::strnum(){
	this->ptrhead = malloc(strnum_inf);
	if(this->ptrhead == NULL)
		error(2);
	memset(this->ptrhead,0,strnum_inf);
	return;
}
strnum::~strnum(){
	if(this->ptrhead)
		free(this->ptrhead);
	this->ptrhead = NULL;
	return;
}

//println
void strnum::soutln(int numSystemMode){
	this->sout(numSystemMode);
	printf("\n");
	return;
}

//return bytes
inline unsigned char& strnum::operator[](int x){
	return *((unsigned char*)this->ptrhead + x);
}

//stream in (cin)
std::istream& operator>>(std::istream &ifs,strnum_stream_io *ssi_ptr){
	ssi_ptr->ptr->gain(ssi_ptr->numsys);
	delete ssi_ptr;
	return ifs;
}
std::istream& operator>>(std::istream &ifs,strnum &x){
	x.gain();
	return ifs;
}

//stream out (cout)
std::ostream& operator<<(std::ostream &ofs,strnum_stream_io *ssi_ptr){
	ssi_ptr->ptr->sout(ssi_ptr->numsys);
	delete ssi_ptr;
	return ofs;
}
std::ostream& operator<<(std::ostream &ofs,strnum &x){
	x.sout();
	return ofs;
}

//There are defining for strint from there:

//copy constructor
strint::strint(const strint &x):strnum(){
	this->lenth = x.lenth;
	this->sign = x.sign;
	for(int i=0;i<this->lenth;i++){
		*((char*)this->ptrhead + i) = *((char*)x.ptrhead + i);
	}
	return;
}

//move construtor
strint::strint(strint &&x){
	this->lenth = x.lenth;
	this->sign = x.sign;
	this->ptrhead = x.ptrhead;
	x.ptrhead = NULL;
	return;
}

//give opposite number
strint operator-(const strint &x){
	if(x.lenth == 1 && *((unsigned char*)x.ptrhead) == 0)
		return x;
	strint y;
	y = x;
	y.sign = !y.sign;
	return y;
}

//clean the class
void strint::clean(){
	this->lenth = 0;
	this->sign = false;
	memset(this->ptrhead,0,strnum_inf);
	return;
}

//scanf it (in)
void strint::gain(int numSystemMode){
	this->clean();
	int nms;
	char chr;
	if(numSystemMode!=-1&&numSystemMode!=2&&numSystemMode!=8&&numSystemMode!=10&&numSystemMode!=16){
		error(1);
	}
	
	//auto gain number system and sign
	if(numSystemMode == -1){
		RtStru rtstc = getSystemMode(); 
		nms = rtstc.RtNms;
		chr = rtstc.RtChr;
		if(chr=='0'){
			this->sign = false;
			this->lenth = 1;
			*((char*)ptrhead) = 0;
			return;
		}
		this->sign = rtstc.RtSign;
	}else{		//unauto gain
		pair<bool,char> rtpir = getSign(numSystemMode);
		nms = numSystemMode;
		chr = rtpir.second;
		if(chr=='0'){
			this->sign = false;
			this->lenth = 1;
			*((char*)ptrhead) = 0;
			return;
		}
		this->sign = rtpir.first;
	}
	
	//read the remain number
	vector<int> sum;
	sum.push_back(num_char_to_int(chr));
	while(1){
		chr = getchar();
		if(!is_number(nms,chr)){
			break;
		}
		sum.push_back(num_char_to_int(chr));
	}
	
	//record the number
	//for 2:
	if(nms == 2){
		int bitaddress = 0;
		char *nowptr = (char*)this->ptrhead;
		*nowptr = 0;
		this->lenth = 1;
		for(int i=sum.size()-1;i>=0;i--){  // be careful: sum.size() is not means sizeof(sum)
			 int k = sum[i];
			 k <<= bitaddress;
			 *nowptr |= (char)k;
			 bitaddress++;
			 if(bitaddress == 8 && i){
			 	bitaddress = 0;
			 	nowptr++;
			 	*nowptr = 0;
			 	this->lenth++;
			 }
		} 
		return;
	} 
	//for 8: a number use 3 bits : 000 001 010 011 100 101 110 111
	if(nms == 8){
		int bitaddress = 0; //this is the bit where you start to do something
		char *nowptr = (char*)this->ptrhead;
		*nowptr = 0;
		this->lenth = 1;
		for(int i=sum.size()-1;i>=0;i--){
			unsigned char k = (char)sum[i];// use unsigned to prevent rightmove include signbit
			k <<= bitaddress;// do not care about overflow
			*nowptr |= k;
			bitaddress += 3;
			if(bitaddress >= 8){ // there are not "&& i" , becouse it may cross the byte
				bitaddress -= 8;
				nowptr++;
				*nowptr = 0;
				this->lenth++;
				if(bitaddress){ // it is mean that cross byte : 1 -> 1|01... , 2 -> 10|1...
					k = (char)sum[i];
					if(bitaddress == 1)
						k >>= 2;
					else
						k >>= 1;
					*nowptr |= k;
				}
			}
		}
		if(*((char*)(this->ptrhead)+this->lenth-1) == 0) this->lenth--; // prevent the high byte is 0
		return;
	}
	//for 16: a number use 4 bits : 0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
	if(nms == 16){
		bool bitaddress = false; // false means 0 , true means 4
		char *nowptr = (char*)this->ptrhead;
		*nowptr = 0;
		this->lenth = 1;
		for(int i=sum.size()-1;i>=0;i--){
			char k = (char)sum[i];
			k <<= (bitaddress?4:0);
			*nowptr |= k;
			bitaddress = !bitaddress;
			if(!bitaddress && i){
				nowptr++;
				*nowptr = 0;
				this->lenth++;
			}
		} 
		return;
	}
	//for 10: mod to 2 , back to see
	if(nms == 10){
		//devide and take mod
		vector<int> model;
		int sumsize = sum.size();
		do{
			for(int i=sum.size()-sumsize;i<sum.size();i++){
				if(i+1 == sum.size()){
					model.push_back(sum[i]&1);
					sum[i] >>= 1; 
					break;
				}
				if(sum[i]&1){
					sum[i+1] += 10;
				}
				sum[i] >>= 1;
			}
			for(int i=sum.size()-sumsize;i<sum.size();i++){
				if(sum[i]) break;
				sumsize--;
			}
		}while(sumsize);
		
		//back to see and record
		int bitaddress = 0;
		char *nowptr = (char*)this->ptrhead;
		*nowptr = 0;
		this->lenth = 1;
		for(int i=0;i<model.size();i++){
			 int k = model[i];
			 k <<= bitaddress;
			 *nowptr |= (char)k;
			 bitaddress++;
			 if(bitaddress == 8 && i){
			 	bitaddress = 0;
			 	nowptr++;
			 	*nowptr = 0;
			 	this->lenth++;
			 }
		} 
		return;
	}
}

//print it (out)
void strint::sout(int nms){
	if(nms!=2&&nms!=8&&nms!=10&&nms!=16){
		error(1);
	}
	if(lenth == 1 && *(char*)this->ptrhead == 0){
		printf("0");
		return;
	}
	if(this->sign)
		printf("-");
	//for 2:
	if(nms == 2){
		printf("0b");
		for(int i=this->lenth-1;i>=0;i--){
			unsigned char nowbyte = *((char*)(this->ptrhead)+i);
			bool flag = false;
			for(int j=7;j>=0;j--){
				// give the highst bit;
				unsigned char k = nowbyte;
				k <<= 1;
				k >>= 1;
				k = ~k;
				int highbit = (k&nowbyte?1:0);
				if(i!=this->lenth-1 || flag || highbit==1){
					flag = true;
					printf("%d",highbit);
				}
				nowbyte <<= 1;
			}
		}
		return;
	}
	// for 8: 3 bits is a number
	if(nms == 8){ 
		printf("0");
		vector<int> sum;
		pair<int,int> rmbit = {0,0}; // first -> how many need; second -> what is
		for(int i=0;i<lenth;i++){
			int nowbit = 0 + rmbit.first;
			unsigned char nowbyte = *((char*)(this->ptrhead)+i);
			
			// solve remain bits 
			if(rmbit.first){
				if(rmbit.first == 1){
					int q = 1; // 0b1
					q = q & nowbyte;
					q <<= 2;
					q = q | rmbit.second;
					sum.push_back(q);
				} 
				else{
					int q = 3; // 0b11
					q = q & nowbyte;
					q <<= 1;
					q = q | rmbit.second;
					sum.push_back(q);
				}
				nowbyte >>= nowbit;
				rmbit = {0,0};
			}
			
			// solve main bits
			while(nowbit <= 5){
				int k = 7; // 0b111
				k = k & nowbyte;
				sum.push_back(k);
				nowbyte >>= 3;
				nowbit += 3;
			}
			
			// record remain bits
			if(nowbit == 8){
				rmbit = {0,0};
				continue;
			}
			else if(nowbit == 6){
				rmbit = {1,nowbyte};
				continue;
			}
			else{
				rmbit = {2,nowbyte};
				continue;
			}
		}
		if(rmbit.first) sum.push_back(rmbit.second);
		
		//print
		bool flag = false;
		for(int i=sum.size()-1;i>=0;i--){
			if(sum[i] || flag){
				flag = true;
				printf("%d",sum[i]);
			}
		}
		return;
	}
	//for 16: 4 bits a num
	if(nms == 16){
		printf("0x");
		for(int i=lenth-1;i>=0;i--){
			unsigned char nowbyte = *((char*)(this->ptrhead)+i);
			if(i == lenth-1 && (nowbyte>>4) == 0){
				nowbyte <<= 4;
				nowbyte >>= 4;
				printf("%c",num_int_to_char(nowbyte));
				continue;
			}
			printf("%c",num_int_to_char(nowbyte>>4));
			nowbyte <<= 4;
			nowbyte >>= 4;
			printf("%c",num_int_to_char(nowbyte));
		}
		return;
	}
	//for 10:
	if(nms == 10){
		int a[strnum_inf];
		int maxsize = 0;
		memset(a,0,sizeof(a));
		for(int i=0;i<lenth;i++){
			vector<int> b;
			unsigned char nowbyte = *((char*)(this->ptrhead)+i);
			b.push_back(nowbyte);
			while(b[0] >= 10){
				int k=0;
				while(b[k] >= 10){
					if(b.size()==k+1)
						b.push_back(1);
					else
						b[k+1]++;
					b[k] -= 10;
					k++;
				}
			}
			
			// leftmove for i times ,8 bit per time
			for(int lmc=1;lmc<=i;lmc++){
				int bsize = b.size();
				for(int j=bsize-1;j>=0;j--){
					b[j] <<= 8;
					while(b[j] >= 10){
						int k=j;
						while(b[k] >= 10){
							if(b.size()==k+1)
								b.push_back(1);
							else
								b[k+1]++;
							b[k] -= 10;
							k++;
						}
					}
				}
			}
			
			// the numbers in b into a
			maxsize = max(maxsize,(int)b.size());
			for(int j=b.size()-1;j>=0;j--){
				a[j] += b[j];
				while(a[j] >= 10){
					int k=j;
					while(a[k] >= 10){
						if(maxsize == k+1)
							maxsize++;
						a[k+1]++;
						a[k] -= 10;
						k++;
					}
				}
			}
		}
		for(int i=maxsize-1;i>=0;i--){
			printf("%d",a[i]);
		}
		return;
	}
}

//give the value
void strint::operator=(const strint &x){
	this->clean();
	char *ptrx = (char*)x.ptrhead;
	this->lenth = x.lenth;
	this->sign = x.sign;
	for(int i=0;i<x.lenth;i++){
		char *nowptr = (char*)this->ptrhead + i;
		*nowptr = *ptrx;
		ptrx++;
	}
	return;
}
void strint::operator=(strint &&x){
	this->clean();
	if(this->ptrhead)
		free(this->ptrhead);
	this->lenth = x.lenth;
	this->sign = x.sign;
	this->ptrhead = x.ptrhead;
	x.ptrhead = NULL;
	return;
}
void strint::operator=(long long x){
	this->clean();
	if(x == 0){
		this->sign = false;
		this->lenth = 1;
		*(char*)(this->ptrhead) = 0;
		return;
	}else if(x < 0){
		this->sign = true;
		x--;
		x = ~x;
	}else this->sign = false;
	
	int i=0;
	while(x){
		char *nowptr = (char*)this->ptrhead + i;
		unsigned char k = (char)(x & 255); // 255 = 0b11111111
		*nowptr = k;
		x >>= 8;
		i++;
	}
	this->lenth = i;
	return;
}
long long strint::stoi(){
	long long ans = 0;
	for(int i=0;i<min(8,this->lenth);i++){ // sizeof(long long) = 8
		unsigned char nowbyte = *((char*)(this->ptrhead) + i);
		ans |= (long long)(nowbyte) << (8*i);
	}
	long long numsign = (this->sign?-1:1);
	ans *= numsign;
	return ans;
}
strflt strint::strc_strflt(){
	if(this->lenth == 1 && *(char*)this->ptrhead == 0){
		strflt ans;
		ans = (long long)0;
		return ans;
	}
	strflt ans;
	ans.clean();
	ans.is_0 = false;
	ans.sign = this->sign;
	ans.index = this->log2(true);
	int highbit;
	unsigned char highbyte = *((unsigned char*)this->ptrhead + this->lenth -1);
	for(int i=7;i>=0;i--){
		if((1<<i) & highbyte){
			highbit = i;
			break;
		}
	}
	int start_ = this->lenth;
	if(highbit == 0){
		start_ = ans.lenth = this->lenth - 1;
		highbit = 8;
	}else ans.lenth = this->lenth;

	ans.lenth = min(ans.lenth,strflt::precision);

	int cnt = 0;
	for(int i=start_-1;i>=(start_ - ans.lenth);i--){
		unsigned char nowbyte = *((unsigned char*)this->ptrhead + i);
		int k = (1 << highbit) - 1;
		k &= nowbyte;
		k <<= (8 - highbit);
		if(i != 0 && highbit != 8){
			unsigned char nxtbyte = *((unsigned char*)this->ptrhead + i -1);
			k |= (((0xff - (1 << highbit) + 1) & nxtbyte) >> highbit);
		}
		*((unsigned char*)ans.ptrhead + cnt) = k;
		cnt++;
	}
	return ans;
}

// logv2 operation
int strint::log2(bool ignoreSign){
	if((this->lenth == 1 && *(char*)(this->ptrhead) == 0) || (this->sign && !ignoreSign)){
		if(isErrorOpen)
			error(4);
		else
			return -1;
	}
	int ans = 0;
	for(int i=7;i>=0;i--){
		unsigned char nowbyte = *((unsigned char*)(this->ptrhead) + lenth - 1);
		unsigned char k = 1;
		k <<= i;
		if(k & nowbyte){
			ans = i;
			break;
		}
	}
	ans |= ((this->lenth - 1) << 3);
	return ans;
}

//some lojic operations
bool operator==(const strint &x,const strint &y){
	if(x.sign != y.sign){
		return false;
	}
	if(x.lenth != y.lenth){
		return false;
	}
	for(int i=0;i<x.lenth;i++){
		char *ptrx = (char*)(x.ptrhead) + i;
		char *ptry = (char*)(y.ptrhead) + i;
		if(*ptrx != *ptry)
			return false;
	}
	return true;
}
bool operator!=(const strint &x,const strint &y){
	return !(x == y);
}
bool operator>(const strint &x,const strint &y){
	if(x.sign != y.sign){
		if(x.sign)
			return false;
		else
			return true;
	}

	bool absbs;
	if(x.lenth < y.lenth){
		absbs = false;
		goto solve_return;
	}
	if(x.lenth > y.lenth){
		absbs = true;
		goto solve_return;
	}
	for(int i=x.lenth-1;i>=0;i--){
		unsigned char *ptrx = (unsigned char*)(x.ptrhead) + i;
		unsigned char *ptry = (unsigned char*)(y.ptrhead) + i;
		if(*ptrx < *ptry){
			absbs = false;
			goto solve_return;
		}
		if(*ptrx > *ptry){
			absbs = true;
			goto solve_return;
		}
	}
	return false;
	
	solve_return:
	return x.sign?(!absbs):absbs;
}
bool operator<(const strint &x,const strint &y){
	if(x.sign != y.sign){
		if(x.sign)
			return true;
		else
			return false;
	}

	bool absbs;
	if(x.lenth < y.lenth){
		absbs = true;
		goto solve_return;
	}
	if(x.lenth > y.lenth){
		absbs = false;
		goto solve_return;
	}
	for(int i=x.lenth-1;i>=0;i--){
		unsigned char *ptrx = (unsigned char*)(x.ptrhead) + i;
		unsigned char *ptry = (unsigned char*)(y.ptrhead) + i;
		if(*ptrx < *ptry){
			absbs = true;
			goto solve_return;
		}
		if(*ptrx > *ptry){
			absbs = false;
			goto solve_return;
		}
	}
	return false;
	
	solve_return:
	return x.sign?(!absbs):absbs;
}
bool operator>=(const strint &x,const strint &y){
	return !(x < y);
}
bool operator<=(const strint &x,const strint &y){
	return !(x > y);
}

//some bit operations
void operator<<=(strint &x,int y){
	if(x.lenth == 1 && *((char*)(x.ptrhead)) == 0)
		return;
	if(y == 0)
		return;
	if(y < 0){
		x >>= (-y);
		return;
	}
	int lmbyte = (y >> 3);
	int lmbit = (y & ~(lmbyte << 3));
	
	//clear
	for(int i=x.lenth;i<x.lenth + lmbyte + 1;i++){
		char *nowptr = (char*)x.ptrhead + i;
		*nowptr = 0;
	}

	// move bytes
	if(lmbyte){
		for(int i=x.lenth-1;i>=0;i--){
			char *nowptr = (char*)x.ptrhead + i;
			char *nxtptr = nowptr + lmbyte;
			*nxtptr = *nowptr;
			*nowptr = 0;
		}
		x.lenth += lmbyte;
	}
	
	//move bits
	*((char*)x.ptrhead + x.lenth) = 0; 
	for(int i=x.lenth-1;i>=lmbyte;i--){
		char *nowptr = (char*)x.ptrhead + i;
		unsigned char k = *nowptr;
		k <<= lmbit;
		k >>= lmbit;
		k = ~k;
		k = (k & *nowptr);
		k >>= (8 - lmbit);
		*(nowptr + 1) |= k;
		*nowptr <<= lmbit;
	}
	if(*((char*)x.ptrhead + x.lenth))
		x.lenth++;
	
	return;
}
void operator>>=(strint &x,int y){
	if(x.lenth == 1 && *((char*)(x.ptrhead)) == 0)
		return;
	if(y == 0)
		return;
	if(y < 0){
		x <<= (-y);
		return;
	}
	int rmbyte = (y >> 3);
	int rmbit = (y & ~(rmbyte << 3));
	
	//move bytes
	for(int i=0;i<x.lenth;i++){
		if(i-rmbyte < 0)
			continue;
		char *nowptr = (char*)x.ptrhead + i;
		char *nxtptr = nowptr - rmbyte;
		*nxtptr = *nowptr;
	}
	x.lenth -= rmbyte;
	if(x.lenth <= 0){
		x.lenth = 1;
		*((char*)(x.ptrhead)) = 0;
		return;
	}
	
	//move bits
	for(int i=0;i<x.lenth;i++){
		unsigned char *nowptr = (unsigned char*)x.ptrhead + i;
		if(!i){
			*nowptr >>= rmbit;
			continue;
		}
		unsigned char k = *nowptr;
		k <<= (8 - rmbit);
		*(nowptr - 1) |= k;
		*nowptr >>= rmbit;
	}
	if((*((char*)(x.ptrhead) + x.lenth -1) == 0) && (x.lenth != 1))
		x.lenth--;
	
	return;
}
strint operator<<(const strint &x,int y){
	strint ans = x;
	ans <<= y;
	return ans;
}
strint operator>>(const strint &x,int y){
	strint ans = x;
	ans >>= y;
	return ans;
}

//some maths operations 

//unsigned plus
strint unsigned_plus(const strint &in1,const strint &in2){
	strint x,y;
	if(in1 <= in2){ // x >= y
		x = in2;
		y = in1;
	}else{
		x = in1;
		y = in2;
	}
	x.sign = false;
	y.sign = false;
	
	strint ans;
	ans.clean();
	ans.sign = false;
	
	//plus per byte
	bool flag = false; // if it plus 1 from last byte
	for(int i=0;i<y.lenth;i++){
		unsigned char xbyte = *((char*)(x.ptrhead) + i);
		unsigned char ybyte = *((char*)(y.ptrhead) + i);
		char *nowbyte = (char*)ans.ptrhead + i;
		*nowbyte = xbyte;
		
		// plus 1
		if(flag){
			flag = false;
			int k = 1;
			while((*nowbyte & k) && k!=256){ // 256 = 0b1,00000000
				*nowbyte ^= k;
				k <<= 1;
			}
			if(k == 256)
				flag = true;
			else
				*nowbyte ^= k;
		}
		
		//plus ybyte
		int j=0;
		while(ybyte){
			if(ybyte & 1){
				int k = 1;
				k <<= j;
				while((*nowbyte & k) && k!=256){ // 256 = 0b1,00000000
					*nowbyte ^= k;
					k <<= 1;
				}
				if(k == 256)
					flag = true;
				else
					*nowbyte ^= k;
			}
			ybyte >>= 1; 
			j++;
		}
	}
	
	//pick remain bytes
	for(int i=y.lenth;i<x.lenth;i++){
		unsigned char xbyte = *((char*)(x.ptrhead) + i);
		char *nowbyte = (char*)ans.ptrhead + i;
		*nowbyte = xbyte;
		if(flag){
			flag = false;
			int k = 1;
			while((*nowbyte & k) && k!=256){ // 256 = 0b1,00000000
				*nowbyte ^= k;
				k <<= 1;
			}
			if(k == 256)
				flag = true;
			else
				*nowbyte ^= k;
		}
	}
	
	//return
	if(flag){
		ans.lenth = x.lenth + 1;
		*((char*)(ans.ptrhead) + ans.lenth - 1) = 1; 
		return ans;
	}else{
		ans.lenth = x.lenth;
		return ans;
	}
}

//unsigned subtract
strint unsigned_subtract(const strint &in1,const strint &in2){
	strint ans;
	strint x,y;
	ans.clean();
	if(in1 == in2){
		ans = 0;
		return ans;
	}else if(in1 < in2){ // x > y
		ans.sign = true;
		x = in2;
		y = in1;
	}else{
		ans.sign = false;
		x = in1;
		y = in2;
	}
	
	//subtract per bit
	bool flag = false; // if need a bit from high byte 
	for(int i=0;i<y.lenth;i++){
		unsigned char xbyte = *((char*)(x.ptrhead) + i);
		unsigned char ybyte = *((char*)(y.ptrhead) + i);
		char *nowbyte = (char*)ans.ptrhead + i;
		*nowbyte = xbyte;
		
		//subtract 1
		if(flag){
			flag = false;
			int k = 1;
			while((k & (~*nowbyte)) && k != 256){ // 256 = 0b1,00000000
				*nowbyte ^= k;
				k <<= 1;
			}
			if(k == 256)
				flag = true;
			else
				*nowbyte ^= k;
		}
		
		//subtract ybyte
		int j=0;
		while(ybyte){
			if(ybyte & 1){
				int k = 1;
				k <<= j;
				while((k & (~*nowbyte)) && k != 256){ // 256 = 0b1,00000000
					*nowbyte ^= k;
					k <<= 1;
				}
				if(k == 256)
					flag = true;
				else
					*nowbyte ^= k;
			}
			ybyte >>= 1;
			j++;
		}
	}
	
	// pick remain bytes
	for(int i=y.lenth;i<x.lenth;i++){
		unsigned char xbyte = *((char*)(x.ptrhead) + i);
		char *nowbyte = (char*)ans.ptrhead + i;
		*nowbyte = xbyte;
		if(flag){
			flag = false;
			int k = 1;
			while((k & (~*nowbyte)) && k != 256){ // 256 = 0b1,00000000
				*nowbyte ^= k;
				k <<= 1;
			}
			if(k == 256)
				flag = true;
			else
				*nowbyte ^= k;
		}
	}
	
	//return
	ans.lenth = x.lenth;
	for(int i=x.lenth-1;i>=0;i--){
		unsigned char nowbyte = *((char*)(ans.ptrhead) + i);
		if(nowbyte){
			break;
		}
		ans.lenth--;
	}
	return ans;
}

//plus
strint operator+(const strint &x,const strint &y){
	if(x.sign != y.sign){
		if(!x.sign)
			return unsigned_subtract(x,y);
		else
			return -unsigned_subtract(x,y);
	}else{
		if(!x.sign)
			return unsigned_plus(x,y);
		else
			return -unsigned_plus(x,y);
	}
}
void operator+=(strint &x,const strint &y){
	x = x + y;
	return;
}

//subtract
strint operator-(const strint &x,const strint &y){
	if(x.sign == y.sign){
		if(!x.sign)
			return unsigned_subtract(x,y);
		else
			return -unsigned_subtract(x,y);
	}else{
		if(!x.sign)
			return unsigned_plus(x,y);
		else
			return -unsigned_plus(x,y);
	}
}
void operator-=(strint &x,const strint &y){
	x = x - y;
	return;
}

//multy
strint operator*(const strint &in1,const strint &in2){
	//solve x and y
	strint _0;
	_0 = 0;
	strint x,y;
	x = in1;y = in2;
	x.sign = false,y.sign = false;
	if(x == _0 || y == _0)
		return _0;
	if(x < y){ //x >= y
		x = in2;y = in1;
		x.sign = false,y.sign = false;
	}
	bool finsign = (in1.sign != in2.sign);

	//prevent use ~strnum
	strint ans;
	ans = 0;
	
	//multy per bit
	for(int i=0;i<y.lenth;i++){
		strint xlm;
		xlm = x;
		xlm <<= (i<<3);
		for(int j=0;j<=7;j++){
			unsigned char ybyte = *((char*)(y.ptrhead) + i);
			ybyte >>= j;
			if(ybyte & 1){
				ans = ans + xlm;
			}
			xlm <<= 1;
		}
	}
	ans.sign = finsign;
	return ans;
}
void operator*=(strint &x,const strint &y){
	x = x * y;
	return;
}

//devide
strint operator/(const strint &in1,const strint &in2){
	bool finsign = (in1.sign != in2.sign);
	strint _0,ans;
	_0 = 0 ; ans = 0;
	strint x,y;
	x = in1 ; y = in2;
	x.sign = false ; y.sign = false;
	if(y == _0) error(5);
	if(x < y) return _0;
	
	int rbit = x.log2() - y.log2();
	do{
		strint k;
		k = x;
		k >>= rbit;
		k = k - y;
		if(k < _0){
			rbit--;
			continue;
		}else{
			k = y;
			k <<= rbit;
			x = x - k;
			strint del;
			del = 1;
			del <<= rbit;
			ans = ans + del;
			if(x > _0)
				rbit = x.log2() - y.log2();
		}
	}while(rbit >= 0 && x > _0);

	ans.sign = finsign;
	return ans;
}
void operator/=(strint &x,const strint &y){
	x = x / y;
	return;
}

//mod
strint operator%(const strint &in1,const strint &in2){
	bool finsign = in1.sign;
	strint x,y;
	x = in1 ; y = in2;
	x.sign = false ; y.sign = false;
	strint ans;
	ans = x / y;
	ans = ans * y;
	ans = x - ans;
	ans.sign = finsign;
	return ans;
}
void operator%=(strint &x,const strint &y){
	x = x % y;
	return;
}

// end strint

//------------------------------------beautiful-cutline--------------------------------------//

//There are defining for strflt from there:

const int strflt::precision = 55;
const int strflt::deviation = 16;

//copy constructor
strflt::strflt(const strflt &x):strnum(){
	this->lenth = x.lenth;
	this->index = x.index;
	this->sign = x.sign;
	this->is_0 = x.is_0;
	if(!this->is_0 && this->lenth)
		for(int i=0;i<this->lenth;i++)
			*((char*)this->ptrhead + i) = *((char*)x.ptrhead + i);
	return;
}

//move constructor
strflt::strflt(strflt &&x){
	this->lenth = x.lenth;
	this->index = x.index;
	this->sign = x.sign;
	this->is_0 = x.is_0;
	this->ptrhead = x.ptrhead;
	x.ptrhead = NULL;
	return;
}

//give opposite number
strflt operator-(const strflt &x){
	if(x.is_0)
		return x;
	strflt ans = x;
	ans.sign = !(ans.sign);
	return ans;
}

//clean the class
void strflt::clean(){
	this->is_0 = false;
	this->lenth = 0;
	this->sign = false;
	this->index = 0;
	if(this->ptrhead)
		memset(this->ptrhead,0,strnum_inf);
	return;
}

//scanf it (in)
void strflt::gain(int numSystemMode){
	this->clean();
	int nms;
	char chr;
	bool hasPoint = false;
	if(numSystemMode!=-1&&numSystemMode!=2&&numSystemMode!=8&&numSystemMode!=10&&numSystemMode!=16){
		error(1);
	}
	
	//auto gain number system and sign
	if(numSystemMode == -1){
		RtStru rtstc = getSystemMode(true); 
		nms = rtstc.RtNms;
		chr = rtstc.RtChr;
		if(chr=='0'){
			this->sign = false;
			this->lenth = 0;
			this->is_0 = true;
			this->index = 0;
			return;
		}else if(chr == '.'){
			hasPoint = true;
			chr = getchar();
			if(!is_number(nms,chr)){
				this->sign = false;
				this->lenth = 0;
				this->is_0 = true;
				this->index = 0;
				return;
			}
		}
		this->sign = rtstc.RtSign;
	}else{		//unauto gain
		pair<bool,char> rtpir = getSign(numSystemMode,true);
		nms = numSystemMode;
		chr = rtpir.second;
		if(chr=='0'){
			this->sign = false;
			this->lenth = 0;
			this->is_0 = true;
			this->index = 0;
			return;
		}else if(chr == '.'){
			hasPoint = true;
			chr = getchar();
			if(!is_number(nms,chr)){
				this->sign = false;
				this->lenth = 0;
				this->is_0 = true;
				this->index = 0;
				return;
			}
		}
		this->sign = rtpir.first;
	}

	//read the remain number
	vector<int> sum,mantissa;
	if(hasPoint == true)
		mantissa.push_back(num_char_to_int(chr));
	else
		sum.push_back(num_char_to_int(chr));
	while(1){
		chr = getchar();
		if(chr == '.' && !hasPoint){
			hasPoint = true;
			continue;
		}
		if(!is_number(nms,chr)){
			break;
		}
		if(hasPoint == true)
			mantissa.push_back(num_char_to_int(chr));
		else
			sum.push_back(num_char_to_int(chr));
	}
	while(mantissa.size() && mantissa[mantissa.size()-1] == 0)
		mantissa.pop_back();
	if(sum.size() == 0 && mantissa.size() == 0){
		this->sign = false;
		this->lenth = 0;
		this->is_0 = true;
		this->index = 0;
		return;
	}

	//record the number 

	//lambda which can solve bits
	auto gain_bitsolve = [&sum,&mantissa,this]() -> void {
		int i__ = 0;
		int x = 0;
		int cnt = 7;
		memset(this->ptrhead,0,strflt::precision);
		if(sum.size() == 1){
			this->index = 0;
			goto code_gain_mantissa_piece;
		}
		if(sum.size() == 0){ // no int
			int i = 0;
			while(mantissa[i] == 0)
				i++;
			this->index = -i-1;
			i__ = i+1;
			goto code_gain_mantissa_piece;
		}

		// int piece
		for(int i=1;i<sum.size();i++){
			unsigned char *nowbyte = (unsigned char*)this->ptrhead + x;
			int k;
			k = sum[i] << cnt;
			*nowbyte |= k;
			cnt--;
			if(cnt == -1){
				cnt = 7;
				if(i != sum.size()-1)
					x++;
			}
		}
		this->index = sum.size() - 1;
		this->lenth = min(x + 1 , strflt::precision);
		if(this->lenth == strflt::precision)
			return;
		if(cnt == 7)
			x++;

		// mantissa piece
		code_gain_mantissa_piece:
		if(mantissa.size() == 0)
			return;
		for(int i=i__;i<mantissa.size();i++){
			unsigned char *nowbyte = (unsigned char*)this->ptrhead + x;
			int k;
			k = mantissa[i] << cnt;
			*nowbyte |= k;
			cnt--;
			if(cnt == -1){
				if(i == mantissa.size() -1)
					break;
				cnt = 7;
				x++;
			}
		}
		if(x == 0 && cnt == 7)
			this->lenth = 0;
		else
			this->lenth = min(x + 1,strflt::precision);
		return;
	};

	//lambda which can put bits into vector
	auto put_vector = [](vector<int> &vec,int number,int bits) -> void {
		for(int i = 1<<(number-1);i!=0;i>>=1){
			if(i & bits)
				vec.push_back(1);
			else
				vec.push_back(0);
		}
		return;
	};

	//for 2 :
	if(nms == 2){
		gain_bitsolve();
		while(this->lenth && !(*this)[this->lenth-1])
			this->lenth--;
		return;
	}

	//for 8 :
	if(nms == 8){
		vector<int> a,b;
		a = sum;
		b = mantissa;
		sum.clear();
		mantissa.clear();
		for(int i=0;i<a.size();i++)
			put_vector(sum,3,a[i]);
		int j=0;
		while(j != sum.size() && sum[j] == 0)
			j++;
		if(j == sum.size())
			sum.clear();
		else if(j != 0){
			for(int i=j;i<sum.size();i++)
				sum[i-j] = sum[i];
			for(int i=1;i<=j;i++)
				sum.pop_back();
		}
		for(int i=0;i<b.size();i++)
			put_vector(mantissa,3,b[i]);
		while(mantissa.size() && mantissa[mantissa.size()-1] == 0)
			mantissa.pop_back();
		gain_bitsolve();
		while(this->lenth && !(*this)[this->lenth-1])
			this->lenth--;
		return;
	}

	//for 16 :
	if(nms == 16){
		vector<int> a,b;
		a = sum;
		b = mantissa;
		sum.clear();
		mantissa.clear();
		for(int i=0;i<a.size();i++)
			put_vector(sum,4,a[i]);
		int j=0;
		while(j != sum.size() && sum[j] == 0)
			j++;
		if(j == sum.size())
			sum.clear();
		else if(j != 0){
			for(int i=j;i<sum.size();i++)
				sum[i-j] = sum[i];
			for(int i=1;i<=j;i++)
				sum.pop_back();
		}
		for(int i=0;i<b.size();i++)
			put_vector(mantissa,4,b[i]);
		while(mantissa.size() && mantissa[mantissa.size()-1] == 0)
			mantissa.pop_back();
		gain_bitsolve();
		while(this->lenth && !(*this)[this->lenth-1])
			this->lenth--;
		return;
	}

	//for 10:
	if(nms == 10){
		vector<int> a,b;
		a = sum;
		b = mantissa;
		sum.clear();
		mantissa.clear();

		// int piece
		if(a.size()){
			vector<int> model;
			int abegin = 0;
			while(a.size() != abegin){
				for(int i=0;i<a.size();i++){
					if(i == a.size()-1)
						model.push_back(a[i] & 1);
					else
						a[i+1] += 10 * (a[i] & 1);
					a[i] >>= 1;
				}
				while(a[abegin] == 0 && abegin != a.size())
					abegin++;
			}
			reverse(model.begin(),model.end());
			sum = model;
		}

		// mantissa piece
		if(b.size()){
			bool flag;
			while(1){
				flag = false;
				for(int i=0;i<b.size();i++){
					if(i == 0){
						if(b[i] != 0)
							flag = true;
						b[i] <<= 1;
						if(b[i] >= 10){
							b[i] -= 10;
							mantissa.push_back(1);
						}else mantissa.push_back(0);
					}else{
						if(b[i] != 0)
							flag = true;
						b[i] <<= 1;
						if(b[i] >= 10){
							b[i] -= 10;
							b[i-1]++;
						}
					}
				}
				if(!flag)
					break;
				if((mantissa.size() >> 3) >= strflt::precision)
					break;
			}
			while(mantissa.size() && mantissa[mantissa.size()-1] == 0)
				mantissa.pop_back();
		}

		gain_bitsolve();
		while(this->lenth && !(*this)[this->lenth-1])
			this->lenth--;
		return;
	}
	return;
}

//print it (out)
void strflt::sout(int numSystemMode){
	if(numSystemMode!=2&&numSystemMode!=8&&numSystemMode!=10&&numSystemMode!=16){
		error(1);
	}
	if(this->is_0){
		printf("0");
		return;
	}
	if(this->sign){
		printf("-");
	}

	//this is a lambda which can pick the needed bits
	auto pickbit = [](unsigned char thebyte,int highbit,int lenth) -> int {
		int lowbit = highbit - lenth + 1;
		if(lowbit < 0)
			return 0;
		int k = (1 << (highbit + 1)) - 1;
		k -= (1 << lowbit) - 1;
		return (thebyte & k) >> lowbit;
	};

	//for 2:
	if(numSystemMode == 2){
		printf("0b");
		if(this->index < 0){
			printf("0.");
			for(int i=-2;i>=this->index;i--)
				printf("0");
		}
		printf("1");
		if(!this->lenth){
			for(int i=1;i<=this->index;i++)
				printf("0");
			return;
		}
		if(this->index == 0)
			printf(".");
		int cnt = 1;
		for(int i=0;i<=this->lenth-2;i++){
			unsigned char *nowptr = (unsigned char*)this->ptrhead + i;
			for(int j=7;j>=0;j--){
				if(*nowptr & (1 << j))
					printf("1");
				else
					printf("0");
				if(cnt == this->index)
					printf(".");
				cnt++;
			}
		}
		
		//delete 0
		int jmin = 7;
		for(int i=0;i<=7;i++){
			if((1 << i) & *((unsigned char*)this->ptrhead + this->lenth - 1)){
				jmin = i;
				break;
			}
		}
		for(int j=7;j>=jmin;j--){
			if(*((unsigned char*)this->ptrhead + this->lenth - 1) & (1 << j))
				printf("1");
			else
				printf("0");
			if(cnt == this->index && j != jmin)
				printf(".");
			cnt++;
		}

		//pick 0
		while(cnt++ <= this->index)
			printf("0");
		return;
	}

	//for 8:
	if(numSystemMode == 8){
		printf("0");
		*((char*)this->ptrhead + this->lenth) = 0;
		vector<int> sum,mantissa;
		int sumbyte = -1;
		int sumbit = -1;
		int mantissabyte = -1;
		int mantissabit = -1;
		
		if(this->index < 0){
			printf("0.");
			int k = (-this->index-1) / 3;
			for(int i=1;i<=k;i++)
				printf("0");
			if(this->lenth == 0){
				switch((-this->index-1) % 3){
				case 1:
					printf("2");
					break;
				case 2:
					printf("1");
					break;
				default:
					printf("4");
					break;
				}
				return;
			}
			switch((-this->index-1) % 3){
			case 1:
				mantissa.push_back(2 | ((*(unsigned char*)this->ptrhead & 128) >> 7));
				mantissabyte = 0;
				mantissabit = 6;
				break;
			case 2:
				mantissa.push_back(1);
				mantissabyte = 0;
				mantissabit = 7;
				break;
			default:
				mantissa.push_back(4 | ((*(unsigned char*)this->ptrhead & 192) >> 6));
				mantissabyte = 0;
				mantissabit = 5;
				break;
			}
		}

		if(this->index == 0){
			if(this->lenth == 0){
				printf("1");
				return;
			}
			printf("1.");
			mantissabyte = 0;
			mantissabit = 7;
		}

		if(this->index > 0){
			if(this->lenth == 0){
				switch(this->index % 3){
				case 1:
					printf("2");
					break;
				case 2:
					printf("4");
					break;
				default:
					printf("1");
					break;
				}
				int k = this->index / 3;
				for(int i=1;i<=k;i++)
					printf("0");
				return;
			}
			mantissabyte = this->index >> 3;
			mantissabit = 7 - (this->index & 7);
			if(mantissabit == 7){
				sumbyte = mantissabyte -1;
				sumbit = 0;
			}else{
				sumbyte = mantissabyte;
				sumbit = mantissabit +1;
			}
		}

		//mantissa piece
		while(mantissabyte != this->lenth){
			unsigned char nowbyte = *((unsigned char*)this->ptrhead + mantissabyte);
			if(mantissabit == 0){
				int k = nowbyte & 1;
				k <<= 2;
				unsigned char nxtbyte = *((unsigned char*)this->ptrhead + mantissabyte + 1);
				k |= ((nxtbyte & 192) >> 6);
				mantissa.push_back(k);
				mantissabyte++;
				mantissabit = 5;
				continue;
			}else if(mantissabit == 1){
				int k = nowbyte & 3;
				k <<= 1;
				unsigned char nxtbyte = *((unsigned char*)this->ptrhead + mantissabyte + 1);
				k |= ((nxtbyte & 128) >> 7);
				mantissa.push_back(k);
				mantissabyte++;
				mantissabit = 6;
				continue;
			}else{
				mantissa.push_back(pickbit(nowbyte,mantissabit,3));
				mantissabit -= 3;
				if(mantissabit == -1){
					mantissabyte++;
					mantissabit = 7;
				}
				continue;
			}
		}

		//int piece
		while(sumbyte != -1){
			unsigned char nowbyte = *((unsigned char*)this->ptrhead + sumbyte);
			if(sumbit == 6){
				int k = pickbit(nowbyte,7,2);
				if(sumbyte == 0){
					k |= 4;
					sum.push_back(k);
					break;
				}
				unsigned char nxtbyte = *((unsigned char*)this->ptrhead + sumbyte - 1);
				k |= ((nxtbyte & 1) << 2);
				sum.push_back(k);
				sumbyte--;
				sumbit = 1;
				continue;
			}else if(sumbit == 7){
				int k = pickbit(nowbyte,7,1);
				if(sumbyte == 0){
					k |= 2;
					sum.push_back(k);
					break;
				}
				unsigned char nxtbyte = *((unsigned char*)this->ptrhead + sumbyte - 1);
				k |= ((nxtbyte & 3) << 1);
				sum.push_back(k);
				sumbyte--;
				sumbit = 2;
				continue;
			}else{
				sum.push_back(pickbit(nowbyte,sumbit + 2,3));
				sumbit += 3;
				if(sumbit == 8){
					sumbyte--;
					sumbit = 0;
					if(sumbyte == -1){
						sum.push_back(1);
						break;
					}
				}
			}
		}
		
		for(int i=mantissa.size()-1;i>=0;i--){
			if(mantissa[i] == 0)
				mantissa.pop_back();
			else
				break;
		}

		if(sum.size()){
			reverse(sum.begin(),sum.end());
			printVector(sum);
			if(mantissa.size())
				printf(".");
		}
		if(mantissa.size())
			printVector(mantissa);
		return;
	}

	//for 16:
	if(numSystemMode == 16){
		printf("0x");
		*((char*)this->ptrhead + this->lenth) = 0;
		vector<int> sum,mantissa;
		int sumbyte = -1;
		int sumbit = -1;
		int mantissabyte = -1;
		int mantissabit = -1;

		if(this->index < 0){
			printf("0.");
			int k = (-this->index-1) >> 2;
			for(int i=1;i<=k;i++)
				printf("0");
			if(this->lenth == 0){
				switch((-this->index-1) & 3){
				case 1:
					printf("4");
					break;
				case 2:
					printf("2");
					break;
				case 3:
					printf("1");
					break;
				default:
					printf("8");
					break;
				}
				return;
			}
			switch((-this->index-1) & 3){
			case 1:
				mantissa.push_back(4 | ((*(unsigned char*)this->ptrhead & 192) >> 6));
				mantissabyte = 0;
				mantissabit = 5;
				break;
			case 2:
				mantissa.push_back(2 | ((*(unsigned char*)this->ptrhead & 128) >> 7));
				mantissabyte = 0;
				mantissabit = 6;
				break;
			case 3:
				mantissa.push_back(1);
				mantissabyte = 0;
				mantissabit = 7;
				break;
			default:
				mantissa.push_back(8 | ((*(unsigned char*)this->ptrhead & 224) >> 5));
				mantissabyte = 0;
				mantissabit = 4;
				break;
			}
		}

		if(this->index == 0){
			if(this->lenth == 0){
				printf("1");
				return;
			}
			printf("1.");
			mantissabyte = 0;
			mantissabit = 7;
		}

		if(this->index > 0){
			if(this->lenth == 0){
				switch(this->index & 3){
				case 1:
					printf("2");
					break;
				case 2:
					printf("4");
					break;
				case 3:
					printf("8");
					break;
				default:
					printf("1");
					break;
				}
				int k = this->index >> 2;
				for(int i=1;i<=k;i++)
					printf("0");
				return;
			}
			mantissabyte = this->index >> 3;
			mantissabit = 7 - (this->index & 7);
			if(mantissabit == 7){
				sumbyte = mantissabyte -1;
				sumbit = 0;
			}else{
				sumbyte = mantissabyte;
				sumbit = mantissabit +1;
			}
		}

		//mantissa piece
		while(mantissabyte != this->lenth){
			unsigned char nowbyte = *((unsigned char*)this->ptrhead + mantissabyte);
			unsigned char nxtbyte = *((unsigned char*)this->ptrhead + mantissabyte + 1);
			if(mantissabit == 2){
				int k = (nowbyte & 7);
				k <<= 1;
				k |= (pickbit(nxtbyte,7,1));
				mantissa.push_back(k);
				mantissabit = 6;
				mantissabyte++;
				continue;
			}else if(mantissabit == 1){
				int k = (nowbyte & 3);
				k <<= 2;
				k |= (pickbit(nxtbyte,7,2));
				mantissa.push_back(k);
				mantissabit = 5;
				mantissabyte++;
				continue;
			}else if(mantissabit == 0){
				int k = (nowbyte & 1);
				k <<= 3;
				k |= (pickbit(nxtbyte,7,3));
				mantissa.push_back(k);
				mantissabit = 4;
				mantissabyte++;
				continue;
			}else{
				mantissa.push_back(pickbit(nowbyte,mantissabit,4));
				mantissabit -= 4;
				if(mantissabit == -1){
					mantissabyte++;
					mantissabit = 7;
				}
				continue;
			}
		}

		//int piece
		while(sumbyte != -1){
			unsigned char nowbyte = *((unsigned char*)this->ptrhead + sumbyte);
			unsigned char nxtbyte = *((unsigned char*)this->ptrhead + sumbyte - 1);
			if(sumbit == 5){
				int k = pickbit(nowbyte,7,3);
				if(sumbyte == 0){
					k |= 8;
					sum.push_back(k);
					break;
				}
				k |= ((nxtbyte & 1) << 3);
				sum.push_back(k);
				sumbit = 1;
				sumbyte--;
			}else if(sumbit == 6){
				int k = pickbit(nowbyte,7,2);
				if(sumbyte == 0){
					k |= 4;
					sum.push_back(k);
					break;
				}
				k |= ((nxtbyte & 3) << 2);
				sum.push_back(k);
				sumbit = 2;
				sumbyte--;
			}else if(sumbit == 7){
				int k = pickbit(nowbyte,7,1);
				if(sumbyte == 0){
					k |= 2;
					sum.push_back(k);
					break;
				}
				k |= ((nxtbyte & 7) << 1);
				sum.push_back(k);
				sumbit = 3;
				sumbyte--;
			}else{
				sum.push_back(pickbit(nowbyte,sumbit + 3,4));
				sumbit += 4;
				if(sumbit == 8){
					if(sumbyte == 0){
						sum.push_back(1);
						break;
					}
					sumbit = 0;
					sumbyte--;
				}
			}
		}

		for(int i=mantissa.size()-1;i>=0;i--){
			if(mantissa[i] == 0)
				mantissa.pop_back();
			else
				break;
		}

		if(sum.size()){
			reverse(sum.begin(),sum.end());
			printVector(sum);
			if(mantissa.size())
				printf(".");
		}
		if(mantissa.size())
			printVector(mantissa);
		return;
	}

	//for 10:
	if(numSystemMode == 10){
		vector<unsigned char> mantissa;
		*((char*)this->ptrhead + this->lenth) = 0;

		//this is a lambda which can give numbers to vector mantissa
		auto put_mantissa = [&mantissa,this](void *begin,int ned) -> void {
			unsigned char *endptr = (unsigned char*)this->ptrhead + this->lenth;
			for(unsigned char *ptr = (unsigned char*)begin; ptr < endptr; ptr++){
				unsigned char k = *ptr;
				unsigned char q = *(ptr + 1);
				k <<= ned;
				q >>= (8 - ned);
				k |= q;
				mantissa.push_back(k);
			}
			return;
		};

		if(this->index == 0){
			if(this->lenth == 0){
				printf("1");
				return;
			}
			printf("1.");
			put_mantissa(this->ptrhead,0);
		}

		if(this->index < 0){
			printf("0.");
			// how many bits this byte need in next byte
			int cnt = 0;
			int k = (-this->index - 1) >> 3;
			for(int i=1;i<=k;i++)
				mantissa.push_back(0);
			k = (-this->index - 1) & 7;
			cnt = 8 - k - 1;
			mantissa.push_back((1 << cnt) | (*(unsigned char*)this->ptrhead >> (8 - cnt)));
			if(this->lenth != 0)
				put_mantissa(this->ptrhead,cnt);
		}

		if(this->index > 0){
			strint sint;
			sint = this->strc_strint();
			sint.sign = false;
			sint.sout(10);
			int k = this->index >> 3;
			int cnt = this->index & 7;
			if(!this->lenth || this->lenth == k || (this->lenth-1 == k && !((*((unsigned char*)this->ptrhead + k) << cnt) & 255)))
				return;
			printf(".");
			put_mantissa((char*)this->ptrhead + k,cnt);
		}

		//it can record the bit's 10 number
		vector<int> flt;
		flt.push_back(5);

		//it is a lambda which can devide flt to 2
		auto fltdev = [&flt]() -> void {
			int i=0;
			while(i != flt.size()){
				if(flt[i] & 1){
					if(i == flt.size()-1)
						flt.push_back(10);
					else
						flt[i+1] += 10;
				}
				flt[i] >>= 1;
				i++;
			}
			return;
		};

		//it records the answer
		vector<int> ans;

		//it is a lambda which can plus the answer to the flt vector
		auto ansplus = [&ans,&flt]() -> void {
			for(int i=0;i<flt.size();i++){
				if(i >= ans.size())
					ans.push_back(flt[i]);
				else
					ans[i] += flt[i];
			}
			for(int i=ans.size()-1;i>=0;i--){
				if(ans[i] >= 10){
					ans[i] -= 10;
					ans[i-1]++;
				}
			}
			return;
		};

		//solve mantissa
		for(int i=0;i<mantissa.size();i++){
			for(int j=7;j>=0;j--){
				if(mantissa[i] & (1 << j))
					ansplus();
				fltdev();
			}
		}

		for(int i=0;i<ans.size();i++){
			printf("%d",ans[i]);
		}
		return;
	}
	return;
}

//give the value
void strflt::operator=(const strflt &x){
	this->clean();
	this->index = x.index;
	this->is_0 = x.is_0;
	this->lenth = x.lenth;
	this->sign = x.sign;
	if(!this->is_0 && this->lenth)
		for(int i=0;i<this->lenth;i++)
			*((unsigned char*)this->ptrhead + i) = *((unsigned char*)x.ptrhead + i);
	return;
}
void strflt::operator=(strflt &&x){
	this->clean();
	this->index = x.index;
	this->is_0 = x.is_0;
	this->lenth = x.lenth;
	this->sign = x.sign;
	if(this->ptrhead)
		free(this->ptrhead);
	this->ptrhead = x.ptrhead;
	x.ptrhead = NULL;
	return;
}
void strflt::operator=(long long x){
	this->clean();
	if(x == 0){
		this->sign = false;
		this->lenth = 0;
		this->is_0 = true;
		this->index = 0;
		return;
	}else this->is_0 = false;

	if(x < 0){
		this->sign = true;
		x--;
		x = ~x;
	}else{
		this->sign = false;
	}

	int highbit,lowbit;
	for(int i=63;i>=0;i--){
		if((1ULL << i) & x){
			this->index = i;
			highbit = i;
			break;
		}
	}
	for(int i=0;i<=63;i++){
		if((1ULL << i) & x){
			lowbit = i;
			break;
		}
	}
	printf("\nh : %d\nl : %d\n",highbit,lowbit);
	if(highbit == lowbit){
		this->lenth = 0;
		return;
	}
	this->lenth = ((highbit - lowbit -1) >> 3) + 1;
	printf("lenth : %d\n",this->lenth);
	x <<= (64 - highbit);
	for(int i=0;i<lenth;i++){
		unsigned char *nowbyte = (unsigned char*)this->ptrhead + i;
		*nowbyte = (x & 0xff00000000000000ull) >> 56;
		x <<= 8;
	}
	return;
}
void strflt::operator=(float x){
	this->clean();
	void *vp = &x;
	unsigned int num = *(int*)vp;
	if(x == 0){
		this->is_0 = true;
		this->lenth = 0;
		this->sign = false;
		this->index = 0;
		return;
	}else this->is_0 = false;
	
	//give index
	unsigned int k1 = num;
	k1 <<= 1;
	k1 >>= 24;
	int k2 = k1;
	k2 -= 127;
	this->index = k2;

	//give mantissa
	unsigned int k3 = num;
	k3 <<= 9;
	k3 >>= 9;
	if(x < 0)
		this->sign = true;
	else 
		this->sign = false;
	
	this->lenth = 3;
	k3 <<= 1;
	*((unsigned char*)this->ptrhead) = ((k3 & 0xff0000) >> 16);
	*((unsigned char*)this->ptrhead + 1) = ((k3 & 0xff00) >> 8);
	*((unsigned char*)this->ptrhead + 2) = (k3 & 0xff);
	return;
}
void strflt::operator=(double x){
	this->clean();
	void *vp = &x;
	unsigned long long num = *(long long*)vp;
	if(x == 0){
		this->is_0 = true;
		this->lenth = 0;
		this->sign = false;
		this->index = 0;
		return;
	}else this->is_0 = false;
	
	//give index
	unsigned long long k1 = num;
	k1 <<= 1;
	k1 >>= 53;
	long long k2 = k1;
	k2 -= 1023;
	this->index = k2;

	//give mantissa
	unsigned long long k3 = num;
	k3 <<= 12;
	k3 >>= 12;
	if(x < 0)
		this->sign = true;
	else 
		this->sign = false;
	
	this->lenth = 7;
	k3 <<= 4;
	*((unsigned char*)this->ptrhead + 0) = ((k3 & 0x00ff000000000000ull) >> 48);
	*((unsigned char*)this->ptrhead + 1) = ((k3 & 0x0000ff0000000000ull) >> 40);
	*((unsigned char*)this->ptrhead + 2) = ((k3 & 0x000000ff00000000ull) >> 32);
	*((unsigned char*)this->ptrhead + 3) = ((k3 & 0x00000000ff000000ull) >> 24);
	*((unsigned char*)this->ptrhead + 4) = ((k3 & 0x0000000000ff0000ull) >> 16);
	*((unsigned char*)this->ptrhead + 5) = ((k3 & 0x000000000000ff00ull) >> 8 );
	*((unsigned char*)this->ptrhead + 6) = ((k3 & 0x00000000000000ffull) >> 0 );
	return;
}
strint strflt::strc_strint(){
	strint ans;
	if(this->is_0){
		ans.sign = false;
		ans.lenth = 1;
		*(unsigned char*)ans.ptrhead = 0;
		return ans;
	}
	ans.sign = false;
	ans.lenth = this->lenth + 1;
	for(int i=0;i<this->lenth;i++)
		*((unsigned char*)ans.ptrhead + i) = *((unsigned char*)this->ptrhead + this->lenth - i - 1);
	*((unsigned char*)ans.ptrhead + this->lenth) = 1;
	ans >>= (ans.log2() - this->index);
	ans.sign = this->sign;
	return ans;
}
long long strflt::stoi(){
	strint x;
	x = this->strc_strint();
	long long ans = x.stoi();
	return ans;
}
float strflt::stof(){
	if(this->is_0)
		return 0.0;
	int ansi = 0;
	int idx = this->index;
	idx += 127;
	idx &= 255;
	idx <<= 23;
	int imax = min(3,this->lenth);
	for(int i=0;i<imax;i++){
		int nowbyte = *((unsigned char*)this->ptrhead + i);
		nowbyte <<= ((2 - i) << 3);
		ansi |= nowbyte;
	}
	ansi >>= 1;
	ansi |= idx;
	void *vp = &ansi;
	float ans = *(float*)vp;
	if(this->sign)
		ans = -ans;
	return ans;
}
double strflt::stod(){
	if(this->is_0)
		return 0.0;
	long long ansi = 0;
	long long idx = this->index;
	idx += 1023;
	idx &= 2047;
	idx <<= 52;
	int imax = min(7,this->lenth);
	for(int i=0;i<imax;i++){
		long long nowbyte = *((unsigned char*)this->ptrhead + i);
		nowbyte <<= ((6 - i) << 3);
		ansi |= nowbyte;
	}
	ansi >>= 4;
	ansi |= idx;
	void *vp = &ansi;
	double ans = *(double*)vp;
	if(this->sign)
		ans = -ans;
	return ans;
}

//some lojic operations
bool operator==(const strflt &x,const strflt &y){
	if(x.is_0){
		if(y.is_0)
			return true;
		else if(y.index < 0 && -y.index >= strflt::deviation)
			return true;
		else
			return false;
	}else if(y.is_0){
		if(x.index < 0 && -x.index >= strflt::deviation)
			return true;
		else
			return false;
	}else if(x.index < 0 && -x.index >= strflt::deviation){
		if(y.index < 0 && -y.index >= strflt::deviation)
			return true;
		else
			return false;
	}else if(y.index < 0 && -y.index >= strflt::deviation){
		return false;
	}

	if(x.sign != y.sign)
		return false;

	if(x.index != y.index)
		return false;
	
	if(x.lenth != y.lenth && ((x.lenth << 3) < strflt::deviation || (y.lenth << 3) < strflt::deviation))
		return false;

	for(int i=0;i<min(x.lenth,(strflt::deviation >> 3));i++){
		unsigned char xbyte = *((unsigned char*)x.ptrhead + i);
		unsigned char ybyte = *((unsigned char*)y.ptrhead + i);
		if(xbyte != ybyte)
			return false;
	}

	return true;
}
bool operator!=(const strflt &x,const strflt &y){
	return !(x == y);
}
bool operator>(const strflt &x,const strflt &y){
	if(x.is_0){
		if(y.is_0)
			return false;
		else
			return y.sign;
	}else if(y.is_0){
		return !x.sign;
	}

	if(x.sign != y.sign)
		return y.sign;

	if(x.index != y.index)
		return (x.index > y.index);

	for(int i=0;i<min(x.lenth,y.lenth);i++){
		unsigned char xbyte = *((unsigned char*)x.ptrhead + i);
		unsigned char ybyte = *((unsigned char*)y.ptrhead + i);
		if(xbyte > ybyte)
			goto absRetBig;
		if(xbyte < ybyte)
			goto absRetSmall;
	}
	return false;

	absRetBig: return !x.sign;
	absRetSmall: return x.sign;
}
bool operator<(const strflt &x,const strflt &y){
	if(x.is_0){
		if(y.is_0)
			return false;
		else
			return !y.sign;
	}else if(y.is_0){
		return x.sign;
	}

	if(x.sign != y.sign)
		return x.sign;

	if(x.index != y.index)
		return (x.index < y.index);

	for(int i=0;i<min(x.lenth,y.lenth);i++){
		unsigned char xbyte = *((unsigned char*)x.ptrhead + i);
		unsigned char ybyte = *((unsigned char*)y.ptrhead + i);
		if(xbyte > ybyte)
			goto absRetBig;
		if(xbyte < ybyte)
			goto absRetSmall;
	}
	return false;

	absRetBig: return x.sign;
	absRetSmall: return !x.sign;
}
bool operator>=(const strflt &x,const strflt &y){
	return (x > y || x == y);
}
bool operator<=(const strflt &x,const strflt &y){
	return (x < y || x == y);
}

//some maths operations

//unsigned plus 
strflt unsigned_plus(const strflt &in1,const strflt &in2){
	strflt x = in1, y = in2;
	x.sign = false;
	y.sign = false;
	
	//buildmsg
	unsigned char *ptr = (unsigned char*)malloc(strnum_inf);
	memset(ptr,0,strnum_inf);
	unsigned char *msg = ptr + (strnum_inf - 5);
	unsigned char *msgtop = msg;

	//push to the last bytes
	if((x.lenth << 3) - x.index < (y.lenth << 3) - y.index)
		swap(x,y);
	for(int i=x.lenth-1;i>=0;i--)
		*--msgtop = *((unsigned char*)x.ptrhead + i);
	*--msgtop = 1;
	unsigned char *pointbyte = msgtop; 
	
	//plus
	int nowbit = (x.lenth << 3) - x.index - (y.lenth << 3) + y.index;
	int nowbyte = nowbit >> 3;
	nowbit &= 7;
	bool flag = false;
	//a lambda which can plus y to msg
	auto addmsg = [&nowbyte,&nowbit,&flag,&msgtop,&msg,&y](int ybyte,int ybit) mutable -> void {
		int k = 1;
		k <<= ybit;
		if(ybyte == -1 || k & *((unsigned char*)y.ptrhead + ybyte)){
			int q = 1;
			q <<= nowbit;
			while(q & *(msg - nowbyte - 1) && q != 256){
				*(msg - nowbyte - 1) ^= q;
				q <<= 1;
			}
			if(q == 256)
				flag = true;
			else
				*(msg - nowbyte - 1) ^= q;
		}
		nowbit++;
		if(nowbit == 8){
			nowbyte++;
			nowbit = 0;
			if(flag == true){
				flag = false;
				int q = 1;
				while(q & *(msg - nowbyte - 1) && q != 256){
					*(msg - nowbyte - 1) ^= q;
					q <<= 1;
				}
				if(q == 256)
					flag = true;
				else
					*(msg - nowbyte - 1) ^= q;
			}
		}
		return;
	};
	//plus y
	for(int i=y.lenth-1;i>=0;i--)
		for(int j=0;j<=7;j++)
			addmsg(i,j);
	addmsg(-1,0);
	while(flag == true){
		flag = false;
		nowbyte++;
		int q = 1;
		while(q & *(msg - nowbyte - 1) && q != 256){
			*(msg - nowbyte - 1) ^= q;
			q <<= 1;
		}
		if(q == 256)
			flag = true;
		else
			*(msg - nowbyte - 1) ^= q;
	}

	//pick msg into ans
	for(unsigned char *ptri = ptr; ptri < msg; ptri++){
		if(*ptri){
			msgtop = ptri;
			break;
		}
	}
	strflt ans;
	ans.clean();
	ans.is_0 = false;
	ans.sign = false;
	//solve index (msgtop <= pointbyte)
	ans.index = 0;
	for(int i=7;i>=0;i--){
		if((1 << i) & *msgtop){
			ans.index = i;
			break;
		}
	}
	ans.index += ((pointbyte - msgtop) << 3);
	ans.index += x.index;
	//solve lenth
	int needbit;
	for(int i=7;i>=0;i--){
		if((1 << i) & *msgtop){
			needbit = (8 - i);
			break;
		}
	}
	if(needbit == 0){
		ans.lenth = msg - msgtop - 1;
		for(int i=0;i<ans.lenth;i++)
			*((unsigned char*)ans.ptrhead + i) = *(msgtop + i + 1);
	}else{
		ans.lenth = msg - msgtop;
		for(int i=0;i<ans.lenth;i++){
			unsigned char nowbyte_ = *(msgtop + i);
			unsigned char nxtbyte_ = *(msgtop + i + 1);
			nowbyte_ <<= needbit;
			nxtbyte_ >>= (8 - needbit);
			*((unsigned char*)ans.ptrhead + i) = nowbyte_ | nxtbyte_;
		}
	}
	ans.lenth = min(ans.lenth,strflt::precision);
	while(ans.lenth && *((unsigned char*)ans.ptrhead + ans.lenth - 1) == 0)
		ans.lenth--;

	free(ptr);
	return ans;
}

//unsigned subtract 
strflt unsigned_subtract(const strflt &in1,const strflt &in2){
	strflt x = in1, y = in2;
	x.sign = false;
	y.sign = false;
	bool finsign = false;
	if(x == y){
		strflt _0;
		_0.clean();
		_0.is_0 = true;
		return _0;
	}else if(x < y){
		finsign = true;
		swap(x,y);
	}

	//buildmsg and pick byte
	byte *ptr = (byte*)malloc(strnum_inf);
	memset(ptr,0,strnum_inf);
	byte *msg = ptr;
	byte *msgtop = msg;
	*msg = 1;
	for(int i=0;i<x.lenth;i++)
		*++msgtop = x[i];

	//subtract
	int nowbit = x.index - y.index + (y.lenth << 3) + 7;
	int nowbyte = nowbit >> 3;
	nowbit &= 7;
	nowbit = 7 - nowbit;
	bool flag = false;
	//a lambda which can subtract y to msg
	auto submsg = [&nowbyte,&nowbit,&flag,&msgtop,&msg,&y](int ybyte,int ybit) mutable -> void {
		int k = 1;
		k <<= ybit;
		if(ybyte == -1 || k & y[ybyte]){
			int q = 1;
			q <<= nowbit;
			while(q & (~msg[nowbyte]) && q != 256){
				msg[nowbyte] ^= q;
				q <<= 1;
			}
			if(q == 256)
				flag = true;
			else
				msg[nowbyte] ^= q;
		}
		nowbit++;
		if(nowbit == 8){
			nowbit = 0;
			nowbyte--;
			if(flag == true){
				flag = false;
				int q = 1;
				while(q & (~msg[nowbyte]) && q != 256){
					msg[nowbyte] ^= q;
					q <<= 1;
				}
				if(q == 256)
					flag = true;
				else
					msg[nowbyte] ^= q;
			}
		}
		return;
	};
	//plus y
	for(int i=y.lenth-1;i>=0;i--)
		for(int j=0;j<=7;j++)
			submsg(i,j);
	submsg(-1,0);
	while(flag == true){
		flag = false;
		nowbyte--;
		int q = 1;
		while(q & (~msg[nowbyte]) && q != 256){
			msg[nowbyte] ^= q;
			q <<= 1;
		}
		if(q == 256)
			flag = true;
		else
			msg[nowbyte] ^= q;
	}

	//pick msg into ans
	for(byte *ptri = ptr; ptri <= ptr + strnum_inf - 5; ptri++){
		if(*ptri){
			msg = ptri;
			break;
		}
	}
	for(byte *ptri = ptr + strnum_inf - 5; ptri >= ptr; ptri--){
		if(*ptri){
			msgtop = ptri;
			break;
		}
	}
	strflt ans;
	ans.clean();
	ans.is_0 = false;
	ans.sign = finsign;
	//solve idx
	ans.index = 0;
	for(int i=7;i>=0;i--){
		if(*msg & (1 << i)){
			ans.index = -(8 - i);
			break;
		}
	}
	ans.index -= (msg - ptr - 1) << 3;
	ans.index += x.index;
	//solve lenth
	int needbit = 0;
	for(int i=7;i>=0;i--){
		if(*msg & (1 << i)){
			needbit = (8 - i);
			break;
		}
	}
	if(needbit == 0){
		ans.lenth = msgtop - msg;
		for(int i=0;i<=ans.lenth;i++)
			ans[i] = msg[i+1];
	}else{
		ans.lenth = msgtop - msg + 1;
		for(int i=0;i<=ans.lenth;i++){
			byte nowbyte_ = msg[i];
			byte nxtbyte_ = msg[i+1];
			nowbyte_ <<= needbit;
			nxtbyte_ >>= (8 - needbit);
			ans[i] = nowbyte_ | nxtbyte_;
		}
	}
	ans.lenth = min(ans.lenth,strflt::precision);
	while(ans.lenth && !ans[ans.lenth-1])
		ans.lenth--;
	
	free(ptr);
	return ans;
}

//plus
strflt operator+(const strflt &x,const strflt &y){
	if(x.is_0)
		return y;
	if(y.is_0)
		return x;
	if(x.sign != y.sign){
		if(!x.sign)
			return unsigned_subtract(x,y);
		else
			return -unsigned_subtract(x,y);
	}else{
		if(!x.sign)
			return unsigned_plus(x,y);
		else
			return -unsigned_plus(x,y);
	}
}
void operator+=(strflt &x,const strflt &y){
	x = x + y;
	return;
}

//subtract
strflt operator-(const strflt &x,const strflt &y){
	if(x.is_0)
		return -y;
	if(y.is_0)
		return x;
	if(x.sign == y.sign){
		if(!x.sign)
			return unsigned_subtract(x,y);
		else
			return -unsigned_subtract(x,y);
	}else{
		if(!x.sign)
			return unsigned_plus(x,y);
		else
			return -unsigned_plus(x,y);
	}
}
void operator-=(strflt &x,const strflt &y){
	x = x - y;
	return;
}

//multy
strflt operator*(const strflt &in1,const strflt &in2){
	if(in1.is_0 || in2.is_0){
		strflt _0;
		_0 = 0.0;
		return _0;
	}
	bool finsign = (in1.sign != in2.sign);
	strflt x = in1, y = in2;
	x.sign = false;
	y.sign = false;
	if(x.lenth < y.lenth)
		swap(x,y);
	
	//buildmsg
	byte *ptr = (byte*)malloc(strnum_inf);
	memset(ptr,0,strnum_inf);
	byte *msg = ptr;
	byte *msgtop = msg;
	*msg = 1;
	for(int i=0;i<x.lenth;i++)
		*++msgtop = x[i];

	int nowbyte = 0;
	int nowbit = 0;	
	bool flag = false;

	//this is a lambda which can add moved x to msg
	auto addmsg = [&flag,&msg,&nowbyte,&nowbit,&x](int xbyte,int xbit) mutable -> void {
		int k = 1;
		k <<= xbit;
		if(xbyte == -1 || x[xbyte] & k){
			int q = 1;
			q <<= nowbit;
			while(q != 256 && msg[nowbyte] & q){
				msg[nowbyte] ^= q;
				q <<= 1;
			}
			if(q == 256)
				flag = true;
			else
				msg[nowbyte] ^= q;
		}
		nowbit++;
		if(nowbit == 8){
			nowbit = 0;
			nowbyte--;
			if(flag == true){
				flag = false;
				int q = 1;
				while(q != 256 && msg[nowbyte] & q){
					msg[nowbyte] ^= q;
					q <<= 1;
				}
				if(q == 256)
					flag = true;
				else
					msg[nowbyte] ^= q;
			}
		}
		return;
	};

	//this is a lambta which can move x and add it
	auto mulmsg = [&flag,&msg,&addmsg,&nowbyte,&nowbit,&x](int ybyte,int ybit) mutable -> void {
		nowbit = (8 - ybit) + (ybyte << 3) + (x.lenth << 3) + 7;
		nowbyte = nowbit >> 3;
		nowbit &= 7;
		nowbit = 7 - nowbit;
		for(int i=x.lenth-1;i>=0;i--)
			for(int j=0;j<=7;j++)
				addmsg(i,j);
		addmsg(-1,0);
		while(flag == true){
			flag = false;
			nowbyte--;
			int q = 1;
			while(q != 256 && msg[nowbyte] & q){
				msg[nowbyte] ^= q;
				q <<= 1;
			}
			if(q == 256)
				flag = true;
			else
				msg[nowbyte] ^= q;
		}
		return;
	}; 

	//multy
	for(int i=0;i<y.lenth;i++)
		for(int j=7;j>=0;j--)
			if(y[i] & (1 << j))
				mulmsg(i,j);

	//pick msg into ans
	for(byte *ptri = ptr + strnum_inf - 5; ptri >= ptr; ptri--){
		if(*ptri){
			msgtop = ptri;
			break;
		}
	}
	strflt ans;
	ans.clean();
	ans.is_0 = false;
	ans.sign = finsign;
	int needbit = 0;
	//solve idx and lenth
	for(int i=7;i>=0;i--){
		if(*msg & (1 << i)){
			ans.index = x.index + y.index + i;
			needbit = (8 - i);
			break;
		}
	}
	if(needbit == 0){
		ans.lenth = msgtop - msg;
		for(int i=0;i<ans.lenth;i++)
			ans[i] = msg[i+1];
	}else{
		ans.lenth = msgtop - msg + 1;
		for(int i=0;i<ans.lenth;i++){
			byte nowbyte_ = msg[i];
			byte nxtbyte_ = msg[i+1];
			nowbyte_ <<= needbit;
			nxtbyte_ >>= (8 - needbit);
			ans[i] = nowbyte_ | nxtbyte_;
		}
	}
	ans.lenth = min(ans.lenth,strflt::precision);
	while(ans.lenth && !ans[ans.lenth-1])
		ans.lenth--;

	free(ptr);
	return ans;
}
void operator*=(strflt &x,const strflt &y){
	x = x * y;
	return;
}

//devide
strflt operator/(const strflt &in1,const strflt &in2){
	if(in2.is_0)
		error(5);
	if(in1.is_0){
		strflt _0;
		_0 = 0.0;
		return _0;
	}

	bool finsign = (in1.sign != in2.sign);
	strflt x = in1, y = in2;
	x.sign = false;
	y.sign = false;

	//buildmsg
	byte *ptr = (byte*)malloc(strnum_inf);
	memset(ptr,0,strnum_inf);
	byte *msg = ptr;
	*msg = 1;
	for(int i=0;i<x.lenth;i++)
		ptr[i+1] = x[i];
	byte *ansptr = (byte*)malloc(strnum_inf);
	memset(ansptr,0,strnum_inf);

	int maxbyte = min(strnum_inf-5,strflt::precision);
	int nowbyte = 0;
	int nowbit = 0;
	bool flag = false;

	//this is a lambda which can judge if can sub in there
	auto isbig = [ptr,&msg,&y](int xbyte,int xbit) mutable -> bool {
		if(msg < ptr + xbyte)
			return true;
		if(msg > ptr + xbyte)
			return false;
		int needbit;
		for(int i=7;i>=0;i--){
			if(i < xbit)
				return false;
			if(ptr[xbyte] & (1 << i)){
				if(i > xbit)
					return true;
				else{
					needbit = (8 - i);
					break;
				}
			}
		}
		for(int i=0;i<y.lenth;i++){
			byte nowbyte_ = ptr[xbyte + i];
			byte nxtbyte_ = ptr[xbyte + i + 1];
			nowbyte_ <<= needbit;
			nxtbyte_ >>= (8 - needbit);
			if(y[i] < (nowbyte_ | nxtbyte_))
				return true;
			if(y[i] > (nowbyte_ | nxtbyte_))
				return false;
		}
		return true;
	};
	
	//this is a lambda which can subtract y to msg
	auto submsg = [ptr,&nowbyte,&nowbit,&y,&flag](int ybyte,int ybit) mutable -> void {
		int k = 1;
		k <<= ybit;
		if(ybyte == -1 || (y[ybyte] & k)){
			int q = 1;
			q <<= nowbit;
			while((q != 256) && (q & (~ptr[nowbyte]))){
				ptr[nowbyte] ^= q;
				q <<= 1;
			}
			if(q == 256)
				flag = true;
			else
				ptr[nowbyte] ^= q;
		}
		nowbit++;
		if(nowbit == 8){
			nowbit = 0;
			nowbyte--;
			if(flag == true){
				flag = false;
				int q = 1;
				while((q != 256) && (q & (~ptr[nowbyte]))){
					ptr[nowbyte] ^= q;
					q <<= 1;
				}
				if(q == 256)
					flag = true;
				else
					ptr[nowbyte] ^= q;
			}
		}
		return;
	};

	//this is a lambda which can devide y to msg
	auto devmsg = [&,ansptr,ptr](int xbyte,int xbit) mutable -> void {
		if(!isbig(xbyte,xbit))
			return;
		ansptr[xbyte] |= (1 << xbit);
		nowbit = (xbyte << 3) + (7 - xbit) + (y.lenth << 3);
		nowbyte = nowbit >> 3;
		nowbit &= 7;
		nowbit = (7 - nowbit);
		for(int i=y.lenth-1;i>=0;i--)
			for(int j=0;j<=7;j++)
				submsg(i,j);
		submsg(-1,0);
		while(flag == true){
			nowbyte--;
			flag = false;
			int q = 1;
			while((q != 256) && (q & (~ptr[nowbyte]))){
				ptr[nowbyte] ^= q;
				q <<= 1;
			}
			if(q == 256)
				flag = true;
			else
				ptr[nowbyte] ^= q;
		}
		while(!(*msg))
			msg++;
		return;
	};

	//devide
	devmsg(0,0);
	for(int i=1;(i <= maxbyte) && (msg <= ptr + maxbyte);i++)
		for(int j=7;j>=0;j--)
			devmsg(i,j);
	
	strflt ans;
	ans.clean();
	ans.sign = finsign;
	ans.is_0 = false;
	int beginbyte;
	int needbit;
	//solve index
	for(int i=0;i<=maxbyte;i++){
		if(i == maxbyte){
			ans.clean();
			ans = 0.0;
			free(ptr);
			free(ansptr);
			return ans;
		}
		for(int j=7;j>=0;j--){
			if(ansptr[i] & (1 << j)){
				ans.index = -((8 - j) + (i << 3) - 8);
				ans.index += x.index;
				ans.index -= y.index;
				beginbyte = i;
				needbit = (8 - j);
				goto for_break;
			}
		}
	}
	for_break:
	//solve lenth
	for(int i=strnum_inf-5;i>=0;i--){
		if(ansptr[i]){
			ans.lenth = i - beginbyte + 1;
			break;
		}
	}
	for(int i=0;i<ans.lenth;i++){
		byte nowbyte_ = ansptr[beginbyte + i];
		byte nxtbyte_ = ansptr[beginbyte + i + 1];
		nowbyte_ <<= needbit;
		nxtbyte_ >>= (8 - needbit);
		ans[i] = nowbyte_ | nxtbyte_;
	}
	ans.lenth = min(ans.lenth,strflt::precision);
	while(ans.lenth && !ans[ans.lenth-1])
		ans.lenth--;

	free(ptr);
	free(ansptr);
	return ans;
}
void operator/=(strflt &x,const strflt &y){
	x = x / y;
	return;
}

}