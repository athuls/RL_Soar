#!/usr/bin/perl
#This script is used to merge cumulative reward values for all possible learning and exploration rate combinations including alert. 
#The index values in lines 19 and 29 determine whether the rewards for first config or second config are picked

open MERGE_FILE, ">./cum_rewards_config1/merged_data_second_config" or die $!;
print MERGE_FILE "learningrate,explorationrate,cumulativereward\n";

my @files=</home/athuls89/Desktop/RL_SOAR/Fall2011/results/winter_2012/cum_rewards_config1/*>;
foreach my $file(@files)
{
	my @temp_name=split(/\//,$file);
	my $file_name=$temp_name[$#temp_name];
	if($file_name =~ m/^cumRewardsFile_(.*)_(.*)$/)
	{
			open REWARDS_FILE, $file; 
			while(<REWARDS_FILE>)
			{
				my @temp_rewards=split(/\s+/,$_);
				print MERGE_FILE "$1,$2,$temp_rewards[1]\n";
			}
			close(REWARDS_FILE);
	}	
}

open REWARDS_FILE, "<./cum_rewards_config1/cumRewardsFile_alert";
while(<REWARDS_FILE>)
{
	my @temp_rewards=split(/\s+/,$_);
	print MERGE_FILE "al,al,$temp_rewards[1]\n";
}
close(REWARDS_FILE);

close(MERGE_FILE);
