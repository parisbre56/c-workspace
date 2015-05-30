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
#Arg5: An array containing the various sqrtN values to use for tests.
function conduct_test() {
	#>&2 echo "\nAll args=$*"
	#>&2 echo "Arg1: $1"
	#>&2 echo "Arg2: $2"
	#>&2 echo "Arg3: $3"
	#>&2 echo "Arg4: $4"
	#>&2 echo "Arg5: $5"
	for w1 in $3; do
		for w2 in $4; do #for all omegaRed and omegaBlack and q and dataSize
			for sqrtN in $5; do
				>&2 echo "g++ -O3 -std=c++11 -D SINGLE_PROC -D NDEBUG -D \"omegaRed=$w1\" -D \"omegaBlack=$w2\" -D \"sqrtN=$sqrtN\" -D \"ITER_CHECK=10\" -o ./Ex2 ./main.cpp"
				g++ -O3 -std=c++11 -D NDEBUG -D SINGLE_PROC -D "omegaRed=$w1" -D "omegaBlack=$w2" -D "sqrtN=$sqrtN" -D "ITER_CHECK=10" -o ./Ex2 ./main.cpp
				t=0.0
				for j in $(seq 1 $1); do #execute the thing 20 times and use sum/20 to find the average time
					>&2 echo -n "$w1:$w2:$sqrtN:1:$j "
					#sleep 2
					mpi=$(./Ex2)
					>&2 echo "$mpi"
					action="$t + $mpi"
					t=$(float_eval "$action")
				done
				#find average time 
				tAv=$(float_eval "$t / $j")
				>&2 echo "$tAv"
				outTimes="$outTimes;$tAv"
				outData="$outData;'$w1:$w2:$sqrtN:1'"
				outW1="$outW1;$w1"
				outW2="$outW2;$w2"
				outSqrtN="$outSqrtN;$sqrtN"
				outP="$outP;1"
			done
		done
	done
	echo "t=[$outTimes];"
	echo "w1=[$outW1];"	
	echo "w2=[$outW2];"
	echo "sqrtN=[$outSqrtN];"
	echo "p=[$outP];"
	echo "%Data is in the form of w1:w2:q:sqrtN:procs"
	echo "d=[$outData];"
}

  ####################################
 #		Script starts here			#
####################################

tests=5;
w1s='0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9'
#the 00001 is so that the compiler does not optimise the two equal values to make computations faster
w2s='0.100001 0.200001 0.300001 0.400001 0.500001 0.600001 0.700001 0.800001 0.900001 1.000001 1.100001 1.200001 1.300001 1.400001 1.500001 1.600001 1.700001 1.800001 1.900001'
sqrtNs='5 10 20 40 60 100 150'

conduct_test $tests "" "$w1s" "$w2s" "$sqrtNs"

