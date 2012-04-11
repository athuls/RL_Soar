#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <map>
#include "boost/unordered/unordered_map.hpp"
#include "Agent.hpp"
#include "Environment.hpp"

using namespace std;

	Environment::Environment(){
		srand((unsigned)time(NULL));
	}
	
	bool eqStateActionPair::operator()(stateActionPair p1, stateActionPair p2) const
	{
		return (!(p1.state+" "+p1.action).compare(p2.state+" "+p2.action));
	}

	bool eqstr::operator()(string s1, string s2) const
  	{
		return (!s1.compare(s2));
  	}

	size_t hash_value(stateActionPair const& s)
	{
		boost::hash<string> hasher;
		return hasher((s.state+" "+s.action));
	}

	void Environment::readConfig(string fileName)
	{
		 string line;
                vector<string> splitLine, possibleActions, possibleNextStates;

                ifstream config(fileName.c_str());
		
		if(config.is_open())
                {
                        while(config.good()){
                                getline(config,line);
                        	
				splitLine=split1(line, '\t');
				
				if(splitLine.size()<2)
				{	
					m_initialState=splitLine[0];
					if(config.good())
					{	getline(config,line);
						m_goalState=line;
					}
					break;
				}	
			
				mapType  mapRewards;       
				possibleActions=split1(splitLine[1],',');
				possibleNextStates=split1(splitLine[2], ',');
				mapRewards=getRewards(splitLine[3]);

				for(int i=0;i<possibleActions.size();i++){
					stateActionPair tempStateAction;
	                                tempStateAction.state=splitLine[0];
        	                        tempStateAction.action=possibleActions[i];
						
					mapNextState[tempStateAction]=getNextStates(possibleNextStates[i], mapRewards);
				}
                        }
                        config.close();
                }

                /*for(boost::unordered_map<stateActionPair, vector<nextStateStruct> >::iterator it=mapNextState.begin();it!=mapNextState.end();it++){
                        cout<<it->first.state<<" "<<it->first.action<<" "<<it->second.at(0).nextState<<" "<<it->second.at(0).reward<<endl;
                }*/
		
	}

	mapType Environment::getRewards(string rewards){
		mapType returnMap;
		vector<string> rewardNextState=split1(rewards,',');
		for(int i=0;i<rewardNextState.size();i++){
			vector<string> rewardAndNextState = split1(rewardNextState[i], ':');
			returnMap[rewardAndNextState[1]]=atof(rewardAndNextState[0].c_str());
		}
		return returnMap;
	}

	vector<nextStateStruct> Environment::getNextStates(string nextStates, mapType mapArg){
                nextStates=removeBrackets(nextStates);
		vector<nextStateStruct> returnVector;
		vector<string> nextStateWithProbabilities=split1(nextStates,';');
                for(int i=0;i<nextStateWithProbabilities.size();i++)
                {
			nextStateStruct tempNextState;
			vector<string> stateProbability=split1(nextStateWithProbabilities[i], '-');
			tempNextState.nextState=stateProbability[0];
			tempNextState.probability=atof(stateProbability[1].c_str());
			tempNextState.reward=mapArg[stateProbability[0]];
			returnVector.push_back(tempNextState);
		}
		
                return returnVector;
        }

	nextStateStruct Environment::getNextStateAndReward(string agentState, string agentAction){
		
		double randomNumber=(double)rand()/(double)RAND_MAX;	
		stateActionPair tempStruct;
		tempStruct.state=agentState;
		tempStruct.action=agentAction;
		
		//depending on the random number generated, check if it lies within higher or lower probabilities
		vector<nextStateStruct>::iterator it=mapNextState[tempStruct].begin();
		
		int index=0;
	
			if(randomNumber<=it->probability)
				return *it;
			else 
			{
				double probValue=it->probability;
				//index gives the number of times we need to increment
				if(fmod((randomNumber-probValue),((++it)->probability))==0.0)	
					index=((randomNumber-probValue)/(it->probability));	
				else
					index=((randomNumber-probValue)/(it->probability))+1;	
			}
	
			for(int i=0;i<index-1;i++)
				it++;
			
			return *it;
	}

	
vector<string> split1(string l,char c)
{
        vector <string> result;
        string word;
        stringstream stream1;
        stream1<<l;
        while(getline(stream1,word,c))
        {
               if(word.compare(""))
			result.push_back(word);
        }
        return result;
}

string removeBrackets(string word){
	string returnString="";
	for(int i=0;i<word.size();i++){
		if(word.at(i)!=']' && word.at(i)!='[')
			returnString=returnString+word.at(i);
	}
	return returnString;
}

/*void Environment::readMDP(){
                string line, init, g;
		int num_states=0;
		double track_hashQ=0;
                ifstream in_file;
		vector<string> s, action, reward_vector, next_state_vec;
                in_file.open("input_taxi.txt");         
                struct actionVal temp;
			
		stAcPair reward_temp; 	
		map<stAcPair, double> reward;
		map<stAcPair, string> next_state;
			
		map<string, vector<struct actionVal> > hash_Q;	
		
		if(in_file.is_open()){
                        while(!in_file.eof()){
              		        getline(in_file, line);
				s=split1(line,'\t');
	                        if(s.size() > 1)
	                        {
	                                if(s.at(0)=="State")
	                                {
        	                                continue;
                	                }
                        	        num_states++;
                	                action=split1(s.at(1),',');
                        	        for (int i=0;i<action.size();i++)
                                	{
                                    		temp.action=atoi(action.at(i).c_str());
	                                        temp.Q_value=0;
	                                        hash_Q[s.at(0)].push_back(temp);
                                        }
                        	        next_state_vec=split1(s.at(2),',');
                                	reward_vector=split1(s.at(3),',');
	                                for(int i=0;i<reward_vector.size();i++)
        	                        {
                	                        reward_temp.state=s.at(0);
                        	                reward_temp.action=hash_Q[s.at(0)].at(i).action;
					        reward[reward_temp]=atof(reward_vector.at(i).c_str());
                                	        next_state[reward_temp]=next_state_vec.at(i);
	                                }
        	                }
                	        else if(s.size()==1)
	                        {
        	                        init=line;
                	                getline(in_file,line);
                        	        g=line;
	                        }	
        	        }	
                	track_hashQ++;
	                cout<<"I am here hashQ"<<endl;
        	        map <stAcPair,float>::iterator it;
	                for(it=reward.begin();it!=reward.end();it++)
        	        {
                	        cout<<(*it).first.state<<" "<<(*it).first.action<<" "<<endl;
                        	cout<<(*it).second<<endl;
	                }	
        	}

	}*/ 

