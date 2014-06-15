#!/bin/bash

#for all variables
for var in "$@"
do
	#--arg-file="$var" file gives it the file to read input from
	#-L1 means it will apply it to each line
	#--no-run-if-empty means it will ignore empty lines
	#--verbose means it will output the command in stdout before running it
	#So the end result will be "./jobCommander issuejob <whatever was in that line>" for each line of each file
	xargs --arg-file="$var" -L1 --no-run-if-empty --verbose ./jobCommander issuejob
done
