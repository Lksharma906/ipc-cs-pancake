
#! /bin/bash

# Menu Script to test the client server IPC application code .
# Below will the menu options 
# Build server
# Build Client
# Build Venders ==> Venders are integral part of Server so need to take care them along side server.
# Run Server 
# Run Client is not part of this process as to test extensively running it from a seprate script which is part of client itself.


#Unlink done so when ever we are going to run server again request_fifo is created as fresh.
unlink request_fifo

echo

build_server()
{
    read -n1 -p "Do you want to build server (y/n)?"
    if [ $REPLY = 'y' ]
    then
        make clean -C ./Server
        echo "Make Clean Done before make. "

        if( make -C ./Server )
        then
            echo " Server build successfull "
        else
            echo " ERROR: server build failed "
            read
        fi
    fi
}

build_client()
{
    read -n1 -p "Do you want to build client (y/n)?"
    if [ $REPLY = 'y' ]
    then
        make clean -C ./Client
        echo "Make clean done before make. "

        if( make -C ./Client)
        then 
            echo "Client Build Successfully "
        else
            echo "ERROR: Client Build Failed. "
            read
        fi 
    fi
}

build_venders()
{
    
    pwd
    read -n1 -p "Do you want to build Venders (y/n)?"
    if [ $REPLY = 'y' ]
    then
        make clean -C ./Server/Vender/adder
        make clean -C ./Server/Vender/sub
        make clean -C ./Server/Vender/mod
        make clean -C ./Server/Vender/mul
        make clean -C ./Server/Vender/div
        echo "Make clean done before make. "

        if( make -C ./Server/Vender/adder)
        then 
            echo "Vender adder Build Successfully "
        else
            echo "ERROR: adder Build Failed. "
            read
        fi

        if( make -C ./Server/Vender/mul)
        then 
            echo "Vender mul Build Successfully "
        else
            echo "ERROR: mul Build Failed. "
            read
        fi 

        if( make -C ./Server/Vender/div)
        then 
            echo "Vender div Build Successfully "
        else
            echo "ERROR: div Build Failed. "
            read
        fi

        if( make -C ./Server/Vender/sub)
        then 
            echo "Vender sub Build Successfully "
        else
            echo "ERROR: sub Build Failed. "
            read
        fi 

        if( make -C ./Server/Vender/mod)
        then 
            echo "Vender mod Build Successfully "
        else
            echo "ERROR: mod Build Failed. "
            read
        fi 
    fi

}

clean_all()
{
    make clean -C ./Server
    make clean -C ./Client
    make clean -C ./Server/Vender/adder
    make clean -C ./Server/Vender/sub
    make clean -C ./Server/Vender/mod
    make clean -C ./Server/Vender/mul
    make clean -C ./Server/Vender/div
}
run_server()
{
    echo 
    read -n1 -p "Do you want to run server(y/n)?"
    if [ $REPLY = 'y' ]
    then
        ./Server/server
    fi
}


#buildServer
#runServer



##
# Color  Variables
##
green='\e[32m'
blue='\e[34m'
clear='\e[0m'

##
# Color Functions
##

ColorGreen(){
	echo -ne $green$1$clear
}
ColorBlue(){
	echo -ne $blue$1$clear
}

menu(){
echo -ne "
My First Menu
$(ColorGreen '1)') Build Server
$(ColorGreen '2)') Build Client
$(ColorGreen '3)') Build Venders. 
$(ColorGreen '4)') Run Server.
$(ColorGreen '5)') Clean All.
$(ColorGreen '0)') Exit
$(ColorBlue 'Choose an option:') "
        read a
        case $a in
	        1) build_server ; menu ;;  
            2) build_client ; menu ;; 
	        3) build_venders ;    menu ;;
            4) run_server ; menu;;
            5) clean_all ; menu ;;
		0) exit 0 ;;
		*) echo -e $red"Wrong option."$clear; WrongCommand;;
        esac
}

# Call the menu function
menu











