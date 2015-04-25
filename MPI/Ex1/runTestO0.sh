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
#First argument is an integer. Second and third argument are a list of numbers in the form "1 2 3 etc". The rest are strings that can be ommited. 
#First argument is the number test to run
#Second argument is the number of processors
#Third argument is kappa (number of collumns per thread, *2 for extra question)
#Takes four string arguments. All are used to define things when compiling. You can omit them if you want.
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
			#>&2 echo "mpic++ -O0 -D NDEBUG -D \"kappa=$k\" -o ./Ex1 ./main.cpp"
			mpic++ -O0 -D NDEBUG -D "kappa=$k" -o ./Ex1 ./main.cpp
		elif [ -z "$5" ]; then
			#>&2 echo "mpic++ -O0 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -o ./Ex1 ./main.cpp"
			mpic++ -O0 -D NDEBUG -D "kappa=$k" -D "$4" -o ./Ex1 ./main.cpp
		elif [ -z "$6" ]; then
			#>&2 echo "mpic++ -O0 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -o ./Ex1 ./main.cpp"
			mpic++ -O0 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -o ./Ex1 ./main.cpp
		elif [ -z "$7" ]; then
			#>&2 echo "mpic++ -O0 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -D \"$6\" -o ./Ex1 ./main.cpp"
			mpic++ -O0 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -D "$6" -o ./Ex1 ./main.cpp
		else
			#>&2 echo "mpic++ -O0 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -D \"$6\" -D \"$7\" -o ./Ex1 ./main.cpp"
			mpic++ -O0 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -D "$6" -D "$7" -o ./Ex1 ./main.cpp
		fi
		echo -n "["
		for p in $2; do #for all numbers of processors
			#>&2 echo $p
			t=0.0
			#>&2 echo $t
			for j in $(seq 1 $1); do
				>&2 echo -n "$k:$p:$j "
				mpi=$(mpiexec --allow-run-as-root -n $p ./Ex1)
				#sleep $(float_eval "(($k+$p*2)/5)")
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
#procs='2 4 8 16'
#kappas='4 16 32 64'
procs='2 4 6 8 10 12 14 16'
kappas='4 16 32 64'
echo "procs=[$procs];"
echo -n "kappas=["
for k in $kappas; do
	echo $k
done
echo "];"

#I means the first question, a simple partition of the collumns
>&2 echo -e "\n%I"
echo -e "\n%I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "" "")
echo "t1=$temp"

#II means the second question, a shuffle partition of the collumns
>&2 echo -e "\n%II"
echo -e "\n%II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question2__" "" "")
echo "t2=$temp"

#Single proc means only 1 thread
>&2 echo -e "\n%I Single proc"
echo -e "\n%I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "")
echo "ts=$temp"

#Extra means doing the inverse instead of gaussian elimination
>&2 echo -e "\n%Extra I"
echo -e "\n%Extra I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "")
echo "te1=$temp"

>&2 echo -e "\n%Extra II"
echo -e "\n%Extra II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "")
echo "te2=$temp"

>&2 echo -e "\n%Extra I Single proc"
echo -e "\n%Extra I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__")
echo "tes=$temp"

#BTest means broadcast test, which means using broadcast instead of send and receive
>&2 echo -e "\n%BTest I"
echo -e "\n%BTest I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "USE_BROADCAST" "" "")
echo "bt1=$temp"

>&2 echo -e "\n%BTest II"
echo -e "\n%BTest II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question2__" "USE_BROADCAST" "" "")
echo "bt2=$temp"

>&2 echo -e "\n%BTest I Single proc"
echo -e "\n%BTest I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "USE_BROADCAST" "")
echo "bts=$temp"

>&2 echo -e "\n%BTest Extra I"
echo -e "\n%BTest Extra I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST" "")
echo "bte1=$temp"

>&2 echo -e "\n%BTest Extra II"
echo -e "\n%BTest Extra II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST" "")
echo "bte2=$temp"

>&2 echo -e "\n%BTest Extra I Single proc"
echo -e "\n%BTest Extra I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__" "USE_BROADCAST")
echo "btes=$temp"

#AsBTest means asynchronus broadcast test, same as broadcast test but asynchronus
>&2 echo -e "\n%AsBTest I"
echo -e "\n%AsBTest I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "USE_BROADCAST_ASYNC" "" "")
echo "abt1=$temp"

>&2 echo -e "\n%AsBTest II"
echo -e "\n%AsBTest II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question2__" "USE_BROADCAST_ASYNC" "" "")
echo "abt2=$temp"

>&2 echo -e "\n%AsBTest I Single proc"
echo -e "\n%AsBTest I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "USE_BROADCAST_ASYNC" "")
echo "abts=$temp"

>&2 echo -e "\n%AsBTest Extra I"
echo -e "\n%AsBTest Extra I"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST_ASYNC" "")
echo "abte1=$temp"

>&2 echo -e "\n%AsBTest Extra II"
echo -e "\n%AsBTest Extra II"
temp=$(conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST_ASYNC" "")
echo "abte2=$temp"

>&2 echo -e "\n%AsBTest Extra I Single proc"
echo -e "\n%AsBTest Extra I Single proc"
temp=$(conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__" "USE_BROADCAST_ASYNC")
echo "abtes=$temp"
