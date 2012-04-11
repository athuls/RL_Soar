#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<time.h>

#define highest 6
#define lowest 0

using std::vector;
//Function to calculate the maximum Q value of next state, given the next state and possible actions/states from next state
float max(float Q1[][highest],int n,vector<int> a)
{
	float max=Q1[n][a.at(0)];
	for(int i=0;i<a.size();i++)
	{
		if(Q1[n][a.at(i)]>max)
		{
			max=Q1[n][a.at(i)];
		}			
	}
	return max;
}

int main()
{
srand(time(NULL));
float gamma=0.6;
float alpha=0.1;
int R[highest][highest];	//Reward matrix
for(int i=0;i<highest;i++)
{
	for(int j=0;j<highest;j++)
	{
		R[i][j]=0;
	}
}
//Specify the reward matrix with 0 at i,j if transition from i to j not possible, and reward value for possible actions
R[0][4]=1;
R[1][3]=1;
R[2][3]=1;
R[3][1]=1;
R[3][2]=1;
R[3][4]=1;
R[4][0]=1;
R[4][3]=1;
R[5][1]=1;
R[5][4]=1;
R[1][5]=100;

float Q[highest][highest];	//Q matrix
for(int i=0;i<highest;i++)
{
        for(int j=0;j<highest;j++)
        {
                Q[i][j]=0;
        }
}
int init_state,current_state,next_state, goal_state=5,rand_sel;
vector<int> actions;
float next_Q;
for(int i=0;i<10;i++)	//Loop over 10 episodes
{
	init_state=rand()%(highest);
	current_state=init_state;
	while(current_state!=goal_state)
	{
		for(int j=0;j<highest;j++)
		{
			if(R[current_state][j]!=0)
			{	
				actions.push_back(j);
			}	
		}
		//select next state randomly
		rand_sel=rand()%actions.size();
                next_state=actions.at(rand_sel);
		printf("the next state is %d \n",next_state);
		actions.clear();
		for(int j=0;j<highest;j++)
		{
			if(R[next_state][j]!=0)
			{
				actions.push_back(j);
			}
		}
		next_Q=max(Q,next_state,actions);
		actions.clear();
		//Q value update equation
		Q[current_state][next_state]=Q[current_state][next_state]+alpha*(((float)R[current_state][next_state]+(gamma*next_Q))-Q[current_state][next_state]);
		/*
		for(int j=0;j<highest;j++)
		{
			for(int k=0;k<highest;k++)
			{
				printf("%f ",Q[j][k]);
			}
			printf("\n");
		}
		*/
		current_state=next_state;	
	}
}
for(int i=0;i<highest;i++)
{
	for(int j=0;j<highest;j++)
	{
		printf("%f ", Q[i][j]);
	}
	printf("\n");
}
/*for(int i=0;i<highest;i++)
{
        for(int j=0;j<highest;j++)
        {
                printf("%d ", R[i][j]);
        }
        printf("\n");
}
*/
}
