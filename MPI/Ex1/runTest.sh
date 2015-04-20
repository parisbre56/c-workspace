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

#conducts various tests a number of times and averages the results to produce a mean. Writes the results as matlab/octave array definitions. 
#Inner arrays are processors. Outer arrays are kappa
#First argument is an integer. Second and third argument are a list of numbers in the form "1 2 3 etc". Fourth, fifth and sixth are strings that can be ommited. 
#First argument is the number test to run
#Second argument is the number of processors
#Third argument is kappa (number of collumns per thread, *2 for extra question)
#Takes three string arguments. Both are used to define things when compiling. You can omit them if you want.
function conduct_test() {
	#>&2 echo "\nAll args=$*"
	#>&2 echo "Arg1: $1"
	#>&2 echo "Arg2: $2"
	#>&2 echo "Arg3: $3"
	#>&2 echo "Arg4: $4"
	#>&2 echo "Arg5: $5"
	#>&2 echo "Arg6: $6"
	echo -n "["
	for k in $3; do #for all kappa (number of collumns per thread, *2 for extra question)
		#>&2 echo $k
		if [ -z "$4" ]; then
			#>&2 echo "mpic++ -O2 -D NDEBUG -D \"kappa=$k\" -o ./Ex1 ./main.cpp"
			mpic++ -O2 -D NDEBUG -D "kappa=$k" -o ./Ex1 ./main.cpp
		elif [ -z "$5" ]; then
			#>&2 echo "mpic++ -O2 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -o ./Ex1 ./main.cpp"
			mpic++ -O2 -D NDEBUG -D "kappa=$k" -D "$4" -o ./Ex1 ./main.cpp
		elif [ -z "$6" ]; then
			#>&2 echo "mpic++ -O2 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -o ./Ex1 ./main.cpp"
			mpic++ -O2 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -o ./Ex1 ./main.cpp
		else
			#>&2 echo "mpic++ -O2 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -D \"$6\" -o ./Ex1 ./main.cpp"
			mpic++ -O2 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -D "$6" -o ./Ex1 ./main.cpp
		fi
		echo -n "["
		for p in $2; do #for all numbers of processors
			#>&2 echo $p
			t=0.0
			#>&2 echo $t
			for j in $(seq 1 $1); do
				>&2 echo -n "$k:$p:$j "
				mpi=$(mpiexec -n $p ./Ex1)
				sleep $(float_eval "(($k+$p*2)/100)*3")
				#>&2 echo "mpi is $mpi"
				action="$t + $mpi"
				#>&2  echo "action is $action"
				t=$(float_eval "$action")
				#>&2 echo "t is $t"
				#>&2 echo "j is $j"
			done
			#>&2 echo "$t / $j"
			tAv=$(float_eval "$t / $j")
			echo -n "$tAv "
		done
		echo "]"
	done
	echo "];"
}

  ####################################
 #		Script starts here			#
####################################

tests=20;
procs='2 4 8 16'
kappas='4 16 32 64'
echo "procs=[$procs];"
echo -n "kappas=["
for k in $kappas; do
	echo $k
done
echo "];"

#I means the first question, a simple partition of the collumns
>&2 echo "\n%I"
echo "\n%I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "" "")
echo "t1=$temp"

#II means the second question, a shuffle partition of the collumns
>&2 echo "\n%II"
echo "\n%II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question2__" "" "")
echo "t2=$temp"

>&2 echo "\n%I Single proc"
echo "\n%I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "")
echo "ts=$temp"

>&2 echo "\n%Extra I"
echo "\n%Extra I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "")
echo "te1=$temp"

>&2 echo "\n%Extra II"
echo "\n%Extra II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "")
echo "te2=$temp"

>&2 echo "\n%Extra I Single proc"
echo "\n%Extra I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__")
echo "tes=$temp"
