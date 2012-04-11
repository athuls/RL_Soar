//Do we need this?

#include "Episodes.hpp"

using namespace std;

class perfCompare
{
	double maxRewards[TESTING_PHASE_EPISODES];
	double compareRewards[TESTING_PHASE_EPISODES];
	
	//Store the sums of ranks for the max array and the array with which comparison is being done	
	double maxSum, compArrSum;
	double sortedRewards[2*TESTING_PHASE_EPISODES];
		
	//Checks for the maxRewards and accordingly updates the maximum rewards array
	bool updateMaxArray();			
};
