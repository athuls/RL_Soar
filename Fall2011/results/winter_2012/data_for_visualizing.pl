#!/usr/bin/perl

open FH, "<learningrates_for_actions_alert";

my @linesplit;
my @action_alpha;
my $count=0;

while(<FH>)
{
	chomp $_;
	@linesplit=split(/\s+/, $_);
	if($linesplit[0] eq "the")
	{
		print "$count\n";
	}
	if($linesplit[0] eq "7")
	{
		$action_alpha[$#action_alpha+1]=$linesplit[1];		
		$count++;
	}
}


for(my $i=0;$i<=$#action_alpha;$i++)
{
	print "$i\t$action_alpha[$i]\n";
}
