#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "Constants.hpp"
#include "Environment.hpp"
#include "Agent.hpp"

using namespace std;
#ifndef mdp
#define mdp

class MDP
{
	
	public:
	ofstream fileToWrite;
	string *actions;
	string *nextState;
	string choice;
	int numStates;
	string initialState;
	string goalState;
	string higherProbability;
	string lowerProbability;	

	MDP(){
	}

	MDP(string fileName, string initState, string goal, string lowerProb, string higherProb, int numState, int caseNum);
	virtual void insertActionsAndNextState();
	virtual void insertRewards();
	virtual void insertRewards(int i);
};

#endif
