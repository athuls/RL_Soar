#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<ctime>

#include "boost/unordered/unordered_map.hpp"
#include "Agent.hpp"
#include "Environment.hpp"

using namespace std;

	string Agent::getEnvironmentState(){
		return " "; 
	}
	
	Agent::Agent(){
		srand((unsigned)time(NULL));
		alphaMax=0.9;
                alphaMin=0.05;
                alphaSteps=30;
                delAlpha=(double)((double)(alphaMax-alphaMin)/(double)alphaSteps);

                epsilonMax=0.5;
                epsilonMin=0.01;
                epsilonStep=30;
                delEpsilon=(double)((double)(epsilonMax-epsilonMin)/(double)epsilonStep);
		constantF=0.5;
	}

/////////////////The below functions are for without Alert
	void Agent::initializeLearningAndExplorationRate(double learning, double exploration){
		learningRateNonAlert=learning;
		explorationRate=exploration;
	}
	
	double Agent::getLearningRate(){
		return learningRateNonAlert;
	}
	

/////////////////The below functions are for Alert
	//to obtain learning rate
	double Agent::getLearningRate(string action){
		
		if(learningRate.find(action)==learningRate.end())
		{
			learningRate[action]=alphaMax;
			return alphaMax;
		}
		else
			return learningRate[action];
	}

	//to get exploration rate
	double Agent::getExplorationRate(){
		return explorationRate;
	}
	
	//update the exploration rate	
	void Agent::updateExplorationRate(bool increase){
        	double currRate=explorationRate;
	        if(increase)
        	        currRate+=delEpsilon;
	        else
	                currRate-=delEpsilon;

	        if(currRate<=epsilonMax && currRate>=epsilonMin)
	                explorationRate=currRate;
	}

	void Agent::setExplorationRate(double rate){
		explorationRate=rate;
	}
		
	void Agent::updateLearningRate(string action, bool increase){

	        double currRate=learningRate[action];
	
	        if(increase)
        	        currRate+=delAlpha;
	        else
        	        currRate-=delAlpha;

	        if(currRate<=alphaMax && currRate>=alphaMin)
        	        learningRate[action]=currRate;
	}
	
	//learning rates are intialized when getLearningRate is called	
	void Agent::initializeExplorationRate(){
		explorationRate=epsilonMax;
	}
	
	void Agent::readConfig(string fileName){

		string line;
		vector<string> splitLine;

		ifstream config(fileName.c_str());
		if(config.is_open())
		{
			while(config.good()){
				getline(config,line);
				splitLine=split1(line, '\t');
				if(splitLine.size()<2)
					continue;
				stateActionPair tempStateAction;
				tempStateAction.state=splitLine[0];
				vector<string> actionList=split1(splitLine[1],',');
				for(vector<string>::iterator it=actionList.begin();it!=actionList.end();it++){
					tempStateAction.action=*it;
					QValues[tempStateAction]=0;		
				}
			}
			config.close();
		}
		
	}

	//Epsilon-greedy action selection
	string Agent::getAction(string currState, double explorationRate){
		string action;
		double randomNumber=(double)((double)rand()/(double)RAND_MAX);
		int randomActionSelection;
		bool randomAction=false;
			vector<string> actionList;
		string maxAction;
	
		if(randomNumber<explorationRate)
			randomAction=true;

		for(mapTypeQValues::iterator it=QValues.begin();it!=QValues.end();it++){
			if(!it->first.state.compare(currState))
				actionList.push_back(it->first.action);
		}
			
		maxAction=maximumValueAction(currState);

				if(randomAction)	//random action should be one among all possible actions, except the maximum value action
				{
					do{
						 randomActionSelection=rand()%actionList.size();	
					}while(!maxAction.compare(actionList[randomActionSelection]));

					return actionList[randomActionSelection];
				}				
				return maxAction;
	}

	//Greedy action selection
	string Agent::maximumValueAction(string state){
		double max=0;
		bool first=true;
		string returnAction="";

		for(mapTypeQValues::iterator it=QValues.begin();it!=QValues.end();it++){
			if(!it->first.state.compare(state)){
				if(first)	
				{	max=it->second;
					returnAction=it->first.action;
					first=false;
				}
					if(it->second>max)
					{	
						max=it->second;
						returnAction=it->first.action;
					}	
				}
			}
		return returnAction;
	}

	double Agent::maximumQValue(string nextState){
		double max=0;
		for(mapTypeQValues::iterator it=QValues.begin();it!=QValues.end();it++){
			if(!it->first.state.compare(nextState)){
					if(it->second>max)
						max=it->second;
				}
			}
		return max;
	}	

