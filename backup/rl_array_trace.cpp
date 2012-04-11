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

struct state_action{
int state;
int action;

friend bool operator<(state_action const& a, state_action const& b)
{
	bool result;
	if(a.state==b.state)
		result=(a.action<b.action);
	else
		result=(a.state<b.state);
	return result;
}
};

struct action_value{
int action;	//action is represented as the next state (assuming deterministic environment)
double Q_value;	//Q value for the state (key in the hash map) and the action above
action_value()
{
	Q_value=0;
}
};

float epsilon=0.1, alpha=0.5;
int num_states=0;
map <int, vector <action_value> > hash_Q;	//hash map that stores the Q values for each state action pair
map <state_action, double> reward;	//immediate reward values for state action pairs
map <state_action, double> trace;	//eligibility trace for each state action pair 
vector <string> delimit(string s,char character);
void initialize(int &i,int &j);

//Function to calculate the maximum Q value from the next state, from amongst all actions possible from the next state

int maxQ_index(int state)
{
	int result;
//	double maxQ=max(hash_Q[state]);

	double max1=hash_Q[state].at(0).Q_value;
	result=0;
        for(int i=1;i<hash_Q[state].size();i++)
        {
                if(hash_Q[state].at(i).Q_value>max1)
                {
                        max1=hash_Q[state].at(i).Q_value;
			result=i;
                }
        }
	return result;
}

//Function for action selection using epsilon-greedy
int select_action(int s)
{
	float r=(double)rand() / (double)RAND_MAX;
	int index_fin;
	if(r>=0 && r<=epsilon)
		index_fin=rand()%hash_Q[s].size();
	else
		index_fin=maxQ_index(s);
	return index_fin;
}

int main()
{
srand(time(NULL));
//Parameter initialization
double gamma=0.6;
int num_episodes=1;
double trace_decay=0.6;

int init_state,current_state,next_state, goal_state,next_action, best_action, current_action;

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values

initialize(goal_state,init_state);

//cout<<hash_Q[11].size()<<endl;
//exit(0);
double cum_reward=0;
double next_Q;
double delta;
	
state_action current_pair_track, temp, temp_init;

map<state_action,double>::iterator it;

for(int i=0;i<num_states;i++)
{	for(int j=0;j<hash_Q[i].size();i++)
	{	temp_init.state=i;
		temp_init.action=hash_Q[i].at(j).action;
		trace[temp_init]=0;
	}
}		

for(int i=0;i<num_episodes;i++)	//Loop over 10 episodes
{
	current_state=init_state;
	current_action=select_action(current_state);

	while(current_state!=goal_state)	//Loop till we reach goal state for an episode
	{
		//select action i.e. next state randomly
		//rand_sel=rand()%hash_Q[current_state].size();
		//create a structure to store the current state and action to take, which will be used for rewards in update equation
                current_pair_track.state=current_state;
		current_pair_track.action=hash_Q[current_state].at(current_action).action;
		
		next_state=hash_Q[current_state].at(current_action).action;
		next_action=select_action(next_state);
		best_action=maxQ_index(next_state);
		//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
		cout<<next_state<<" "<<next_action<<endl;
		//next_Q=hash_Q[next_state].at(next_action).Q_value;
		next_Q=hash_Q[next_state].at(best_action).Q_value;
		delta=(reward[current_pair_track]+gamma*next_Q)-hash_Q[current_state].at(current_action).Q_value;
		trace[current_pair_track]=trace[current_pair_track]+1;
		cout<<"I am here "<<hash_Q[next_state].at(next_action).Q_value<<endl;

		//Q value update equation (next state reward used, but would work only with deterministic environments
		for(it=trace.begin();it!=trace.end();it++)
		{
				hash_Q[it->first.state].at(it->first.action).Q_value=hash_Q[it->first.state].at(it->first.action).Q_value+alpha*delta*trace[temp];
				if(best_action==next_action)
					trace[temp]=trace_decay*gamma*trace[temp];
				else
					trace[temp]=0;
		}
		cum_reward=cum_reward+reward[current_pair_track];
		current_state=next_state;	
		current_action=next_action;
	}
	cout<<cum_reward<<endl;
	cum_reward=0;
}
/*
for(int i=0;i<num_states;i++)
{
	for(int j=0;j<hash_Q[i].size();j++)
	{
		printf("%f ", hash_Q[i].at(j).Q_value);
	}
	printf("\n");
}
*/
}

vector <string> delimit(string l,char c)
{
	vector <string> result;
	string word;
	stringstream stream1;
        stream1<<l;
        while(getline(stream1,word,c))
	{
		result.push_back(word);
	}
	return result;	
}

//Read input values for markov decision process from input.txt
void initialize(int &g, int &init)
{
	action_value temp;
	state_action reward_temp;
	string line,word;
	ifstream in_file;
	in_file.open("input_mines.txt");
	if(in_file.is_open())
	{
			vector <string> action;
			vector <string> reward_vector;
			vector <string> s;
        	while(!in_file.eof())
        	{
			getline(in_file,line);
                	s=delimit(line,'\t');
			if(s.size()>1)
			{
				if(s.at(0)=="State")
				{
					continue;
				}
				num_states++;
				action=delimit(s.at(1),',');
				for (int i=0;i<action.size();i++)
				{
					temp.action=atoi(action.at(i).c_str());
					temp.Q_value=0;
					hash_Q[atoi(s.at(0).c_str())].push_back(temp);
				}
				reward_vector=delimit(s.at(2),',');
				for(int i=0;i<reward_vector.size();i++)
				{
					reward_temp.state=atoi(s.at(0).c_str());
					reward_temp.action=hash_Q[atoi(s.at(0).c_str())].at(i).action;
					reward[reward_temp]=atof(reward_vector.at(i).c_str());
				}
			}
			else if(s.size()==1)
			{
				init=atoi(line.c_str());
				getline(in_file,line);
				g=atoi(line.c_str());
			}
        	}
		map <state_action,float>::iterator it;
		/*for(it=reward.begin();it!=reward.end();it++)
		{
			cout<<(*it).first.state<<" "<<(*it).first.action<<" "<<endl;
			cout<<(*it).second<<endl;
		}*/
	}		
}
