#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include <string>
#include "boost/unordered/unordered_map.hpp"
#include<vector>

using namespace std;

#ifndef split
#define split
struct stateActionPair{
	string state;
	string action;
};

struct nextStateStruct{
	string nextState;
	double probability;
	double reward;
};

struct eqStateActionPair{
	bool operator()(stateActionPair s1, stateActionPair s2) const;
};

struct eqstr{
	bool operator()(string s1, string s2) const;
};

size_t hash_value(stateActionPair const& s);

vector<string> split1(string str, char separator);
string removeBrackets(string word);
#endif

#ifndef ENV
#define ENV	

typedef boost::unordered_map<stateActionPair, vector<nextStateStruct>, boost::hash<stateActionPair>, struct eqStateActionPair> mapTypeNextState;
typedef boost::unordered_map<string, double, boost::hash<string>, eqstr> mapType;

class Environment{

	public:	
	string m_initialState, m_goalState;
        mapTypeNextState mapNextState;

	vector<nextStateStruct> getNextStates(string next, mapType mapReward);
	mapType getRewards(string rewards);
	
	Environment();
	void readConfig(string file);	
	
	nextStateStruct getNextStateAndReward(string state, string action);
		
};
#endif
