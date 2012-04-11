#!/usr/bin/perl

open ALERT, "alert_stats.txt";
open STAT, "static_stats.txt";

my @alert_stats, @static_stats, @temp;

while(<ALERT>)
{
	chomp($_);	
	@temp=split(/:\s+/,$_);
	push(@alert_stats, $temp[$#temp]);	
}

while(<STAT>)
{
	chomp($_);	
	@temp=split(/:\s+/,$_);
	push(@static_stats, $temp[$#temp]);	
}

print $alert_stats[0], " ", $alert_stats[1], " ", $alert_stats[2], " ", $alert_stats[3], " ", $alert_stats[4], " ", $alert_stats[5], " ", $alert_stats[6], "\n";
print $static_stats[0], " ", $static_stats[1], " ", $static_stats[2], " ", $static_stats[3], " ", $static_stats[4], " ", $static_stats[5], " ", $static_stats[6], "\n";

my $alert_avg=$alert_stats[0], $alert_ss=$alert_stats[1], $alert_numEval=$alert_stats[2], $stat_avg=$static_stats[0], $stat_ss=$static_stats[1], $stat_numEval=$static_stats[2];

$variance=(((($alert_numEval-1)*$alert_ss)+(($stat_numEval-1)*$stat_ss))/($alert_numEval+$stat_numEval-2));
$est_var=sqrt(($variance/$alert_numEval)+($variance/$stat_numEval));
$t=($alert_avg-$stat_avg)/$est_var;
$df=($alert_numEval-1)+($stat_numEval-1);
print "alert - stat is (for learning from start ) ", $t, " ", $df,"\n"; 

$alert_avg=$alert_stats[3], $alert_ss=$alert_stats[4], $alert_numEval=$alert_stats[5], $stat_avg=$static_stats[3], $stat_ss=$static_stats[4], $stat_numEval=$static_stats[5];

#this has to be used only when sample sizes are small ( <30 )
#$variance=(((($alert_numEval-1)*$alert_ss)+(($stat_numEval-1)*$stat_ss))/($alert_numEval+$stat_numEval-2));
#$est_var=sqrt(($variance/$alert_numEval)+($variance/$stat_numEval));

#this has to be used for large sample sizes
$est_var=sqrt(($alert_ss/($alert_numEval*($alert_numEval-1)))+($stat_ss/($stat_numEval*($stat_numEval-1))));

$t=($alert_avg-$stat_avg)/$est_var;
$df=($alert_numEval-1)+($stat_numEval-1);
print "alert - stat is (for unlearning and relearn) ", $t, " ", $df,"\n"; 
