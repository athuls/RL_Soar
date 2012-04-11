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

//class for handling delta values
class vec_wrap{
public:

vector <vector <double> > v;

vec_wrap()
{
	vector<double> temp_vector;
	v.push_back(temp_vector);
}

void push(double element)
{
	if(v.at(v.size()-1).size()>10)
	{
		vector<double> v_temp;
		v_temp.push_back(element);
		v.push_back(v_temp);;
	}
	else
		v.at(v.size()-1).push_back(element);
}

double ave_v(int position)
{
	double average=0;
	for(int i=0;i<v.at(position).size();i++)	
		average=average+v.at(position).at(i);
	
	return (average/v.at(position).size());
}

double ave_mean()
{
	double mean_return=0;
	int number_elements=0;

	for(int i=0;i<v.size();i++)
	{		
		mean_return=mean_return+ave_v(i);
		number_elements++;
	}	

	return (mean_return/number_elements);
}
	
double stddev_mean()
{
	double dev=0;
	double average=ave_mean();
	for(int i=0;i<v.size();i++)
	{
		dev=dev+pow((ave_v(i)-average),2);
	}
	return dev/(v.size()-1);
}

void clean()
{
	for(int i=0;i<v.size();i++)
		v.at(i).clear();
	v.clear();
	vector<double> temp_vector;
	v.push_back(temp_vector);
}

};

struct action_value{
int action;	//action is represented as the next state (assuming deterministic environment)
float Q_value;	//Q value for the state (key in the hash map) and the action above
};

float prob_nondeterminism=0.01,alpha=0.4, epsilon=0.1, alpha_step=0.05;
int num_states=0;
int track_hashQ=0;

//map <string, vector<action_value> > hash_Q;	//hash map that stores the Q values for each state action pair
//map <state_action, float> reward;	//immediate reward values for state action pairs
//map <state_action, string> next_state;

float Q[3900][7];
float reward[3900][7];
int next_state[3900][7];

vector <string> delimit(string s,char character);
void initialize(int &i,int &j);

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

double evaluate_policy(int i,int g)
{
	double cum_reward,avg_cum_reward=0;
	int current_state,action_exec,actual_action,next_state_reached,time_cap;

	int num_episodes=50;
	
	for(int it=0;it<num_episodes;it++)
	{
		time_cap=0;
		cum_reward=0;
		current_state=i;
		while(current_state!=g)
		{
			time_cap++;
			if(time_cap>10000)
				break;
		//	cout<<"looking for goal"<<endl;
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
int runs=100,sum_count=0;
double value_policy,temp_delta;
float next_Q;
int flag=0, count=0, check_evaluate,time_cap;
int init_state,current_state,next_state_val, goal_state;
int rand_sel, obtained_index;
float sum_reward=0;
float temp_cumreward, check_convergence;
vec_wrap delta;

////////////////////////////////////////////////////////////////////////////
//Read input values for markov decision process
/////////////////////////////////////////////////////////////////////////////

//Initialize the structures for reward and initial Q values
ofstream o_file;
o_file.open("./values.csv");	

//initialize(goal_state,init_state);

//vector<float> delta_window;

//int episode_flip=rand()%runs;
//state_action flip;
//int garbage;

	initialize(goal_state,init_state);

for(int i=0;i<runs;i++)	//Loop over 10 episodes
{
	
/*	for(int j=0;j<3900;j++)
	{	for(int k=0;k<7;k++)
		{	cout<<j<<" "<<k<<" "<<next_state[j][k]<<endl;
		}		
	}*/
	cout<<"done"<<endl;
	//cin>>sum_reward;
	//Initialize the structures for reward and initial Q values
	/*cout<<"here"<<endl;
	char* b[]={(char*)"timepass",(char*)"2", (char*)"0", (char*)"0", (char*)"24", (char*)"7", (char*)"5", (char*)"0",(char*)"3",(char*)20,(char*)24};
	if(mdp(9,b))
        	initialize(goal_state,init_state);
	*/
	check_evaluate=0;
		
//		temp_cumreward=0;
//		state_action reward_track;
	while(check_evaluate<=500)
	{
		time_cap=0;
		current_state=init_state;
		while(current_state!=goal_state)	//Loop till we reach goal state for an episode
		{
			time_cap++;
			if(time_cap>10000)
				break;

			//select action i.e. next state randomly
			rand_sel=select_action(current_state);
		
			//create a structure to store the current state and action to take, which will be used for rewards in update equation
			obtained_index=get_nextstate(current_state,rand_sel);
			next_state_val=next_state[current_state][obtained_index];
			
			//we select the maximum Q value from the next state, thus allowing convergence to optimum Q values directly
			next_Q=Q[next_state_val][max(Q[next_state_val])];
			
			//get delta value and push it into delta structure
			temp_delta=(reward[current_state][obtained_index]+gamma*next_Q)-Q[current_state][rand_sel];			
			delta.push(temp_delta);
	
			//Q value update equation (next state reward used, but would work only with deterministic environments
			Q[current_state][rand_sel]=Q[current_state][rand_sel]+alpha*(temp_delta);
			
			//update alpha
			if((delta.ave_v(delta.v.size()-1)*temp_delta > 0) && ((delta.ave_v(delta.v.size()-1)-delta.ave_mean())> (0.5*delta.stddev_mean()) ))
				alpha=alpha+alpha_step;
			else if(delta.ave_v(delta.v.size()-1)*temp_delta < 0)
				alpha=alpha-alpha_step;
			
			//cout<<"here in episode"<<endl;	
				
			current_state=next_state_val;	
		}
		check_evaluate++;
		cout<<"end of episode"<<time_cap<<" "<<endl;
	}
	cout<<endl;	
	value_policy=evaluate_policy(init_state,goal_state);
	
	o_file<<value_policy<<endl;
	
	//cout<<temp_cumreward<<endl;
}
/*
for(int i=0;i<num_states;i++)
{
	for(int j=0;j<hash_Q[i].size();j++)
	{
		printf("%f ", hash_Q[i].at(j).Q_value);
	
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
		in_file.open("../convert_input_taxi.txt");
	else
		in_file.open("../convert_input_taxi.txt");
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
						//temp.action=atoi(action.at(i).c_str());
						//temp.Q_value=0;
						//hash_Q[s.at(0)].push_back(temp);
						Q[atoi(s.at(0).c_str())][atoi(action.at(i).c_str())]=0;
					}
					if(action.size()==6)
						Q[atoi(s.at(0).c_str())][6]=-INF;
				//}			
				next_state_vec=delimit(s.at(2),',');
				reward_vector=delimit(s.at(3),',');
				for(int i=0;i<reward_vector.size();i++)
				{
					//reward_temp.state=s.at(0);
					//reward_temp.action=hash_Q[s.at(0)].at(i).action;
					//reward[atoi(s.at(0))]=atof(reward_vector.at(i).c_str());
					//next_state[reward_temp]=next_state_vec.at(i);
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
