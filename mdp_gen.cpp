#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <iostream>
using namespace std;

vector<string> split_func(string s,char separator)
{	
	vector <string> temp;
	string temp_string;
	int i=0;
	while(i<s.length())
	{
		while(i<s.length() && s.at(i)!=separator)
		{	
			temp_string.append(1,s.at(i));
			i++;
		}
		temp.push_back(temp_string);
		temp_string.clear();
		i++;
	}
	return temp;	
}

bool contains(string stemp,vector<string> s_vec)
{
	bool flag=false;
	for(int i=0;i<s_vec.size();i++)
	{
		if(s_vec.at(i)==stemp)
		{	flag=true;
			break;
		}
	}
	return flag;
}

int mdp(int argc, char* argv[])
{
	ofstream in_file;
	in_file.open("input_test.txt");
	switch (atoi(argv[1]))
	{
		case 0:
		{	string reward[10]={"1,2,3,4,8,9","0,2,4,6","0,1,3,6","0,2,7,9","0,1,5,8","4,8","1,2","3,9","5,4,9,0","0,3,7,8"};
			vector<string> split_vec;
			for(int i=0;i<10;i++)
			{
				in_file<<i<<"\t"<<reward[i]<<"\t";
				split_vec=split_func(reward[i],',');
				for(int h=0;h<split_vec.size();h++)
				{
					cout<<split_vec.at(h)<<endl;
				}
				for(int k=0;k<split_vec.size()-1;k++)
				{	
					if(split_vec.at(k)!=argv[3])	
						in_file<<"-0.1,";
					else
						in_file<<"100,";
				}
				if(split_vec.at((split_vec.size()-1))!=argv[3])
					in_file<<"-0.1";
				else
					in_file<<"100";
				in_file<<"\n";
			}
			in_file<<argv[2]<<endl;
			in_file<<argv[3];
			break;
		}
		case 1:
		{	string reward[]={"1,5","0,6,2","1,7,3","2,8,4","3,9","0,6,10","1,5,7,11","2,6,8,12","3,7,9,13","4,8,14","5,11,15","6,10,12,16","7,11,13,17","8,12,14,18","9,13,19","10,16,20","11,15,17,21","12,16,18,22","13,17,19,23","14,18,24","15,21","16,20,22","17,21,23","18,22,24","19,23"};
                        vector<string> split_vec;
                	vector<string> tempchar; 

		        for(int j=4;j<argc;j++)
			{
				tempchar.push_back(argv[j]);
			}
		
			for(int i=0;i<25;i++)
                        {
                                in_file<<i<<"\t"<<reward[i]<<"\t";
                                split_vec=split_func(reward[i],',');
                                for(int k=0;k<split_vec.size()-1;k++)
                                {
                                        if(contains(split_vec.at(k),tempchar))
						in_file<<"-10,";
					else if(split_vec.at(k)!=argv[3])
                                                in_file<<"-0.1,";
                                        else
                                                in_file<<"100,";
                                }
				if(contains(split_vec.at(split_vec.size()-1),tempchar))
					in_file<<"-10";
                                else if(split_vec.at((split_vec.size()-1))!=argv[3])
                                        in_file<<"-0.1";
                                else
                                        in_file<<"100";
                                in_file<<"\n";
                        }
			in_file<<argv[2]<<endl;
			in_file<<argv[3];
			break;
		}
		case 2:	//arguments - case(2), initial state of car, initial state of passenger, goal state, fuelgrid, levels of fuel, pick/drop locations 
		{
			string action[]={"0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5,6","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5","0,1,2,3,4,5"};
			string next_state[]={"5,0,0,1,0,0","6,1,0,2,1,1","7,2,1,3,2,2","8,3,2,4,3,3","9,4,3,4,4,4","10,0,5,6,5,5","11,1,5,7,6,6","12,2,6,8,7,7","13,3,7,9,8,8","14,4,8,9,9,9","15,5,10,11,10,10","16,6,10,12,11,11","17,7,11,13,12,12","18,8,12,14,13,13","19,9,13,14,14,14","20,10,15,16,15,15","21,11,15,17,16,16","22,12,16,18,17,17","23,13,17,19,18,18","24,14,18,19,19,19","20,15,20,21,20,20","21,16,20,22,21,21","22,17,21,23,22,22","23,18,22,24,23,23","24,19,23,24,24,24"};
                        vector<string> split_vec,split_vec1;
			vector<string> tempchar;
			vector<string> reward;
                        for(int j=7;j<argc;j++)
                        {
                                tempchar.push_back(argv[j]);
                        }
			action[atoi(argv[5])]="0,1,2,3,4,5,6";
			next_state[atoi(argv[5])]=next_state[atoi(argv[5])].append(1,',');
			next_state[atoi(argv[5])]=next_state[atoi(argv[5])].append(1,*argv[5]);

                        for(int i=0;i<25;i++)	//for every state
                        {
                                for(int j=0;j<=atoi(argv[6]);j++)	//for every fuel level
				{
					for(int k=0;k<2;k++)	//for passenger or without passenger
					{
						if(k==0)	//passenger not in car
						{
							for(int l=0;l<25;l++)
							{
								in_file<<i<<"-"<<j<<"-"<<k<<"-"<<l<<"\t"<<action[i]<<"\t";	//publish current state and actions
                                				split_vec=split_func(next_state[i],',');			
								split_vec1=split_func(action[i],',');
					        		for(int m=0;m<split_vec.size();m++)		//for every action/next state
								{			
								if(split_vec1.at(m)>="0" && split_vec1.at(m)<="3")	//if action is up, down, right or left
								{
									if(atoi(split_vec.at(m).c_str())!=i)	//if current and next state different
									{
										if(j==0)			//if fuel = 0
										{	in_file<<split_vec.at(m)<<"-"<<j<<"-"<<k<<"-"<<l<<",";
											reward.push_back("-20");
										}
										else 				//if fuel > 0
										{	in_file<<split_vec.at(m)<<"-"<<(j-1)<<"-"<<k<<"-"<<l<<",";
											reward.push_back("-1");
										}
									}
									else					//if current and next state same
									{	in_file<<split_vec.at(m)<<"-"<<j<<"-"<<k<<"-"<<l<<",";
										reward.push_back("-1");
									}
								}
								else if(split_vec1.at(m)=="4" || split_vec1.at(m)=="5")	//pick up and drop off actions
							        {	
									if(split_vec1.at(m)=="4" && l==i)	//if action is pick up and passenger and car match
									{	in_file<<split_vec.at(m)<<"-"<<j<<"-"<<1<<",";
										reward.push_back("-1");
									}
									else if(split_vec1.at(m)=="4" && l!=i)
									{	in_file<<split_vec.at(m)<<"-"<<j<<"-"<<0<<"-"<<l<<",";
										reward.push_back("-10");
									}
									else if(split_vec1.at(m)=="5")
									{	in_file<<split_vec.at(m)<<"-"<<j<<"-"<<0<<"-"<<l;
										reward.push_back("-10");
									}
								}
								else
								{	in_file<<","<<split_vec.at(m)<<"-"<<argv[6]<<"-"<<k<<"-"<<l;	
									reward.push_back("-1");
								}
							}
							in_file<<"\t";
							for(int n=0;n<reward.size()-1;n++)
							{
								in_file<<reward.at(n)<<",";
							}	
							in_file<<reward.at(reward.size()-1);
							reward.clear();
							in_file<<endl;	
							}
						}
						else
						{
								in_file<<i<<"-"<<j<<"-"<<k<<"\t"<<action[i]<<"\t";      //publish current state and actions
                                                                split_vec=split_func(next_state[i],',');
                                                                split_vec1=split_func(action[i],',');
                                                                for(int m=0;m<split_vec.size();m++)             //for every action/next state
                                                                {
                                                                if(split_vec1.at(m)>="0" && split_vec1.at(m)<="3")      //if action is up, down, right or left
                                                                {
                                                                        if(atoi(split_vec.at(m).c_str())!=i)    //if current and next state different
                                                                        {
                                                                                if(j==0)                        //if fuel = 0
                                                                                {        in_file<<split_vec.at(m)<<"-"<<j<<"-"<<k<<",";
											 reward.push_back("-20");
										}
                                                                                else                            //if fuel > 0
                                                                                {        in_file<<split_vec.at(m)<<"-"<<(j-1)<<"-"<<k<<",";
											 reward.push_back("-1");
										}
                                                                        }
                                                                        else                                    //if current and next state same
                                                                        {       in_file<<split_vec.at(m)<<"-"<<j<<"-"<<k<<",";
										reward.push_back("-1");
									}
                                                                }
                                                                else if(split_vec1.at(m)=="4" || split_vec1.at(m)=="5") //pick up and drop off actions
                                                                {
                                                                        if(split_vec1.at(m)=="5")       //if action is pick up and passenger and car match
                                                                        {       in_file<<split_vec.at(m)<<"-"<<j<<"-"<<0<<"-"<<split_vec.at(m);
										if(i==atoi(argv[4]))
											reward.push_back("100");
										else 
											reward.push_back("-1");
									}
                                                                        else if(split_vec1.at(m)=="4")
                                                                        {       in_file<<split_vec.at(m)<<"-"<<j<<"-"<<k<<",";
										reward.push_back("-10");
									}
                                                                }
                                                                else
                                                                {
                                                                        in_file<<","<<split_vec.at(m)<<"-"<<argv[6]<<"-"<<k;
									reward.push_back("-1");
                                                                }
                                                        } 
							in_file<<"\t";
							for(int n=0;n<reward.size()-1;n++)
							{
								in_file<<reward.at(n)<<",";
							}
							in_file<<reward.at(reward.size()-1);
							reward.clear();
                                                        in_file<<endl;
						}
					}
                                /*if(split_vec.size()>6)
				{
					for(int k=0;k<split_vec.size()-1;k++)
                                	{
							if(contains(split_vec.at(k),tempchar) && (split_vec1.at(k)=="4" || split_vec1.at(k)=="5" ))
								in_file<<"-1,";
							else if(!contains(split_vec.at(k),tempchar) && (split_vec1.at(k)=="4" || split_vec1.at(k)=="5" ))
								in_file<<"-10,";
							else if(split_vec.at(k)==argv[3])
								in_file<<"100,";
							else
								in_file<<"-1,";
					}
					in_file<<"-1";
				}
				else
				{
					for(int k=0;k<split_vec.size()-1;k++)
                                        {
                                                        if(contains(split_vec.at(k),tempchar) && (split_vec1.at(k)=="4" || split_vec1.at(k)=="5" ))
                                                                in_file<<"-1,";
							else if(!contains(split_vec.at(k),tempchar) && (split_vec1.at(k)=="4" || split_vec1.at(k)=="5" ))
								in_file<<"-10,";
                                                        else if(split_vec.at(k)==argv[3])
                                                                in_file<<"100,";
                                                        else
                                                                in_file<<"-1,";
                                      	}
					if(contains(split_vec.at(split_vec.size()-1),tempchar) && (split_vec1.at(split_vec.size()-1)=="4" || split_vec1.at(split_vec.size()-1)=="5" ))
	                                        in_file<<"-1";
					else if(!contains(split_vec.at(split_vec.size()-1),tempchar) && (split_vec1.at(split_vec.size()-1)=="4" || split_vec1.at(split_vec.size()-1)=="5" ))
						in_file<<"-10";
                                        else if(split_vec.at(split_vec.size()-1)==argv[3])
                                                in_file<<"100";
                                        else
   	                                        in_file<<"-1";
				}*/
				}
                        }
			in_file<<argv[2]<<"-"<<argv[6]<<"-"<<"0"<<"-"<<argv[3]<<endl;
			in_file<<argv[4]<<"-"<<"0"<<"-"<<"0"<<"-"<<argv[4]<<endl;
			break;
		}
	}	
	in_file.close();	
	return 1;
}
