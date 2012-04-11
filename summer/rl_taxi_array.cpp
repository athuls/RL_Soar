#include <stdlib.h>
#include<limits>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <map>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <math.h>
	
#define INF numeric_limits<float>::infinity()

using std::vector;
using namespace std;

struct state_action{
string state;
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
float Q_value;	//Q value for the state (key in the hash map) and the action above
};

float prob_nondeterminism=0.1, alpha=0.2, epsilon=0.1;
int num_states=0;
int track_hashQ=0;

float Q[3900][7];
float reward[3900][7];
int next_state[3900][7];

vector <string> delimit(string s,char character);
vector<int> initialize(int &j);

//Function to calculate the maximum Q value from the next state, from amongst all actions possible from the next state
int max(float v[])
{
	float max1=v[0];
	int return_i=0;

	for(int i=0;i<7;i++)
	{
		if(v[i]>max1)
		{
			max1=v[i];
			return_i=i;
		}			
	}
	return return_i;
}

//Function for action selection using epsilon-greedy
int select_action(int s)
{
	float r=(double)rand() / (double)RAND_MAX;
	int index_fin;
	if(r>=0 && r<=epsilon)
	{
		if(Q[s][6]==-INF)
			index_fin=rand()%6;
		else
			index_fin=rand()%7;
	}
	else
	{
		index_fin=max(Q[s]);
	}
	return index_fin;
}

int get_nextstate(int state,int current_index)
{
        int return_obtained;
        float r=(double)rand() / (double)RAND_MAX;

        if(r>=0 && r<=prob_nondeterminism)
        {
                if(Q[state][6]==-INF)
			return_obtained=rand()%6;
		else
			return_obtained=rand()%7;			
        }
        else
        {
                return_obtained=current_index;
        }
        return return_obtained;
}

bool check_goalstate(int state,vector<int> set_of_goals)
{
	for(int i=0;i<set_of_goals.size();i++)	
	{
		if(state==set_of_goals.at(i))
			return true;
	}	
	return false;
}

double evaluate_policy(int i,vector<int> g)
{
	double cum_reward,avg_cum_reward=0;
	int current_state,action_exec,actual_action,next_state_reached,time_cap;

	int num_episodes=50;
	
	for(int it=0;it<num_episodes;it++)
	{
		time_cap=0;
		cum_reward=0;
		current_state=i;
		while(!check_goalstate(current_state,g))
		{
			time_cap++;
			if(time_cap>10000)
				break;
			action_exec=max(Q[current_state]);
			actual_action=get_nextstate(current_state,action_exec);
			next_state_reached=next_state[current_state][actual_action];
			cum_reward=cum_reward+reward[current_state][actual_action];
			current_state=next_state_reached;
			cout<<current_state<<endl;
			
		}
		avg_cum_reward=avg_cum_reward+cum_reward;
	}
	cout<<"yoyo here"<<endl;

	return avg_cum_reward/num_episodes;	
}

int main()
{
srand(time(NULL));
//Parameter initialization
float gamma=0.6;
int runs=100;
int sum_count=0;

double value_policy;

vector<float> deviation;

int init_state,current_state,next_state_val;
vector<int> goal_states;
int rand_sel, obtained_index;

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values
int count=0, check_evaluate,time_cap;
float sum_reward=0;
ofstream o_file;
o_file.open("./values.csv");	


vector<float> cum_reward;
float delta;

float temp_cumreward, check_convergence;
int flag=0;

float next_Q;
deviation.clear();	//just to make sure the deviation vector is clear
goal_states=initialize(init_state);

for(int i=0;i<runs;i++)	//Loop over 10 episodes
{
	
	cout<<"done"<<endl;
	check_evaluate=0;
		while(check_evaluate<=500)
		{
			time_cap=0;
			current_state=init_state;
			while(!check_goalstate(current_state,goal_states))	//Loop till we reach goal state for an episode
			{
				time_cap++;
				//cout<<current_state<<endl;
				//select action i.e. next state randomly
				//rand_sel=rand()%hash_Q[current_state].size();
				rand_sel=select_action(current_state);
			
				//create a structure to store the current state and action to take, which will be used for rewards in update equation
		                //reward_track.state=current_state;
				
				obtained_index=get_nextstate(current_state,rand_sel);
				//reward_track.action=(obtained_index).action;
				next_state_val=next_state[current_state][obtained_index];
				//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
				next_Q=Q[next_state_val][max(Q[next_state_val])];
				//Q value update equation (next state reward used, but would work only with deterministic environments
				Q[current_state][rand_sel]=Q[current_state][rand_sel]+alpha*((reward[current_state][obtained_index]+gamma*next_Q)-Q[current_state][rand_sel]);
				current_state=next_state_val;	
				cout<<current_state<<endl;
			}
			check_evaluate++;
			cout<<"end of episode"<<time_cap<<" "<<endl;
		}
		cout<<endl;	
		value_policy=evaluate_policy(init_state,goal_states);
		
		o_file<<value_policy<<endl;
		
		//cout<<temp_cumreward<<endl;
}
	o_file.close();
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
vector<int> initialize(int &init)
{
	action_value temp;
	state_action reward_temp;
	vector<int> goal_return;
	int flag_initread=0;
	string line,word;
	ifstream in_file;
	if(track_hashQ==0)
		in_file.open("../convert_input_taxi1_multiple_goals.txt");
	else
		in_file.open("../convert_input_taxi1_multiple_goals.txt");
	if(in_file.is_open())
	{
			vector <string> action;
			vector <string> next_state_vec; 
			vector <string> reward_vector;
			vector <string> s;
        	while(!in_file.eof())
        	{
			getline(in_file,line);
                	s=delimit(line,'\t');
			if(s.size() > 1)
			{
				if(s.at(0)=="State")
				{
					continue;
				}
				num_states++;
					action=delimit(s.at(1),',');
					for (int i=0;i<action.size();i++)
					{
						Q[atoi(s.at(0).c_str())][atoi(action.at(i).c_str())]=0;
					}
					if(action.size()==6)
						Q[atoi(s.at(0).c_str())][6]=-INF;
				next_state_vec=delimit(s.at(2),',');
				reward_vector=delimit(s.at(3),',');
				for(int i=0;i<reward_vector.size();i++)
				{
					reward[atoi(s.at(0).c_str())][atoi(action.at(i).c_str())]=atof(reward_vector.at(i).c_str());
					next_state[atoi(s.at(0).c_str())][atoi(action.at(i).c_str())]=atoi(next_state_vec.at(i).c_str());
				}
				if(reward_vector.size()==6)
				{	reward[atoi(s.at(0).c_str())][6]=-INF;
					next_state[atoi(s.at(0).c_str())][6]=(int)INF;
				}
			}
			else if(s.size()==1) 
			{
				if(flag_initread==0)
				{
					init=atoi(line.c_str());	
					getline(in_file,line);
					flag_initread=1;
				}
				goal_return.push_back(atoi(line.c_str()));
			}
        	}
		track_hashQ++;
		cout<<"I am here hashQ"<<endl;
		map <state_action,float>::iterator it;
	}
	return goal_return;		
}
