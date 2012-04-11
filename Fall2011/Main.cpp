#include <stdlib.h>                                                                             
#include <fstream>                                                                              
#include <string>                                                                               
#include <string.h>                                                                               
#include <stdio.h>                                                                              
#include <vector>                                                                               
#include <algorithm>                                                                            
#include <cstdlib>                                                                              
#include <sstream>                                                                              
#include <pthread.h>
#include <iostream>                                                                             
#include<limits>
                                                                                                
#include "Environment.hpp"                                                                      
#include "Agent.hpp"                                                                            
#include "MDP.hpp"
#include "Domains.hpp"
#include "Episodes.hpp"

using namespace std;

void *start_thread(void *arg){
	static_cast<Episodes*>(arg)->changeEnvironment();
}

//Arguments expected are : 
//Mines : <alert/not> <domain number> <initial state> <goal state> <lower prob> <higher prob> <mines location/other data> <> <> ...
int main(int argc, char *argv[]){	
	MDP *instance;
		
	string problemInstance;
	vector<string> arguments;
	
	//Check for alert or not
	bool isAlert=false; 
	if(!strcmp(argv[1],"1"))
		isAlert=true;
		
	switch(atoi(argv[2])){
		case 0:
		{
			instance=new BlocksWorld("blocksWorld.txt", argv[3], argv[4], argv[5], argv[6], 10);
			
			instance->insertActionsAndNextState();
			problemInstance="blocksWorld.txt";
			//instance->insertRewards();		
			break;
		}	
		case 1:
		{
			for(int i=7;i<argc;i++)
				arguments.push_back(argv[i]);			
			instance=new MinesWorld("minesWorld.txt", argv[3], argv[4], argv[5], argv[6], 25, arguments);
	
			instance->insertActionsAndNextState();
			problemInstance="minesWorld.txt";
			cout<<"Completed creating file"<<endl;
			break;
		}
		case 2:
		{
                        for(int i=11;i<argc;i++)
                                arguments.push_back(argv[i]);
                        instance=new TaxiWorld("taxiWorld.txt", argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], 25, arguments);

                        instance->insertActionsAndNextState();
			problemInstance="taxiWorld.txt";
			break;
		}	
	}
	
	string res_file="static_stats_temp.txt";
	string rewardsFile="/home/athuls89/Desktop/RL_Soar/Fall2011/temp_results/cumRewardsFile";
	
	Agent *agent=new Agent();		
	agent->readConfig(problemInstance);

	Environment *env=new Environment();
	env->readConfig(problemInstance);

	Episodes *episode=new Episodes(agent, env, 0.7, isAlert, res_file, rewardsFile);
 
	int iret1=pthread_create(&episode->threadEnv, NULL, start_thread, episode);	
	
	//Run the episode with learning and exploration rates
	if(isAlert)
	{
		episode->runEpisodesAlert();	
	//	Commented out for evaluating without a separate evaluation phase i.e. when  the agent is learning
	//	episode->evaluateQValues();
	}
	else
	{
		//Run the static rates algorithm for different learning and exploration rates, to compute the best parameters for the environment
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		statistics maxStats, currentStats;
		vector<double> best_param_stats;
		maxStats.mean=numeric_limits<int>::min();
		maxStats.std_dev=0;	//could give weird behaviour if std_dev is also -INF (because for comparison we subtract std_dev from mean)
		bool flagFirst=false;
		
		double bestlr=0, bester=0;	
		
		for(double lr=0.05;lr<=0.6;lr=lr+0.05){	
			for(double er=0.01;er<=0.4;er=er+0.02){

				if(flagFirst)
				{
					//Blocks world
					//MDP *instance=new BlocksWorld(problemInstance, argv[3], argv[4], argv[5], argv[6], 10);	
					//Mines world	
					//MDP *instance=new MinesWorld(problemInstance, argv[3], argv[4], argv[5], argv[6], 25, arguments);
					//Taxi world
					MDP *instance=new TaxiWorld(problemInstance, argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], 25, arguments);
					
		                        instance->insertActionsAndNextState();
				
					delete(agent);
					delete(env);
					delete(episode);
					agent=new Agent();		
					agent->readConfig(problemInstance);
					env=new Environment();
					env->readConfig(problemInstance);	
					episode=new Episodes(agent, env, 0.7, isAlert, res_file, rewardsFile);
					int iret1=pthread_create(&episode->threadEnv, NULL, start_thread, episode);	

				}
				
				episode->runEpisodes(lr, er);	//provide the learning and exploration rates for the current iteration

				flagFirst=true;
		}
		}
		delete(agent);
		delete(env);
		delete(episode);
		
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
