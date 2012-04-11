#include<iostream>
#include<string>
using namespace std;
string *initialize();

int main(){
	string *ret=initialize();
	cout<<"hello"<<endl;	
}

string* initialize(){
	string a[]={"atul", "sandur"};
	string *p=a;
	int b=10;
	int *i=&b;
	cout<<*i<<endl;
	return p;
}
