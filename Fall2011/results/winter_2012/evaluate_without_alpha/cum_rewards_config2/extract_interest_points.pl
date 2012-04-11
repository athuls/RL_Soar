#!/usr/bin/perl

open FILE,"<output_R";

while(<FILE>)
{
	chomp($_);
	my @templine=split(/\s+/,$_);
	if($#templine==4)
	{
		if($templine[0] =~ m/al:al/)
		{
			if(($templine[1] < 0) && $templine[$#templine] < 0.05)
			{
				print "$_\n";
			}
		}
	}
}
