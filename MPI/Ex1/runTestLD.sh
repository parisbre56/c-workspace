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
			#>&2 echo "mpic++ -O3 -D NDEBUG -D \"kappa=$k\" -o ./Ex1 ./main.cpp"
			mpic++ -O3 -D NDEBUG -D "kappa=$k" -o ./Ex1 ./main.cpp
		elif [ -z "$5" ]; then
			#>&2 echo "mpic++ -O3 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -o ./Ex1 ./main.cpp"
			mpic++ -O3 -D NDEBUG -D "kappa=$k" -D "$4" -o ./Ex1 ./main.cpp
		elif [ -z "$6" ]; then
			#>&2 echo "mpic++ -O3 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -o ./Ex1 ./main.cpp"
			mpic++ -O3 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -o ./Ex1 ./main.cpp
		elif [ -z "$7" ]; then
			#>&2 echo "mpic++ -O3 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -D \"$6\" -o ./Ex1 ./main.cpp"
			mpic++ -O3 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -D "$6" -o ./Ex1 ./main.cpp
		else
			#>&2 echo "mpic++ -O3 -D NDEBUG -D \"kappa=$k\" -D \"$4\" -D \"$5\" -D \"$6\" -D \"$7\" -o ./Ex1 ./main.cpp"
			mpic++ -O3 -D NDEBUG -D "kappa=$k" -D "$4" -D "$5" -D "$6" -D "$7" -o ./Ex1 ./main.cpp
		fi
		echo -n "["
		for p in $2; do #for all numbers of processors
			#>&2 echo $p
			t=0.0
			#>&2 echo $t
			for j in $(seq 1 $1); do
				>&2 echo -n "$8/$9:$k:$p:$j "
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
procs='2 3 4 5 6 7'
kappas='100 200 300 400'
echo "procs=[$procs];"
echo -n "kappas=["
for k in $kappas; do
	echo $k
done
echo "];"

progress=0
let progress=progress+1
total=18

#I means the first question, a simple partition of the collumns
>&2 echo -e "\n%I"
echo -e "\n%I"
echo -n "t1="
conduct_test $tests "$procs" "$kappas" "__Question1__" "" "" "" "$progress" "$total"
let progress=progress+1 

#II means the second question, a shuffle partition of the collumns
>&2 echo -e "\n%II"
echo -e "\n%II"
echo -n "t2="
conduct_test $tests "$procs" "$kappas" "__Question2__" "" "" "" "$progress" "$total"
let progress=progress+1 

#Single proc means only 1 thread
>&2 echo -e "\n%I Single proc"
echo -e "\n%I Single proc"
echo -n "ts="
conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "" "" "$progress" "$total"
let progress=progress+1 

#Extra means doing the inverse instead of gaussian elimination
>&2 echo -e "\n%Extra I"
echo -e "\n%Extra I"
echo -n "te1="
conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%Extra II"
echo -e "\n%Extra II"
echo -n "te2="
conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%Extra I Single proc"
echo -e "\n%Extra I Single proc"
echo -n "tes="
conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__" "" "$progress" "$total"
let progress=progress+1 

#BTest means broadcast test, which means using broadcast instead of send and receive
>&2 echo -e "\n%BTest I"
echo -e "\n%BTest I"
echo -n "bt1="
conduct_test $tests "$procs" "$kappas" "__Question1__" "USE_BROADCAST" "" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%BTest II"
echo -e "\n%BTest II"
echo -n "bt2="
conduct_test $tests "$procs" "$kappas" "__Question2__" "USE_BROADCAST" "" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%BTest I Single proc"
echo -e "\n%BTest I Single proc"
echo -n "bts="
conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "USE_BROADCAST" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%BTest Extra I"
echo -e "\n%BTest Extra I"
echo -n "bte1="
conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%BTest Extra II"
echo -e "\n%BTest Extra II"
echo -n "bte2="
conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%BTest Extra I Single proc"
echo -e "\n%BTest Extra I Single proc"
echo -n "btes="
conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__" "USE_BROADCAST" "$progress" "$total"
let progress=progress+1 

#AsBTest means asynchronus broadcast test, same as broadcast test but asynchronus
>&2 echo -e "\n%AsBTest I"
echo -e "\n%AsBTest I"
echo -n "abt1="
conduct_test $tests "$procs" "$kappas" "__Question1__" "USE_BROADCAST_ASYNC" "" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%AsBTest II"
echo -e "\n%AsBTest II"
echo -n "abt2="
conduct_test $tests "$procs" "$kappas" "__Question2__" "USE_BROADCAST_ASYNC" "" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%AsBTest I Single proc"
echo -e "\n%AsBTest I Single proc"
echo -n "abts="
conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "USE_BROADCAST_ASYNC" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%AsBTest Extra I"
echo -e "\n%AsBTest Extra I"
echo -n "abte1="
conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST_ASYNC" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%AsBTest Extra II"
echo -e "\n%AsBTest Extra II"
echo -n "abte2="
conduct_test $tests "$procs" "$kappas" "__Question1__" "__QuestionExtra__" "USE_BROADCAST_ASYNC" "" "$progress" "$total"
let progress=progress+1 

>&2 echo -e "\n%AsBTest Extra I Single proc"
echo -e "\n%AsBTest Extra I Single proc"
echo -n "abtes="
conduct_test $tests "1" "$kappas" "__Question1__" "__SingleProc__" "__QuestionExtra__" "USE_BROADCAST_ASYNC" "$progress" "$total"
let progress=progress+1 
