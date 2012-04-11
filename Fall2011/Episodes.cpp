#include<iostream>
#include<stdlib.h>
#include<string>
#include<vector>
#include<pthread.h>
#include "Episodes.hpp"
#include <unistd.h>
#include <math.h>
#include <sstream>

using namespace std;

bool statistics::operator<(const statistics &s2){
	if(((mean-std_dev) < (s2.mean-s2.std_dev)) )
		return true;
	return false;
}

bool statistics::operator>(const statistics &s2){
	if(((mean-std_dev) < (s2.mean-s2.std_dev)) )
		return false;
	return true;
}

statistics statistics::operator+(const statistics &s2){
	statistics returnStats;
	returnStats.mean=mean+s2.mean;
	returnStats.std_dev=std_dev+s2.std_dev;
	return returnStats;
}

bool statistics::operator=(const statistics &s2){
	mean=s2.mean;
	std_dev=s2.std_dev;
	return true;
}

Episodes::Episodes(Agent *agent, Environment *environment, double discount, bool alert, string results_file, string rewardsFile){
                m_agent=agent;
                m_environment=environment;
		discountRate=discount;
		numEpisodes=0;
		numEvaluateQValues=0;
		m_alert=alert;
		m_results_file=results_file;
			
		performanceStats.mean=0;
		performanceStats.std_dev=0;		
	
		//For recording the cumulative reward values in a file
		m_countFileWriteCall=0;
                m_cumRewardsFile=rewardsFile;
				
		mutex1=PTHREAD_MUTEX_INITIALIZER;
}

void Episodes::changeEnvironment(){
	
	while(true){
		
		if(find(episodeBoundaries.begin(),episodeBoundaries.end(), numEpisodes)!=episodeBoundaries.end()){
			pthread_mutex_lock(&mutex1);
			
			//Commented out evaluating because we are collecting cumulative rewards directly when the agent is learning	
			//evaluateQValues();
							
			////////////////////////////////////////////////////Blocks World//////////////////////
	/*		MDP *instance=new BlocksWorld("blocksWorld.txt", "0", "7", "0.1", "0.9", 10);
			instance->insertActionsAndNextState();
			m_environment->readConfig("blocksWorld.txt");
	*/
			////////////////////////////////////////////////////Blocks World//////////////////////
			
			////////////////////////////////////////////////////Mines World//////////////////////
			vector<string> argumentsMinesWorld;
			argumentsMinesWorld.push_back("2");	
			argumentsMinesWorld.push_back("3");	
			argumentsMinesWorld.push_back("4");	
			argumentsMinesWorld.push_back("7");	
			argumentsMinesWorld.push_back("13");	
			argumentsMinesWorld.push_back("14");	
			argumentsMinesWorld.push_back("15");	
			argumentsMinesWorld.push_back("19");	
			argumentsMinesWorld.push_back("24");	
			MDP *instance=new MinesWorld("minesWorld.txt", "0", "17", "0.1", "0.9", 25, argumentsMinesWorld);
			instance->insertActionsAndNextState();
			m_environment->readConfig("minesWorld.txt");
	
			////////////////////////////////////////////////////Mines World//////////////////////
	
			//////////////////////////////////////////////////Taxi World/////////////////////////////////
		/*	vector<string> argumentsMinesWorld;
			argumentsMinesWorld.push_back("0");
			argumentsMinesWorld.push_back("11");
			argumentsMinesWorld.push_back("18");
			argumentsMinesWorld.push_back("20");

			MDP *instance=new TaxiWorld("taxiWorld.txt", "0", "18", "0", "18", "0.1", "0.9", "24", "5", 25, argumentsMinesWorld);
			instance->insertActionsAndNextState();
			m_environment->readConfig("taxiWorld.txt");
		*/
			//////////////////////////////////////////////////Taxi World/////////////////////////////////
		
			pthread_mutex_unlock(&mutex1);
			break;
		}
	}
}	

