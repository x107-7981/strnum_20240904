#include <bits/stdc++.h>
#include "strnum.h"
using namespace std;
using namespace xxx;

//this file can teach you how to use strnum.h , you can also research by your self

static void printAll(strint *x){
	printf("lenth : %d\n",x->lenth);
	printf("outnms = 2 : ");
	x->soutln(2);
	printf("outnms = 8 : ");
	x->soutln(8);
	printf("outnms = 10 : ");
	x->soutln(10);
	printf("outnms = 16 : ");
	x->soutln(16);
}
static void printAll(strflt *x){
	printf("lenth : %d\n",x->lenth);
	printf("index : %d\n",x->index);
	printf("outnms = 2 : ");
	x->soutln(2);
	printf("outnms = 8 : ");
	x->soutln(8);
	printf("outnms = 10 : ");
	x->soutln(10);
	printf("outnms = 16 : ");
	x->soutln(16);
}

int main(){
	isErrorOpen = true;
#if 1
#if 1
	//strint test
	strint a,b;
	char mode = getchar();
	a.gain();
	if(mode == 'O'){printAll(&a);return 0;}
	b.gain();
	if(mode == '+') a += b;
	else if(mode == '-') a -= b;
	else if(mode == '*') a *= b;
	else if(mode == '/') a /= b;
	else if(mode == '%') a %= b;
	else {printf("ERRORMODE!");return 0;}
	printAll(&a);
	return 0;
#else
	//iostream test
	int b,c;
	strint a;
	cin >> b >> a >> c;
	cout << endl << b << endl << strnum_io2.operator[](a) << endl << strnum_io8[a] 
		<< endl << a << endl << strnum_io16[a] << endl << c << endl;
	return 0;
#endif
	////////////////////////////////////////////////////////////////////////////////////////////
#else
#if 1
	//strflt test
	char chr = getchar();
	strflt a,b;
	cin >> a >> b;
	printf("a : \n");
	printAll(&a);
	printf("b : \n");
	printAll(&b);
	clock_t time0 = clock();
	strflt c;
	switch(chr){
	case '+':
		c = a + b;
		break;
	case '-':
		c = a - b;
		break;
	case '*':
		c = a * b;
		break;
	case '/':
		c = a / b;
		break;
	default:
		printf("errormode!\n");
		return 0;
	}
	clock_t time1 = clock();
	printf("time : %d\n",time1 - time0);
	printf("c : \n");
	printAll(&c);
	return 0;
#else
#if 1
	//iterator test
	strint x;
	x.gain();
	for(auto it : x){
		printf("%x",it);
	}
	printf("\n");
	return 0;
#else
	//bitaddress test
	strint x,y;
	x.gain();
	y.gain();
	for(int i=y.lenth-1;i>=0;i--)
		for(int j=7;j>=0;j--)
			x(i,j) = (bool)y(i,j);
	cout << endl << strnum_io2[x] << endl;
	if(x(0,7))
		printf("TRUE__________\n");
	else
		printf("FALSE_________\n");
#endif
#endif
#endif
}
