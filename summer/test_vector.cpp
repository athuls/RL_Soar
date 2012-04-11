#include<stdlib.h>
#include<vector>
#include<stdio.h>
#include <iostream>
using namespace std;

vector<int> modify()
{
	vector<int> ret_vec;
	ret_vec.push_back(3);
	ret_vec.push_back(4);
	ret_vec.push_back(5);
	return ret_vec;
}

int main()
{
	vector<int> get_return;
	get_return=modify();
	cout<<get_return.at(1)<<endl;
}