//static parameters for SARSA learning (ON-POLICY)	
void Episodes::runEpisodes(double learningRateArg, double explorationRateArg){
	
	string currentActionByAgent, nextStateInEnvironment, nextActionByAgent;
	double rewardFromEnvironment;
	int steps;

	m_agent->initializeLearningAndExplorationRate(learningRateArg, explorationRateArg);

	while(numEpisodes!=MAX_EPISODES){
		
		steps=0;
		cumulativeReward=0;
	
		pthread_mutex_lock(&mutex1);
	
		m_agent->currentState=m_environment->m_initialState;		
		currentActionByAgent=m_agent->getAction(m_agent->currentState, m_agent->getExplorationRate());	
			
		while(m_agent->currentState!=m_environment->m_goalState){
			steps++;
			nextStateStruct tempNextStateInfo=m_environment->getNextStateAndReward(m_agent->currentState, currentActionByAgent);
			nextStateInEnvironment=tempNextStateInfo.nextState;
			rewardFromEnvironment=tempNextStateInfo.reward;
			
			//Update the cumulative reward during the learning phase - for non-stationary environments
			cumulativeReward += rewardFromEnvironment;			
		
			stateActionPair tempStateAction;
			tempStateAction.state=m_agent->currentState;
			tempStateAction.action=currentActionByAgent;

			//For SARSA
			//////////////////////////////////////////////////////////////////////////////////////////
			stateActionPair tempNextStateAction;
			tempNextStateAction.state=nextStateInEnvironment;
			tempNextStateAction.action=m_agent->getAction(nextStateInEnvironment, m_agent->getExplorationRate());
		
			m_agent->QValues[tempStateAction]=m_agent->QValues[tempStateAction]+m_agent->getLearningRate()*(rewardFromEnvironment+(discountRate*m_agent->QValues[tempNextStateAction])-m_agent->QValues[tempStateAction]);
			//////////////////////////////////////////////////////////////////////////////////////////
			
			m_agent->currentState=nextStateInEnvironment;	
			currentActionByAgent=tempNextStateAction.action;

			//cout<<"I am in the steps of the episode"<<" "<<m_environment->m_goalState<<" "<<m_agent->currentState<<endl;
			if(steps>MAX_STEPS_PER_EPISODE)
				break;
		}
		writeToFile(cumulativeReward);
		numEpisodes++;
		
		pthread_mutex_unlock(&mutex1);
		
		//if(numEpisodes==EPISODE_BOUNDARY)	
		if(find(episodeBoundaries.begin(),episodeBoundaries.end(), numEpisodes)!=episodeBoundaries.end())
		{	
			//pthread_cond_signal(&episodeNum);
			cout<<"I am waiting at 20 episodes"<<endl;
			pthread_join(threadEnv, NULL);
		}
	}
	
}

