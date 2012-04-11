#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "Environment.hpp"
#include "Agent.hpp"
#include "MDP.hpp"

using namespace std;

#ifndef DomainProblem
#define DomainProblem

bool contains(string str, vector<string> vec);

class BlocksWorld:public MDP{
	
	public:
	BlocksWorld(string fileName, string initState, string goal, string lowerProb, string higherProb, int numState);
	
	void insertRewards(int numState);
};

class MinesWorld:public MDP{
	
	public:
	vector<string> argsVec;
	
	MinesWorld(string fileName, string initState, string goal, string lowerProb, string higherProb, int numState, vector<string> args);
	void insertRewards(int i);
};	

class TaxiWorld:public MDP{
	string m_initCar;
	string m_goalCar;
	string m_initPassenger;
	string m_goalPassenger;
	string m_lowerProb;
	string m_higherProb;
	string m_refillState;
	string m_maxFuel;
	ofstream m_fileToWrite;
	
	public:
	TaxiWorld(string fileName, string initCar, string goalCar, string initPassenger, string goalPassenger, string lowerProb, string higherProb, string refillState, string maxFuel, int numStates, vector<string> args);
	vector<string> reward;
	vector<string> argsVec;
	
	vector<string> safeSpots;	//safe spots to pick up and drop off
				
	void insertActionsAndNextState();		
	void insertRewards();
	bool contains(int num);
	void publishNextState(vector<string> nextState);
};

#endif
