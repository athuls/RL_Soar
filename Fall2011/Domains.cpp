#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "MDP.hpp"
#include "Domains.hpp"
#include "Environment.hpp"
#include "Agent.hpp"

#include "Constants.hpp"

bool contains(string stemp,vector<string> s_vec)
{
        bool flag=false;
        for(int i=0;i<s_vec.size();i++)
        {
                if(s_vec.at(i)==stemp)
                {       flag=true;
                        break;
                }
        }
        return flag;
}


BlocksWorld::BlocksWorld(string fileName, string initState, string goal, string lowerProb, string higherProb, int numState):MDP(fileName, initState, goal, lowerProb, higherProb, numState, 0){

}


void BlocksWorld::insertRewards(int stateNum){

	vector<string> nextStates=split1(nextState[stateNum],',');

	 for(int k=0;k<nextStates.size();k++)
                                {
                                        if(k!=0)
                                                fileToWrite<<",";
                                        if(nextStates.at(k)!=goalState)
                                                fileToWrite<<"-0.1:"<<nextStates.at(k);
                                        else
                                                fileToWrite<<"100:"<<nextStates.at(k);
                                }
                                fileToWrite<<"\n";
                        
}

MinesWorld::MinesWorld(string fileName, string initState, string goal, string lowerProb, string higherProb, int numStates, vector<string> args):MDP(fileName, initState, goal, lowerProb, higherProb, numStates, 1){
			
			argsVec=args;
}


void MinesWorld::insertRewards(int stateNum){
			vector<string> nextStates=split1(nextState[stateNum], ',');
			 for(int k=0;k<nextStates.size();k++)
                                {
                        			
			                if(k!=0)
                                                fileToWrite<<",";
                                        if(contains(nextStates.at(k),argsVec))
                                                fileToWrite<<"-10:"<<nextStates.at(k);
                                        else if(!nextStates.at(k).compare(goalState))
                                                fileToWrite<<"100:"<<nextStates.at(k);
                                        else
                                                fileToWrite<<"-0.1:"<<nextStates.at(k);
                                }
                                fileToWrite<<"\n";

	  }

TaxiWorld::TaxiWorld(string fileName, string initCar, string goalCar, string initPassenger, string goalPassenger, string lowerProb, string higherProb, string refillState, string maxFuel, int numStates, vector<string> args):MDP(fileName, initCar, goalCar, lowerProb, higherProb, numStates, 2){
		
			m_initCar=initCar;
			m_goalCar=goalCar;
			m_initPassenger=initPassenger;
			m_goalPassenger=goalPassenger;
			m_lowerProb=lowerProb;
			m_higherProb=higherProb;
			m_refillState=refillState;
			m_maxFuel=maxFuel;	
			m_fileToWrite.open(fileName.c_str());
				
                        argsVec=args;
			actions[atoi(refillState.c_str())]="0,1,2,3,4,5,6";
                        //nextState is an array which gives the next state with highest probability, given the current state and action
                        nextState[atoi(refillState.c_str())]=nextState[atoi(refillState.c_str())].append(1,',');
                        nextState[atoi(refillState.c_str())]=nextState[atoi(refillState.c_str())].append(1,*refillState.c_str());
}

