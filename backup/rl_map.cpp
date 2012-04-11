#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <map>
#include <cstdlib>
#include <sstream>
#include <iostream>

using std::vector;
using namespace std;

struct action_value{
int action;	//action is represented as the next state (assuming deterministic environment)
float Q_value;	//Q value for the state (key in the hash map) and the action above
};

int num_states=0;
map <int, vector <action_value> > hash_Q;	//hash map that stores the Q values for each state action pair
map <int, float> reward; //reward values for the states
vector <string> delimit(string s,char character);
void initialize();

//Function to calculate the maximum Q value from the next state, from amongst all actions possible from the next state
float max(vector<action_value> v)
{
	float max=v.at(0).Q_value;
	for(int i=0;i<v.size();i++)
	{
		if(v.at(i).Q_value>max)
		{
			max=v.at(i).Q_value;
		}			
	}
	return max;
}

int main()
{
srand(time(NULL));
float gamma=0.6;
float alpha=0.1;
int init_state,current_state,next_state, goal_state=7,rand_sel;

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values

initialize();

/*for(int i=0;i<num_states;i++)
{
        cout<<"reward in state"<< i<<" = "<<reward[i]<<endl;
	for(int j=0;j<hash_Q[i].size();j++)
        {
                printf("%f ", hash_Q[i].at(j).Q_value);
        }
        printf("\n");
}
*/

float next_Q;
for(int i=0;i<100;i++)	//Loop over 10 episodes
{
	init_state=rand()%(num_states);
	current_state=init_state;
	while(current_state!=goal_state)	//Loop till we reach goal state for an episode
	{
		//select action i.e. next state randomly
		rand_sel=rand()%hash_Q[current_state].size();
                
		next_state=hash_Q[current_state].at(rand_sel).action;
		//printf("the next state is %d \n",next_state);
		//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
		next_Q=max(hash_Q[next_state]);
		//Q value update equation
		hash_Q[current_state].at(rand_sel).Q_value=hash_Q[current_state].at(rand_sel).Q_value+alpha*((reward[next_state]+gamma*next_Q)-hash_Q[current_state].at(rand_sel).Q_value);
		current_state=next_state;	
	}
}

for(int i=0;i<num_states;i++)
{
	for(int j=0;j<hash_Q[i].size();j++)
	{
		printf("%f ", hash_Q[i].at(j).Q_value);
	}
	printf("\n");
}

}
//This function is used to initialize the reward hash maps, and Q values for each state action pair
/*void initialize1()
{
	
	for(int i=0;i<num_states;i++)
	{
       		reward[i]=1;
	}

//Specify the reward matrix with 0 at i,j if transition from i to j not possible, and reward value for possible actions
	reward[2]=100;

	action_value temp;

	for(int i=0;i<num_states;i++)
	{
	        switch (i)
		{
		case (0):
                	temp.action=1;
                	temp.Q_value=0;
                	hash_Q[i].push_back(temp);
			break;
                case (1):
                	temp.action=0;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			temp.action=2;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			break;
		case (2):
			temp.action=1;	
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			break;
		case (3):
			temp.action=1;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			temp.action=2;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			temp.action=4;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			break;
		case(4):
			temp.action=0;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);	
			temp.action=3;
			temp.Q_value=0;		
			hash_Q[i].push_back(temp);
			temp.action=5;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			break;
        	case(5):
			temp.action=1;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			temp.action=4;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			temp.action=5;
			temp.Q_value=0;
			hash_Q[i].push_back(temp);
			break;
		default: 
			break;
		}
	}

}
*/

vector <string> delimit(string l,char c)
{
	vector <string> result;
	int word_num=0;
	string word;
	stringstream stream1;
        stream1<<l;
        while(getline(stream1,word,c))
	{
		result.push_back(word);
	}
	return result;	
}

//Read input values for markov decision process
void initialize()
{
	action_value temp;
	string line,word;
	ifstream in_file;
	in_file.open("input.txt");
	if(in_file.is_open())
	{
        	while(!in_file.eof())
        	{
			vector <string> s;
			vector <string> action;
			getline(in_file,line);
                	s=delimit(line,'\t');
			cout<<num_states<<endl;
			cout<<s.size()<<endl;
			if(s.size())
			{
				if(s.at(0)=="State")
				{
					cout<<"yoyo"<<endl;
					continue;
				}
				reward[atoi(s.at(0).c_str())]=atof(s.at(2).c_str());
				num_states++;
				action=delimit(s.at(1),',');
				for (int i=0;i<action.size();i++)
				{
					temp.action=atoi(action.at(i).c_str());
					temp.Q_value=0;
					hash_Q[atoi(s.at(0).c_str())].push_back(temp);
				}
			}
        	}	
	}		
}
