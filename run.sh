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
    id=$(ps | grep ftp_server | cut -d" " -f2)
    kill $id
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


if [ $# -eq 0 ]; then
    launchServer
    if [ $? -eq 0 ]; then
        launchClient
    fi
else
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
            fi
        else
            launchServer
        fi 
    else
        echo "Unrecognized argument $1. Stop."
    fi
fi