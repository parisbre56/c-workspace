#!/bin/bash

#kills all queued jobs

#This first reads all queued jobs
#It then uses sed to find the jobID of that job and print only that
#The -r allows advanced regular expressions
#The -n makes it silent (doesn't print lines) and the p at the end makes it print modified lines despite the -n
#Then xargs tells it to stop
#If a job happens to pass to the running queue, then that's no problem, because we'll catch it below 
#The commander simply complains about the invalid IDs, it doesn't crash or anything
./jobCommander poll queued | sed -r -n "s/JobID: ([0-9]+).*/\1/p" | xargs -L1 --no-run-if-empty --verbose ./jobCommander stop

#kills all running jobs
./jobCommander poll running | sed -r -n "s/JobID: ([0-9]+).*/\1/p" | xargs -L1 --no-run-if-empty --verbose ./jobCommander stop
