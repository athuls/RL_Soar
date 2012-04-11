#include<limits>
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<vector>
#include <string>
#include <sstream>
#include <boost/unordered/unordered_map.hpp>
#include <string.h>
#define INF numeric_limits<float>::infinity()

using namespace std;

struct eqstr
{
  bool operator()(string s1, string s2) const
  {
	cout<<strcmp(s1.c_str(),s2.c_str())<<endl;
	return !s1.compare(s2);
  }
};


int main()
{
        boost::unordered_map<string, int, boost::hash<string>, eqstr> mapTest;

	mapTest["atul"]=1;
	mapTest["vaibhav"]=45;
	mapTest["krish"]=76;
	mapTest["god"]=45;
	cout<<mapTest["god"]<<endl;
}