/*
//static parameters with Q-learning(max updation)
void Episodes::runEpisodes(double learningRateArg, double explorationRateArg){
	
	string currentActionByAgent, nextStateInEnvironment;
	double rewardFromEnvironment;
	int steps;

	m_agent->initializeLearningAndExplorationRate(learningRateArg, explorationRateArg);

	while(numEpisodes!=100){
		
		steps=0;
		cout<<"Goal state is "<<m_environment->m_goalState<<" "<<numEpisodes<<endl;

		pthread_mutex_lock(&mutex1);
		cout<<"acquired lock"<<endl;
		m_agent->currentState=m_environment->m_initialState;		
		
		while(m_agent->currentState!=m_environment->m_goalState){
			steps++;
			currentActionByAgent=m_agent->getAction(m_agent->currentState, m_agent->getExplorationRate());
			nextStateStruct tempNextStateInfo=m_environment->getNextStateAndReward(m_agent->currentState, currentActionByAgent);
			nextStateInEnvironment=tempNextStateInfo.nextState;
			rewardFromEnvironment=tempNextStateInfo.reward;
			
			//Update the cumulative reward during the learning phase - for non-stationary environments
			cumulativeReward += rewardFromEnvironment;			
		
			stateActionPair tempStateAction;
			tempStateAction.state=m_agent->currentState;
			tempStateAction.action=currentActionByAgent;

			//For Q-learning
			m_agent->QValues[tempStateAction]=m_agent->QValues[tempStateAction]+m_agent->getLearningRate()*(rewardFromEnvironment+(discountRate*m_agent->maximumQValue(nextStateInEnvironment))-m_agent->QValues[tempStateAction]);
			
			m_agent->currentState=nextStateInEnvironment;	
			
			cout<<"I am in the steps of the episode"<<" "<<m_environment->m_goalState<<" "<<m_agent->currentState<<endl;
			if(steps>100)
				break;
		}
		numEpisodes++;
		
		cout<<"the learning rate is "<<m_agent->getLearningRate()<<endl;
		cout<<"released lock"<<endl;
		pthread_mutex_unlock(&mutex1);
		if(numEpisodes==20)	
		{	
			cout<<"before signalling"<<endl;
			//pthread_cond_signal(&episodeNum);
//			sleep(2);
			cout<<"I am waiting at 20 episodes"<<endl;
			pthread_join(threadEnv, NULL);
		}

	}
	
	for(mapTypeQValues::iterator it=m_agent->QValues.begin();it!=m_agent->QValues.end();it++)
		cout<<"Q Value is "<<it->second<<endl;
	cout<<"The reward value is "<<cumulativeReward/(double)numEpisodes<<endl;	
}
*/

void Deltas::insertDelta(string action, double delta){
	if(mapActionDeltas[action].size()<windowSize)
		mapActionDeltas[action].push_back(delta);
	else
	{
		mapActionDeltas[action].erase(mapActionDeltas[action].begin());
		mapActionDeltas[action].push_back(delta);
	}
}

double Deltas::computeDeltaBar(string action){
	double avg=0;

	for(int i=0;i<mapActionDeltas[action].size();i++){
		avg+=mapActionDeltas[action][i];
	}
		
	if(mapActionDeltas[action].size()!=0)
	{	
		avg=(double)((double)avg/(double)mapActionDeltas[action].size());	
	
		//Computing mean of Delta bar for every action	
		mapActionDeltaBarNum[action]++;
	
		double previousMean=mapActionDeltaBarMean[action];
		mapActionDeltaBarMean[action]+=(double)((double)(avg-previousMean)/(double)mapActionDeltaBarNum[action]);	

		//For computing standard deviation
		mapActionStdDev[action]+=(avg-previousMean)*(avg-mapActionDeltaBarMean[action]);
	}
	
	return avg;
}
	
double Deltas::computeStdDev(string action){
	double returnDev;
	if(mapActionDeltaBarNum[action]>1)
		returnDev=sqrt((double)((double)mapActionStdDev[action]/(double)(mapActionDeltaBarNum[action]-1)));	

	return returnDev;
}