void TaxiWorld::insertActionsAndNextState(){
		
	vector<string> nextStatesVec, nextActionsVec;
	vector<string> allNextStates;	//to store all the next states, given a current state
	stringstream ss;	//used to store next states
		
	for(int i=0;i<numStates;i++)   //for every position of car
                        {
                                for(int j=0;j<=atoi(m_maxFuel.c_str());j++)       //for every fuel level
                                {
                                        for(int k=0;k<2;k++)    //for passenger or without passenger
                                        {
                                                if(k==0)        //passenger not in car
                                                {
                                                        for(int l=0;l<numStates;l++)	// for every position of passenger
                                                        {
                                                                fileToWrite<<i<<"+"<<j<<"+"<<k<<"+"<<l<<"\t"<<actions[i]<<"\t";      //publish current state and actions

                                                                nextStatesVec=split1(nextState[i],',');
                                                                nextActionsVec=split1(actions[i],',');
					
								allNextStates.clear();
                                                                reward.clear(); 

								for(int m=0;m<nextStatesVec.size();m++)             //for every next state
                                                                {

                                                                if(nextActionsVec.at(m)>="0" && nextActionsVec.at(m)<="3")      //if action is up, down, right or left
                                                                {
                                                                        if(atoi(nextStatesVec.at(m).c_str())!=i)    //if current and next state different
                                                                        {
                                                                                if(j==0)                        //if fuel = 0
                                                                                {       	
											ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<k<<"+"<<l;
											allNextStates.push_back(ss.str());
											//fileToWrite<<nextStatesVec.at(m)<<"-"<<j<<"-"<<k<<"-"<<l<<",";
                                                                                        reward.push_back("-20:"+ss.str());
											ss.str("");
                                                                                }
                                                                                else                            //if fuel > 0
                                                                                {       
											int tempj=j-1;
											ss<<nextStatesVec.at(m)<<"+"<<tempj<<"+"<<k<<"+"<<l;
											allNextStates.push_back(ss.str());
											//fileToWrite<<nextStatesVec.at(m)<<"-"<<(j-1)<<"-"<<k<<"-"<<l<<",";
                                                                                        reward.push_back("-1:"+ss.str());
											ss.str("");
                                                                                }
                                                                        }
                                                                        else                                    //if current and next state same
                                                                        {       
										ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<k<<"+"<<l;
										allNextStates.push_back(ss.str());
										//fileToWrite<<nextStatesVec.at(m)<<"-"<<j<<"-"<<k<<"-"<<l<<",";
                                                                                reward.push_back("-1:"+ss.str());
										ss.str("");
                                                                        }
                                                                }
                                                                else if(nextActionsVec.at(m)=="4" || nextActionsVec.at(m)=="5") //pick up and drop off actions
							 	{
                                                                        if(nextActionsVec.at(m)=="4" && l==i)       //if action is pick up and passenger,car positions match
                                                                        {       
										ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<"1";
										allNextStates.push_back(ss.str());
										//fileToWrite<<nextStatesVec.at(m)<<"-"<<j<<"-"<<1<<",";
										if(contains(i))
											reward.push_back("-1:"+ss.str());
										else
											reward.push_back("-10:"+ss.str());
										ss.str("");
                                                                        }
									//if action=pickup and passenger position=car position
                                                                        else if(nextActionsVec.at(m)=="4" && l!=i)	
                                                                        {       
										ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<0<<"+"<<l;
										allNextStates.push_back(ss.str());
										//fileToWrite<<nextStatesVec.at(m)<<"-"<<j<<"-"<<0<<"-"<<l<<",";
                                                                                reward.push_back("-10:"+ss.str());
										ss.str("");
                                                                        }
									//action is dropoff
                                                                        else if(nextActionsVec.at(m)=="5")
                                                                        {       
										ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<0<<"+"<<l;
										allNextStates.push_back(ss.str());
										//fileToWrite<<nextStatesVec.at(m)<<"-"<<j<<"-"<<0<<"-"<<l;
                                                                                reward.push_back("-10:"+ss.str());
										ss.str("");
                                                                        }
                                                                }
                                                                else
                                                                {       
									ss<<nextStatesVec.at(m)<<"+"<<m_maxFuel<<"+"<<k<<"+"<<l;
									allNextStates.push_back(ss.str());
									//fileToWrite<<","<<nextStatesVec.at(m)<<"-"<<argsVec[7]<<"-"<<k<<"-"<<l;
                                                                        reward.push_back("-1:"+ss.str());
									ss.str("");
                                                                }
                                                        }
							publishNextState(allNextStates);
							insertRewards();
							}
						}
						else
						//if passenger is in car 
                                                {
                                                                fileToWrite<<i<<"+"<<j<<"+"<<k<<"\t"<<actions[i]<<"\t";      //publish current state and actions
                                                                nextStatesVec=split1(nextState[i],',');
                                                                nextActionsVec=split1(actions[i],',');
								allNextStates.clear();
								reward.clear();					
			
                                                                for(int m=0;m<nextStatesVec.size();m++)             //for every action/next state
                                                                {
                                                                if(nextActionsVec.at(m)>="0" && nextActionsVec.at(m)<="3")      //if action is up, down, right or left
                                                                {
                                                                        if(atoi(nextStatesVec.at(m).c_str())!=i)    //if current and next state different
                                                                        {
                                                                                if(j==0)                        //if fuel = 0
                                                                                {        
											 ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<k;
											 allNextStates.push_back(ss.str());
                                                                                         reward.push_back("-20:"+ss.str());
                                                                                         ss.str("");
                                                                                }
                                                                                else                            //if fuel > 0
                                                                                {        int tempj=j-1;
											 ss<<nextStatesVec.at(m)<<"+"<<tempj<<"+"<<k;
											 allNextStates.push_back(ss.str());
                                                                                         reward.push_back("-1:"+ss.str());
                                                                                         ss.str("");
                                                                                }
                                                                        }
                                                                        else                                    //if current and next state same
                                                                        {       ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<k;
										allNextStates.push_back(ss.str());
                                                                                reward.push_back("-1:"+ss.str());
                                                                                ss.str("");
                                                                        }
                                                                }
                                                                else if(nextActionsVec.at(m)=="4" || nextActionsVec.at(m)=="5") //pick up and drop off actions
                                                                {
                                                                        if(nextActionsVec.at(m)=="5")       //if action is drop off and passenger is in car
                                                                        {       
										ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<0<<"+"<<nextStatesVec.at(m);
										allNextStates.push_back(ss.str());
                                                                                if(i==atoi(m_goalPassenger.c_str()))
                                                                                        reward.push_back("100:"+ss.str());
										else if(contains(i))
											reward.push_back("-1:"+ss.str());
                                                                                else
                                                                                        reward.push_back("-10:"+ss.str());
                                                                                ss.str("");
									}
                                                                        else if(nextActionsVec.at(m)=="4")
                                                                        {       	
										ss<<nextStatesVec.at(m)<<"+"<<j<<"+"<<k;
										allNextStates.push_back(ss.str());
                                                                                reward.push_back("-10:"+ss.str());
                                                                                ss.str("");
                                                                        }
                                                                }
                                                                else
                                                                {
                                                                        ss<<nextStatesVec.at(m)<<"+"<<m_maxFuel<<"+"<<k;
									allNextStates.push_back(ss.str());
                                                                        reward.push_back("-1:"+ss.str());
                                                                        ss.str("");
                                                                }
                                                        }
                                                        publishNextState(allNextStates);
							insertRewards();
						}

					}
			}
		}
		fileToWrite<<m_initCar<<"+"<<m_maxFuel<<"+"<<"0"<<"+"<<m_initPassenger<<"\n";
		fileToWrite<<m_goalCar<<"+"<<"1"<<"+"<<"0"<<"+"<<m_goalPassenger<<"\n";
		fileToWrite.close();

}

void TaxiWorld::publishNextState(vector<string> nextStates){
	
	double lowerProb=(double)(atof(m_lowerProb.c_str()))/(double)(nextStates.size()-1);
	for(int i=0;i<nextStates.size();i++){
		if(i!=0)	
			fileToWrite<<",";
		fileToWrite<<"["<<nextStates[i]<<"-"<<m_higherProb;
		for(int j=0;j<nextStates.size();j++){
			if(i!=j)
				fileToWrite<<";"<<nextStates[j]<<"-"<<lowerProb;
		}
		fileToWrite<<"]";
	}
	fileToWrite<<"\t";
}

void TaxiWorld::insertRewards(){
	for(int n=0;n<reward.size()-1;n++)
		fileToWrite<<reward.at(n)<<",";
        fileToWrite<<reward.at(reward.size()-1);
        fileToWrite<<endl;
}

bool TaxiWorld::contains(int num){
	for(int i=0;i<argsVec.size();i++){
		if(atoi(argsVec[i].c_str())==num)
			return true;
	}
	return false;
}
