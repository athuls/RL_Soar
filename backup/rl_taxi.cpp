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

float epsilon=0.01, alpha=0.2, prob_nondeterminism=0.1;
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

int get_nextstate(string state,int current_index)
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
int runs=100;
int sum_count=0;

string init_state,current_state,next_state_val, goal_state;
int rand_sel, obtained_index;

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values
int count=0;
float sum_reward=0;
ofstream o_file;
o_file.open("./domain_taxi/nond_0.0001/prob_0.7_0.01csv");	

//initialize(goal_state,init_state);

vector<float> cum_reward;
vector<float> delta_window;
float delta;

float temp_cumreward, check_convergence;
int flag=0;
int episode_flip=rand()%runs;
state_action flip;
int garbage;

float next_Q;
for(int i=0;i<runs;i++)	//Loop over 10 episodes
{
	sum_reward=0;
	count=0;
	flag=0;
	cum_reward.clear();
	
	track_hashQ=0;
	initialize(goal_state,init_state);

	//Initialize the structures for reward and initial Q values
	/*cout<<"here"<<endl;
	char* b[]={(char*)"timepass",(char*)"2", (char*)"0", (char*)"0", (char*)"24", (char*)"7", (char*)"5", (char*)"0",(char*)"3",(char*)20,(char*)24};
	if(mdp(9,b))
        	initialize(goal_state,init_state);
	*/
		
	check_convergence=90;
	
	while(flag==0)
	{
		count++;
		temp_cumreward=0;
	
		current_state=init_state;
		state_action reward_track;
		while(current_state!=goal_state)	//Loop till we reach goal state for an episode
		{
			//select action i.e. next state randomly
			//rand_sel=rand()%hash_Q[current_state].size();
			rand_sel=select_action(current_state);
		
			//create a structure to store the current state and action to take, which will be used for rewards in update equation
	                reward_track.state=current_state;
			
			obtained_index=get_nextstate(current_state,rand_sel);
			reward_track.action=hash_Q[current_state].at(obtained_index).action;
			next_state_val=next_state[reward_track];
			//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
			next_Q=max(hash_Q[next_state_val]);
			//Q value update equation (next state reward used, but would work only with deterministic environments
			hash_Q[current_state].at(rand_sel).Q_value=hash_Q[current_state].at(rand_sel).Q_value+alpha*((reward[reward_track]+gamma*next_Q)-hash_Q[current_state].at(rand_sel).Q_value);
			temp_cumreward=temp_cumreward+reward[reward_track];
			current_state=next_state_val;	
		}
		cout<<temp_cumreward<<endl;
		if(temp_cumreward>=check_convergence)
			cum_reward.push_back(temp_cumreward);
		else
			cum_reward.clear();
		if(cum_reward.size()>3)
			flag=1;	
		sum_reward=sum_reward+temp_cumreward;
	}
	cout<<"yoyo"<<endl;
	o_file<<sum_reward/count<<"\t"<<count<<endl;	
	sum_count=sum_count+count;
}
	cout<<"the average count is "<<(double)((double)sum_count/runs)<<endl;
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
void initialize(string &g, string &init)
{
	action_value temp;
	state_action reward_temp;
	string line,word;
	ifstream in_file;
	if(track_hashQ==0)
		in_file.open("input_taxi.txt");
	else
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
				//if(track_hashQ==0)
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