//Alert with SARSA
void Episodes::runEpisodesAlert(){
	
	string currentActionByAgent, nextStateInEnvironment;
	double rewardFromEnvironment;
	double currentDelta, epsilon;
	Deltas deltaObj;

	mapType mapActionAlpha;
	mapType mapActionDelta;	
	mapTypeStringInt mapActionCount;
	
	int steps;
	
	m_agent->initializeExplorationRate();
		
	while(numEpisodes!=MAX_EPISODES){
		steps=0;
		//cout<<"Goal state is "<<m_environment->m_goalState<<" "<<numEpisodes<<endl;
		pthread_mutex_lock(&mutex1);
	
		m_agent->currentState=m_environment->m_initialState;		
		currentActionByAgent=m_agent->getAction(m_agent->currentState, m_agent->getExplorationRate());
			
		cumulativeReward=0;	
		while(m_agent->currentState!=m_environment->m_goalState){
			
			cout<<currentActionByAgent<<"\t"<<m_agent->getLearningRate(currentActionByAgent)<<endl;
			steps++;
			//get current action to be executed by agent
			mapActionCount[currentActionByAgent]++;
			
			//get the next state and reward from the environment
			nextStateStruct tempNextStateInfo=m_environment->getNextStateAndReward(m_agent->currentState, currentActionByAgent);
			nextStateInEnvironment=tempNextStateInfo.nextState;
			rewardFromEnvironment=tempNextStateInfo.reward;
				
			cumulativeReward += rewardFromEnvironment;	
		
			stateActionPair tempStateAction;
			tempStateAction.state=m_agent->currentState;
			tempStateAction.action=currentActionByAgent;

			//For SARSA
			//////////////////////////////////////////////////////////////////////////////////////////
			stateActionPair tempNextStateAction;
			tempNextStateAction.state=nextStateInEnvironment;
			tempNextStateAction.action=m_agent->getAction(nextStateInEnvironment, m_agent->getExplorationRate());
			//get the delta value for the current state and action
			currentDelta=rewardFromEnvironment+((discountRate*m_agent->QValues[tempNextStateAction])-m_agent->QValues[tempStateAction]);
			//update the Q value for current state and action	
			m_agent->QValues[tempStateAction]=m_agent->QValues[tempStateAction]+m_agent->getLearningRate(currentActionByAgent)*currentDelta;
			//////////////////////////////////////////////////////////////////////////////////////////
			
			//get deltabar i.e. average of deltas for the current action
			double currentDeltaBar=deltaObj.computeDeltaBar(currentActionByAgent);

			//check and then further change the learning rate if needed
			if((currentDeltaBar*currentDelta>0) && ((currentDeltaBar-deltaObj.mapActionDeltaBarMean[currentActionByAgent])>(m_agent->constantF*deltaObj.computeStdDev(currentActionByAgent))) )
				m_agent->updateLearningRate(currentActionByAgent, true);
			else if(currentDeltaBar*currentDelta <= 0)
				m_agent->updateLearningRate(currentActionByAgent, false);	
		
			deltaObj.insertDelta(currentActionByAgent, currentDelta);		
			
			m_agent->currentState=nextStateInEnvironment;	
			currentActionByAgent=tempNextStateAction.action;
		
				
			if(steps>MAX_STEPS_PER_EPISODE)
				break;
		}
		
		writeToFile(cumulativeReward);
		numEpisodes++;
		
		//check and update the exploration rate
		if(steps>MAX_STEPS_PER_EPISODE)
			m_agent->updateExplorationRate(true);
		else
			m_agent->updateExplorationRate(false);
	
			
		//cout<<"The exploration rate is "<<m_agent->explorationRate<<" "<<endl;
		pthread_mutex_unlock(&mutex1);
		
		//if(numEpisodes==EPISODE_BOUNDARY)		
		if(find(episodeBoundaries.begin(),episodeBoundaries.end(), numEpisodes)!=episodeBoundaries.end())
		{	
			cout<<"the episode number is "<<numEpisodes<<endl;
			pthread_join(threadEnv, NULL);
			cout<<"Episode boundary reached"<<endl;
		//	pthread_cond_signal(&episodeNum);
		}
		
	}
	
	cout<<"The cumulative reward is "<<cumulativeReward/(double)numEpisodes<<endl;
}

