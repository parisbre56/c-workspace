#!/bin/bash

#computes a floating point expression
function float_eval() {
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

#conducts various tests a number of times and averages the results to produce a mean. 
#Writes the results as matlab/octave array definitions in stdout. 
#One contains the times, the other contains data about how thos times were obtained.
#Arg1: Number of tests to run, must be positive >=1
#Arg2: An array containing the number of processors to use in various tests
#Arg3: An array containing the omega value for red. All those values will be tested
#Arg4: Same as Arg3 for black
#Arg5: An array containing the q values to use for tests.
#Arg6: An array containing the various sqrtN values to use for tests.
#Arg7: An array containing the numbers of buffers we are going to be testing
function conduct_test() {
	#>&2 echo "\nAll args=$*"
	#>&2 echo "Arg1: $1"
	#>&2 echo "Arg2: $2"
	#>&2 echo "Arg3: $3"
	#>&2 echo "Arg4: $4"
	#>&2 echo "Arg5: $5"
	#>&2 echo "Arg6: $6"
	for w1 in $3; do
		for w2 in $4; do #for all omegaRed and omegaBlack and q and dataSize
			for q in $5; do
				for sqrtN in $6; do
					for b in $7; do
						set -x
						mpic++ -O3 -std=c++11 -D NDEBUG -D MULTI_BUFF -D "BUFFERS=$b" -D "omegaRed=$w1" -D "omegaBlack=$w2" -D "qDef=$q" -D "sqrtN=$sqrtN" -o ./Ex2 ./main.cpp
						set +x
						for p in $2; do #for all numbers of processors
							t=0.0
							for j in $(seq 1 $1); do #execute the thing 20 times and use sum/20 to find the average time
								>&2 echo -n "$w1:$w2:$q:$sqrtN:$p:$b:$j "
								#timelimit is used because sometimes the MPIEXEC itself hangs even though the underlying process exits normally and produces a result
								sleep 2
								mpi=$(timelimit -q -t 120 -T 20 mpiexec --allow-run-as-root -n $p ./Ex2)
								>&2 echo "$mpi"	
								action="$t + $mpi"
								t=$(float_eval "$action")
							done
							#find average time 
							tAv=$(float_eval "$t / $j")
							>&2 echo "$tAv"
							outTimes="$outTimes;$tAv"
							outData="$outData;'$w1:$w2:$q:$sqrtN:$p:$b'"
							outW1="$outW1;$w1"
							outW2="$outW2;$w2"
							outQ="$outQ;$q"
							outSqrtN="$outSqrtN;$sqrtN"
							outP="$outP;$p"
							outB="$outB;$b"
						done
					done
				done
			done
		done
	done
	echo "t=[$outTimes];"
	echo "w1=[$outW1];"	
	echo "w2=[$outW2];"
	echo "q=[$outQ];"
	echo "sqrtN=[$outSqrtN];"
	echo "p=[$outP];"
	echo "b=[$outB];"
	echo "%Data is in the form of w1:w2:q:sqrtN:procs:buffs"
	echo "d=[$outData];"
}

  ####################################
 #		Script starts here			#
####################################

tests=5;
procs='16'
w1s='1.9'
w2s='1.9'
qs='sqrt(nthreads)/4 sqrt(nthreads)/2 sqrt(nthreads) sqrt(nthreads)*2 sqrt(nthreads)*4'
sqrtNs='1000'
buffs='2 3 4'

conduct_test $tests "$procs" "$w1s" "$w2s" "$qs" "$sqrtNs" "$buffs"

