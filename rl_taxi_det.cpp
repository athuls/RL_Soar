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
//#include "mdp.h"

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

float epsilon=0.1, alpha=0.5;
int num_states=0;
int track_hashQ=0;
map <string, vector<action_value> > hash_Q;	//hash map that stores the Q values for each state action pair
map <state_action, float> reward;	//immediate reward values for state action pairs
map <state_action, string> next_state;

vector <string> delimit(string s,char character);
void initialize(string &i,string &j);

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
int select_action(string s)
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

int main()
{
srand(time(NULL));
//Parameter initialization
float gamma=0.6;
float temp_cumreward;
int num_episodes=5000;
string init_state,current_state,next_state_val, goal_state;
int rand_sel;

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values
cout<<"here"<<endl;
char* b[]={(char*)"timepass",(char*)"2", (char*)"0", (char*)"0", (char*)"24", (char*)"7", (char*)"5", (char*)"0",(char*)"3",(char*)20,(char*)24};
//if(mdp(9,b))
//	initialize(goal_state,init_state);

vector<float> cum_reward;
int episode_flip=rand()%num_episodes;
state_action flip;
int flag=0;
int count=0;
int garbage;

float next_Q;

initialize(goal_state,init_state);
while(flag==0)	//Loop over 10 episodes
{
	/*if(count==5)
	 {
                cout<<"begin1"<<endl;
                for(int i=0;i<num_states;i++)
                {       for(int j=0;j<hash_Q[i].size();j++)
                                cout<<hash_Q[i].at(j).Q_value<<endl;
                }       
                cout<<"begin1"<<endl;

                char* a[]={(char*)"timepass",(char*)"2", (char*)"0", (char*)"0", (char*)"21", (char*)"7", (char*)"5", (char*)"0", (char*)"3",(char*)"20",(char*)24};
                if(mdp(9,a))
                {
                        cin>>garbage;
                        initialize(goal_state, init_state);
                }
                cout<<"changed!!!"<<endl;
                alpha=0.01;
                cout<<"begin2"<<endl;
                for(int i=0;i<num_states;i++)
                {       for(int j=0;j<hash_Q[i].size();j++)
                                cout<<hash_Q[i].at(j).Q_value<<endl;
                }
		cout<<"begin2<<endl;
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
		reward_track.action=hash_Q[current_state].at(rand_sel).action;
		next_state_val=next_state[reward_track];
		//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
		next_Q=max(hash_Q[next_state_val]);
		//Q value update equation (next state reward used, but would work only with deterministic environments
		hash_Q[current_state].at(rand_sel).Q_value=hash_Q[current_state].at(rand_sel).Q_value+alpha*((reward[reward_track]+gamma*next_Q)-hash_Q[current_state].at(rand_sel).Q_value);
		temp_cumreward=temp_cumreward+reward[reward_track];
		current_state=next_state_val;	
	}
	cout<<temp_cumreward<<endl;
	if(temp_cumreward>=90)
		cum_reward.push_back(temp_cumreward);
	else
		cum_reward.clear();
	if(cum_reward.size()>3)
		flag=1;
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
void initialize(string &g, string &init)
{
	action_value temp;
	state_action reward_temp;
	string line,word;
	ifstream in_file;
		in_file.open("input_taxi.txt");
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
			//	if(track_hashQ==0)
				//{
					action=delimit(s.at(1),',');
					for (int i=0;i<action.size();i++)
					{
						temp.action=atoi(action.at(i).c_str());
						temp.Q_value=0;
						hash_Q[s.at(0)].push_back(temp);
					}
				//}			
				next_state_vec=delimit(s.at(2),',');
				reward_vector=delimit(s.at(3),',');
				for(int i=0;i<reward_vector.size();i++)
				{
					reward_temp.state=s.at(0);
					reward_temp.action=hash_Q[s.at(0)].at(i).action;
					reward[reward_temp]=atof(reward_vector.at(i).c_str());
					next_state[reward_temp]=next_state_vec.at(i);
				}
			}
			else if(s.size()==1) 
			{
				init=line;
				getline(in_file,line);
				g=line;
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