/*
//Alert with Q learning
void Episodes::runEpisodesAlert(){
	
	string currentActionByAgent, nextStateInEnvironment;
	double rewardFromEnvironment;
	double currentDelta, epsilon;
	Deltas deltaObj;

	mapType mapActionAlpha;
	mapType mapActionDelta;	
	mapTypeStringInt mapActionCount;
	
	int steps;
	
	m_agent->initializeExplorationRate();
		
	while(numEpisodes!=100){
		steps=0;
		cout<<"Goal state is "<<m_environment->m_goalState<<" "<<numEpisodes<<endl;
		pthread_mutex_lock(&mutex1);
		cout<<"acquired lock"<<endl;
		m_agent->currentState=m_environment->m_initialState;		
	
		while(m_agent->currentState!=m_environment->m_goalState){
			steps++;
			//get current action to be executed by agent
			currentActionByAgent=m_agent->getAction(m_agent->currentState, m_agent->getExplorationRate());
			mapActionCount[currentActionByAgent]++;
			
			//get the next state and reward from the environment
			nextStateStruct tempNextStateInfo=m_environment->getNextStateAndReward(m_agent->currentState, currentActionByAgent);
			nextStateInEnvironment=tempNextStateInfo.nextState;
			rewardFromEnvironment=tempNextStateInfo.reward;
			cumulativeReward+=rewardFromEnvironment;	
		
			stateActionPair tempStateAction;
			tempStateAction.state=m_agent->currentState;
			tempStateAction.action=currentActionByAgent;

			//get the delta value for the current state and action
			currentDelta=rewardFromEnvironment+(discountRate*m_agent->maximumQValue(nextStateInEnvironment))-m_agent->QValues[tempStateAction];
			//update the Q value for current state and action	
			m_agent->QValues[tempStateAction]=m_agent->QValues[tempStateAction]+m_agent->getLearningRate(currentActionByAgent)*currentDelta;
			
			//get deltabar i.e. average of deltas for the current action
			double currentDeltaBar=deltaObj.computeDeltaBar(currentActionByAgent);

			//check and then further change the learning rate if needed
			if((currentDeltaBar*currentDelta>0) && ((currentDeltaBar-deltaObj.mapActionDeltaBarMean[currentActionByAgent])>(m_agent->constantF*deltaObj.computeStdDev(currentActionByAgent))) )
				m_agent->updateLearningRate(currentActionByAgent, true);
			else if(currentDeltaBar*currentDelta <= 0)
				m_agent->updateLearningRate(currentActionByAgent, false);	
		
			deltaObj.insertDelta(currentActionByAgent, currentDelta);		
			
			m_agent->currentState=nextStateInEnvironment;	
			
			cout<<"I am in the steps of the episode"<<" "<<m_environment->m_goalState<<" "<<m_agent->currentState<<endl;
			cout<<"Alpha value is "<<m_agent->learningRate[currentActionByAgent]<<endl;

			if(steps>100)
				break;
		}
		numEpisodes++;
		
		//check and update the exploration rate
		if(steps<100)
			m_agent->updateExplorationRate(false);
		else
			m_agent->updateExplorationRate(true);
	
			
		cout<<"The exploration rate is "<<m_agent->explorationRate<<endl;
		cout<<"released lock"<<endl;
		pthread_mutex_unlock(&mutex1);
		if(numEpisodes==70)		
		//	sleep(5);
			pthread_join(threadEnv, NULL);
			
		//	pthread_cond_signal(&episodeNum);

	}
	
	for(mapTypeQValues::iterator it=m_agent->QValues.begin();it!=m_agent->QValues.end();it++)
		cout<<"Q Value is "<<it->second<<endl;
	cout<<"the cumulative reward value is "<<cumulativeReward/(double)numEpisodes<<endl;
}
*/

