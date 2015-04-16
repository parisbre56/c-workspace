#!/bin/bash
function float_eval()
{
	local float_scale=20
	local stat=0
	local result=0.0
	if [[ $# -gt 0 ]]; then
		result=$(echo "scale=$float_scale; $*" | bc -q 2>/dev/null)
		stat=$?
		if [[ $stat -eq 0  &&  -z "$result" ]]; then stat=1; fi
	fi
	echo $result
	return $stat
}


for i in 2 4 8 16; do
	echo $i
	t=0.0
	#echo $t
	for j in {1..20..1}; do
		mpi=$(mpiexec -n $i ./Ex1)
		#echo "mpi is $mpi"
		action="$t + $mpi"
		#echo "action is $action"
		t=$(float_eval "$action")
		#echo "t is $t"
		#echo "j is $j"
	done
	tAv=$(float_eval "$t / $j")
	echo $tAv
done
