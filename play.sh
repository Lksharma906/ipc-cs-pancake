#! /bin/bash

pwd



cd ./Server
./server &
cd -


cd ./Client 
./run.sh &
cd -

exit 0