void Episodes::evaluateQValues(){

	double cumReward, avgReward=0, prevAvgReward=0, variance=0, prevVariance=0, stdDev, initialExplorationRate;	
	string currAction, currState;
	int numEvaluations=TESTING_PHASE_EPISODES;	//100
	int steps;
	
	if(m_alert)
		initialExplorationRate=m_agent->getExplorationRate();
		
	
	for(int i=numEvaluations;i>0;i--){	
		prevAvgReward=avgReward;
		prevVariance=variance;
		currState=m_environment->m_initialState;
		cumReward=0;
		steps=0;
		while(currState!=m_environment->m_goalState){
			steps++;
			currAction=m_agent->getAction(currState,m_agent->getExplorationRate());
			nextStateStruct tempNextStateAndReward=m_environment->getNextStateAndReward(currState,currAction);
			cumReward=cumReward+tempNextStateAndReward.reward;
			currState=tempNextStateAndReward.nextState;
			if(steps>MAX_STEPS_PER_EPISODE)
				break;
		}
	
		writeToFile(cumReward);		
	
		if(m_alert)
		{	
			if(steps>MAX_STEPS_PER_EPISODE)
				m_agent->updateExplorationRate(true);
			else
				m_agent->updateExplorationRate(false);
		}
	
		avgReward=(double)((numEvaluations-i)*prevAvgReward+cumReward)/(double)(numEvaluations-i+1);
		variance=prevVariance+((cumReward-avgReward)*(cumReward-prevAvgReward));
	}
	
	stdDev=sqrt(variance/numEvaluations);
	statistics tempReturn;
	tempReturn.mean=avgReward;
	tempReturn.std_dev=stdDev;
	
	//Note all the cumulative rewards accumulated, in a file for further evaluation	
	//cout<<"The average reward is : "<<avgReward<<" and standard deviation is "<<stdDev<<" and variance is "<<(double)variance/(double)numEvaluations<<endl;
	
	if(m_alert)
	{	ofstream resultsFile;
		resultsFile.open(m_results_file, ios::app);
		resultsFile<<"The average reward is : "<<avgReward<<"\n"<<"The sum of squares is : "<<variance<<endl<<"Number of runs is : "<<numEvaluations<<endl;
		resultsFile.close();
	}
	
	best_static_param.push_back(avgReward);
	best_static_param.push_back(variance);
	best_static_param.push_back(numEvaluations);
			
	performanceStats=performanceStats+tempReturn;
	numEvaluateQValues++;
		
	if(m_alert)
		m_agent->setExplorationRate(initialExplorationRate);	
}

//Write cumulative rewards in files for postprocessing
void Episodes::writeToFile(double reward)
{
	//If the keyword search is greater than number of episodes in one testing phase, then it is in second evaluation
	//if(m_countFileWriteCall < TESTING_PHASE_EPISODES)
	if(m_countFileWriteCall < (MAX_EPISODES/2))
		m_cumRewards[0][m_countFileWriteCall]=reward;
	else
		m_cumRewards[1][m_countFileWriteCall%(MAX_EPISODES/2)]=reward;
		//m_cumRewards[1][m_countFileWriteCall%TESTING_PHASE_EPISODES]=reward;

	m_countFileWriteCall++;
		
	//If all the rewards have been accumulated, write it all to a file
	//if(m_countFileWriteCall == 2*TESTING_PHASE_EPISODES)
	if(m_countFileWriteCall == MAX_EPISODES)
	{
		//Open the file for writing	
		ofstream rewardsFileHandle;
		stringstream ss;
		string fileName;
		
		if(!m_alert)	
		{
			ss<<m_cumRewardsFile<<"_"<<m_agent->getLearningRate()<<"_"<<m_agent->getExplorationRate();
			fileName=ss.str();
		}
		else
		{	ss<<m_cumRewardsFile<<"_alert";
			fileName=ss.str();
		}

		rewardsFileHandle.open(fileName, ios::app);	
		//for(int i=0;i<TESTING_PHASE_EPISODES;i++)
		for(int i=0;i<MAX_EPISODES/2;i++)
		{
			for(int j=0;j<2;j++)
			{
				rewardsFileHandle<<m_cumRewards[j][i]<<"\t";	
			}
			rewardsFileHandle<<"\n";
		}
		rewardsFileHandle.close();
	}	
}
