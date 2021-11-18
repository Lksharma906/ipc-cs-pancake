
#! /bin/bash

unlink request_fifo

echo

buildServer()
{
    read -n1 -p "Do you want to build server (y/n)?"
    if [ $REPLY = 'y' ]
    then
        if( make -C ./Server )
        then
            echo " Server build successfull "
            mv ./Server/server . 
        else
            echo " ERROR: server build failed "
            read
        fi
    fi


}




runServer()
{
    echo 
    read -n1 -p "Do you want to run server(y/n)?"
    if [ $REPLY = 'y' ]
    then
        ./server
    fi
}


buildServer
runServer














