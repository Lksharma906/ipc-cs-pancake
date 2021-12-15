#! /bin/bash

count=0
while [ $count -lt 1000 ]
do
	./client 1 2 ADD &
	./client 2 1 SUB &
	./client 2 3 MUL &
	./client 54 5 DIV &
	./client 4 1 MOD &
	# increment the value
    	count=`expr $count + 1`
	echo "Iteration no $count "
done


