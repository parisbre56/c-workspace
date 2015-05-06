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
					>&2 echo "mpic++ -O3 -std=c++11 -D NDEBUG -D \"omegaRed=$w1\" -D \"omegaBlack=$w2\" -D \"qDef=$q\" -D \"sqrtN=$sqrtN\" -o ./Ex2 ./main.cpp"
					mpic++ -O3 -std=c++11 -D NDEBUG -D "omegaRed=$w1" -D "omegaBlack=$w2" -D "qDef=$q" -D "sqrtN=$sqrtN" -o ./Ex2 ./main.cpp
					for p in $2; do #for all numbers of processors
						t=0.0
						for j in $(seq 1 $1); do #execute the thing 20 times and use sum/20 to find the average time
							>&2 echo -n "$w1:$w2:$q:$sqrtN:$p:$j "
							mpi=$(mpiexec --allow-run-as-root -n $p ./Ex2)
							action="$t + $mpi"
							t=$(float_eval "$action")
						done
						#find average time 
						tAv=$(float_eval "$t / $j")
						>&2 echo "$tAv"
						outTimes="$outTimes;$tAv"
						outData="$outData;\'$w1:$w2:$q:$sqrtN:$p\'"
					done
				done
			done
		done
	done
	echo "t=[$outTimes];"
	echo "%Data is in the form of w1:w2:q:sqrtN:procs"
	echo "d=[$outData];"
}

  ####################################
 #		Script starts here			#
####################################

tests=20;
procs='50'
w1s='0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9'
w2s='0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9'
qs='sqrt(nthreads)/4 sqrt(nthreads)/2 sqrt(nthreads) sqrt(nthreads)*2 sqrt(nthreads)*4'
sqrtNs='150'

conduct_test $tests "$procs" "$w1s" "$w2s" "$qs" "$sqrtNs"

