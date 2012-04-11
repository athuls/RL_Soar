#include<iostream>
#include<stdlib.h>

#include<pthread.h>
#include "Domains.hpp"
#include<math.h>
//#include "boost/unordered/unordered_map.hpp"

#define MAX_EPISODES 1000
#define TESTING_PHASE_EPISODES 1000
#define MAX_STEPS_PER_EPISODE 200

using namespace std;

#ifndef episodedef
#define episodedef

#define windowSize 5

typedef boost::unordered_map<string, int, boost::hash<string>, eqstr> mapTypeStringInt;
typedef boost::unordered_map<string, vector<double>, boost::hash<string>, eqstr> mapTypeStringDoubleVector;

struct statistics{	
	double mean;
	double std_dev;
	bool operator<(const statistics &s2);	
	bool operator>(const statistics &s2);	
	statistics operator+(const statistics &s2);	
	bool operator=(const statistics &s2);	
};

class Episodes{
	
	public:
	
	pthread_t threadEnv;
	
	pthread_mutex_t mutex1;
	pthread_cond_t episodeNum;
		
	bool m_alert;
	Agent *m_agent;
//	mapTypeQValues prevQValues; //used to store the previous Q values to determine convergence
	
	Environment *m_environment;
	int numEpisodes;
	int numEvaluateQValues;
		
	double discountRate;
	double cumulativeReward;	
	
	//File to enter the cumulative rewards as well as keep the count of calls to writeToFile function in the same run of episode
	string m_cumRewardsFile;	
	int m_countFileWriteCall;	
	//double m_cumRewards[2][TESTING_PHASE_EPISODES];
	double m_cumRewards[2][MAX_EPISODES/2];
	void writeToFile(double reward);
	
	//will contain the mean, variance and number of samples, in that order, for every evaluation done
	vector<double> best_static_param;
	
	string m_results_file;	

	statistics performanceStats;
		
	Episodes(Agent *agent, Environment *environment, double discount, bool alert, string results_file, string rewardsFile);
		
//	void changeEnvironment(Environment *p);
	void changeEnvironment();
	void evaluateQValues();	
	void runEpisodes(double learningRate, double explorationRate);	
	void runEpisodesAlert();

//	bool checkConvergence();	//function to check for convergence of algorithm
};

//For Alert algorithm - handles the delta values used to determine correlation between current and past deltas, and accordingly increment/decrement learning parameters!!
class Deltas{
	
	public:
	
	//Map actions to corresponding deltas. Also maintain the window size of deltas to maintain
	mapTypeStringDoubleVector mapActionDeltas;	
	//Map actions to mean of delta bars for that action
	mapType mapActionDeltaBarMean;
	//Mao actions to number of delta bars computed for that action
	mapTypeStringInt mapActionDeltaBarNum;
	//Map actions to std. dev. in delta bars computed for that action
	mapType mapActionStdDev;		
		
	double computeDeltaBar(string action);
	double computeMeanDeltaBar(string action);
	double computeStdDev(string action);
	void insertDelta(string action, double delta);	
};

#endif
