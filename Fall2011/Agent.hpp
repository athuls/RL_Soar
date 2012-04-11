#include<string>
#include<vector>
#include<fstream>
#include "boost/unordered/unordered_map.hpp"
#include<cstdlib>
#include<ctime>
#include "Environment.hpp"

using namespace std;

#ifndef Alert
#define Alert

typedef boost::unordered_map<stateActionPair, double, boost::hash<stateActionPair>, eqStateActionPair> mapTypeQValues;

class Agent{
	
	string getEnvironmentState();
	
	public:
	
	double discountRate, delAlpha, alphaMax, alphaMin, alphaSteps, delEpsilon, epsilonMax, epsilonMin, epsilonStep, constantF;

	//learningRate mapping every action to a learning rate alpha
	mapType learningRate;
	double explorationRate, learningRateNonAlert;

	Agent();
	void readConfig(string file);
	
	//parameter functions
	double getLearningRate(string action);
	void initializeLearningAndExplorationRate(double learn, double explore);
	double getLearningRate();
	double getExplorationRate();
	void updateExplorationRate(bool flag);
	void updateLearningRate(string action, bool flag);
	void initializeExplorationRate();
	void setExplorationRate(double rate);
	
	double maximumQValue(string state);
	string getAction(string state, double rate);
	string maximumValueAction(string action);
			
	string currentState;
	mapTypeQValues QValues;
	
}; 

#endif
