//Q-learning for mines and blocks domain
//with non-deterministic environments
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
#include "mdp.h"
#include<math.h>

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
float Q_value;	//Q value for the state (key in the hash map) and the action above
};

float epsilon=0,alpha=0.4,prob_nondeterminism=0;
int num_states=0;
int track_hashQ=0;
map <int, vector <action_value> > hash_Q;	//hash map that stores the Q values for each state action pair
map <state_action, float> reward;	//immediate reward values for state action pairs
vector <string> delimit(string s,char character);
void initialize(int &i,int &j);

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

//Function for action selection using epsilon-greedy
int select_action(int s)
{
	float r=(double)rand() / (double)RAND_MAX;
	int index_fin;
	if(r>=0 && r<=epsilon)
	{	
		index_fin=rand()%hash_Q[s].size();
	}
	else
	{
		float max_val=max(hash_Q[s]);
		for(int i=0;i<hash_Q[s].size();i++)
		{
			if(hash_Q[s].at(i).Q_value==max_val)
			{
				index_fin=i;
				break;
			}
		}
	}
	return index_fin;
}

int get_nextstate(int state,int current_index)
{
	int return_obtained;
	float r=(double)rand() / (double)RAND_MAX;

        if(r>=0 && r<=prob_nondeterminism)
        {
                return_obtained=rand()%hash_Q[state].size();
        }
        else
        {	
		return_obtained=current_index;
	}
	return return_obtained;
}

int main()
{
srand(time(NULL));
//Parameter initialization
float gamma=0.6;
int runs=500;
int sum_count=0;

int init_state,current_state,next_state, goal_state,rand_sel,obtained_index;
int count=0;
float sum_reward=0;
ofstream o_file;
o_file.open("./deterministic/prob_0.4_0.csv");

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values


vector<float> cum_reward;
//delta values for adaptation
vector<float> delta_window;
float delta;

float temp_cumreward, check_convergence;
int flag=0;
int episode_flip=rand()%runs;
state_action flip;
int garbage;

float next_Q;

vector<double> deviation;
deviation.clear();

		sum_count=0;
		for(int i=0;i<runs;i++)	//Loop over the required number of runs
		{
			sum_reward=0;
			count=0;
			flag=0;
			cum_reward.clear();
			
			track_hashQ=0;
			initialize(goal_state,init_state);
			
			check_convergence=95;
				
			while(flag==0)	//stopping criterion is the value function approaching a threshold
			{	
				count++;
				//flipping state
				/*if(cum_reward.size()>1 && track_hashQ==1)
				{	
				char* a[]={(char*)"timepass",(char*)"1", (char*)"0", (char*)"20", (char*)"10", (char*)"12", (char*)"3", (char*)"14", (char*)"21", (char*)24};
				if(mdp(9,a))
				{
					//cin>>garbage;
					initialize(goal_state, init_state);
					check_convergence=99.5;
					cum_reward.clear();
				}
				cout<<"changed!!!"<<endl;
				//alpha=0.01;
				cout<<"begin2"<<endl;
				for(int i=0;i<num_states;i++)
		                {       for(int j=0;j<hash_Q[i].size();j++)
		                                cout<<hash_Q[i].at(j).Q_value<<endl;
		                }
				cout<<"begin2"<<endl;
				}*/	
			temp_cumreward=0;
				
			current_state=init_state;
			state_action reward_track;
			/*if(count==9)
				alpha=1;*/
		
			
			while(current_state!=goal_state)	//Loop till we reach goal state for an episode
			{
				//select action i.e. next state randomly
				//rand_sel=rand()%hash_Q[current_state].size();
				rand_sel=select_action(current_state);
		
				//create a structure to store the current state and action to take, which will be used for rewards in update equation
		                reward_track.state=current_state;
		
				/////////////////////////////////////////////////
				/*this section modified to include stochasticity		
				reward_track.action=hash_Q[current_state].at(rand_sel).action;
		
				next_state=hash_Q[current_state].at(rand_sel).action;
				*/
				obtained_index=get_nextstate(current_state,rand_sel);
				next_state=hash_Q[current_state].at(obtained_index).action;
				reward_track.action=hash_Q[current_state].at(obtained_index).action;
				////////////////////////////////////////////////
				
				//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
				next_Q=max(hash_Q[next_state]);
				//Q value update equation (next state reward used, but would work only with deterministic environments
				hash_Q[current_state].at(rand_sel).Q_value=hash_Q[current_state].at(rand_sel).Q_value+alpha*((reward[reward_track]+gamma*next_Q)-hash_Q[current_state].at(rand_sel).Q_value);
				temp_cumreward=temp_cumreward+reward[reward_track];
				current_state=next_state;	
			//	alpha=alpha*(pow(1.1,-(double)num_episode));
			}
			
			//to determine that 4 consecutive episodes had a cumulative reward of greater than 99.3
			cout<<temp_cumreward<<endl;
			if(temp_cumreward>=check_convergence)
				cum_reward.push_back(temp_cumreward);
			else 
				cum_reward.clear();
			if(cum_reward.size() > 3)
				flag=1;
			sum_reward=sum_reward+temp_cumreward;
			}
			//int r;
			//cin>>r;
			//cout<<"yoyoyoyoyoyoyoyooyyoyoyoyoyoyoyoyoyoyoooooooooooooooooo"<<endl;
			o_file<<sum_reward/count<<"\t"<<count<<endl;
			deviation.push_back(count);
			sum_count=sum_count+count;
		}
		
		double mean_count=(double)((double)sum_count/runs);
		cout<<"the average count is "<<mean_count<<endl;
		
		double sum_deviation=0;
		for(int a=0;a<deviation.size();a++)
			sum_deviation=sum_deviation+(pow(((double)deviation.at(a)-mean_count),2));	
		
		cout<<"the variance is "<<sum_deviation/deviation.size()<<endl;		
		
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
void initialize(int &g, int &init)
{
	action_value temp;
	state_action reward_temp;
	string line,word;
	ifstream in_file;
	if(track_hashQ==0)
		in_file.open("input_mines_easy.txt");
	else
		in_file.open("input_mines_easy.txt");
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
			//	if(track_hashQ==0)
			//	{
					action=delimit(s.at(1),',');
					for (int i=0;i<action.size();i++)
					{
						temp.action=atoi(action.at(i).c_str());
						temp.Q_value=0;
						hash_Q[atoi(s.at(0).c_str())].push_back(temp);
					}
			//	}
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
		track_hashQ++;
		cout<<"I am here hashQ"<<endl;
		map <state_action,float>::iterator it;
		/*for(it=reward.begin();it!=reward.end();it++)
		{
			cout<<(*it).first.state<<" "<<(*it).first.action<<" "<<endl;
			cout<<(*it).second<<endl;
		}*/
	}		
}
