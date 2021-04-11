serverDir=server/
clientDir=client/
binDir=bin/

compileServer() {
    echo -e "\033[0;90m"
    make -C $serverDir
    r=$?
    echo -e "\033[0m"
    return $r
}

compileClient() {
    echo -e "\033[0;90m"
    make -C $clientDir
    r=$?
    echo -e "\033[0m"
    return $r
}

executeServer() {
    path=./$serverDir$binDir
    exe=$(find $path -executable -type f)

    echo -e "\033[1;32m"
    echo -e "\nExecuting the server :\n"
    echo -e "\033[0m"

    #if [ ! -z "$exe" ]; then
        $exe&
        return $?
    #fi

    return 1
}

killClient(){
    id=$(ps -au | grep "./client/bin/ftp_client localhost 2121" | head -1 | cut -d" " -f5)
    kill $id

    if [ $? -eq 0 ]; then
        echo "Killed the client"
    else
        echo "Failed to kill the client"
    fi
}

killServer() {
    id=$(ps | grep ftp_server | cut -d" " -f1 | head -1)
    kill -2 $id
    if [ $? -eq 0 ]; then
        echo "Killed the server"
    else
        echo "Failed to kill the server"
    fi
}

executeClient() {
    path=./$clientDir$binDir
    exe=$(find $path -executable -type f)

    echo -e "\033[1;32m"
    echo -e "\nExecuting the client :\n"
    echo -e "\033[0m"
    
    #if [ ! -z "$exe" ]; then
        $exe localhost
        return $?
    #fi

    return 1
}

launchServer() {
    compileServer
    if [ $? -eq 0 ]; then
        executeServer
        return $?
    else
        echo "Failed to compile the server."
    fi

    return 1
}

launchClient() {
    compileClient
    if [ $? -eq 0 ]; then
        executeClient
        return $?
    else
        echo "Failed to compile the client."
    fi

    return 1
}

restartServer(){
    killServer
    launchServer
}


if [ $# -eq 0 ]; then
    launchServer
    if [ $? -eq 0 ]; then
        launchClient
    fi
else
    if [ $# -eq 1 ] && [ $1 == "-h" -o $1 == "--help" ]; then
        echo -e "$0\t\t: compile and execute both and in order the server and the client"
        echo -e "$0 client\t\t: compile and launch the client"
        echo -e "$0 server\t\t: compile and launch the server"
        echo -e "$0 server restart : restart the server"
        echo -e "$0 server kill\t: kill the server"
        echo -e "$0 client kill\t: kill the client"
        exit 0
    fi


    if [ $1 == "client" ]; then
        if [ $# -gt 1 ]; then
            if [ $2 == "kill" ]; then
                killClient
            fi
        else
            launchClient
        fi
    elif [ $1 == "server" ]; then
        if [ $# -gt 1 ]; then
            if [ $2 == "kill" ]; then
                killServer
            elif [ $2 == "restart" ]; then
                restartServer
            fi
        else
            launchServer
        fi 
    else
        echo "Unrecognized argument $1. Stop."
    fi
fi