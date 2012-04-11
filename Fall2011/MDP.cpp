#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <assert.h>
#include "Domains.hpp"
#include "Environment.hpp"
#include "Agent.hpp"
#include "MDP.hpp"
using namespace std;

MDP::MDP(string fileName, string initState, string goal, string lowerProb, string higherProb, int stateNum, int caseNum){

	fileToWrite.open(fileName.c_str());	
	numStates=stateNum;
	initialState=initState;
	goalState=goal;
	lowerProbability=lowerProb;
	higherProbability=higherProb;
	
	//Blocks World (Arguments list - <0/1 depending on not alert/alert> 0 <initial state> <goal state> <lower probability> <higher probability>)
	//sample run : ./agent <0/1 depending on alert/non-alert> 0 0 9 0.1 0.9 
	
	 //Mines World (Arguments list - 1 <initial state> <goal state> <lower probability> <higher probability> <position of mines 1> <position of mines 2> ....)
	//sample run : ./agent <0/1 depending on not alert/alert> 1 0 24 0.1 0.9 3 11 17 23 
	
	//Taxi World (Arguments list - 2 <initial car state> <goal car state> <inital passenger state> <goal passenger state> <lower probability> <higher probability> <fuel refill state> <max fuel levels> <colored box 1> <colored box 2> <colored box 3> ....)
	//sample run : ./agent <0/1 depending on not alert/alert> 2 0 24 0 24 0.1 0.9 7 5 0 3 20 24 
	//NOTE: For taxi world, initial and goal states for passenger and car should be the same!!!
	actions=new string[numStates];
	nextState=new string[numStates];

	switch(caseNum){
		case 0:{
			for(int i=0;i<numStates;i++)
				actions[i]=blocksWorldActions[i];

			//Provide the next state that occurs with highest probability, remaining states would have lesser probability  
			for(int i=0;i<numStates;i++)
				nextState[i]=blocksWorldNextStates[i];	
			cout<<"I am in MDP.cpp case num"<<nextState[9]<<endl;
			break;
		}
		case 1:{
			for(int i=0;i<numStates;i++)
				actions[i]=minesWorldActions[i];

			//Provide the next state that occurs with highest probability, remaining states would have lesser probability  
			for(int i=0;i<numStates;i++)
				nextState[i]=minesWorldNextStates[i];	
			break;
		}
		case 2:{
			for(int i=0;i<numStates;i++)
				actions[i]=taxiWorldActions[i];

			//Provide the next state that occurs with highest probability, remaining states would have lesser probability  
			for(int i=0;i<numStates;i++)
				nextState[i]=taxiWorldNextStates[i];	
			break;
		}
			
	}
}

void MDP::insertActionsAndNextState(){

	for(int i=0;i<numStates;i++){
		vector<string> split_vec;
                               	
				//Insert actions for current state
                                fileToWrite<<i<<"\t"<<actions[i]<<"\t";

                                //Insert the next states along with the probabilities for the next states
                                vector<string> nextStates=split1(nextState[i], ',');
			
                                for(vector<string>::iterator it=nextStates.begin(); it!=nextStates.end();it++){
                                        if(it!=nextStates.begin())
                                                fileToWrite<<",";
					
					fileToWrite<<"["<<*it<<"-"<<higherProbability;
                                        for(vector<string>::iterator subIt=nextStates.begin(); subIt!=nextStates.end(); subIt++){
                                                if(*subIt!=*it)
                                                        fileToWrite<<";"<<*subIt<<"-"<<(atof(lowerProbability.c_str())/(double)(nextStates.size()-1));
                                        }
                                        fileToWrite<<"]";
                                }

                                fileToWrite<<"\t";
				insertRewards(i);
	}

		
	fileToWrite<<initialState<<endl;
        fileToWrite<<goalState;
	fileToWrite.close();	
	
}

void MDP::insertRewards(int i){
	
}

void MDP::insertRewards(){
}

