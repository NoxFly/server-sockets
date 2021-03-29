serverDir=server/
clientDir=client/
binDir=bin/

compileServer() {
    make -C $serverDir
    return $?
}

compileClient() {
    make -C $clientDir
    return $?
}

executeServer() {
    path=./$serverDir$binDir
    exe=$(find $path -executable -type f)

    #if [ ! -z "$exe" ]; then
        $exe&
        return $?
    #fi

    return 1
}

killServer() {
    id=$(ps | grep ftp_server | cut -d" " -f2)
    kill $id
    echo "Killed the server"
}

executeClient() {
    path=./$clientDir$binDir
    exe=$(find $path -executable -type f)
    
    #if [ ! -z "$exe" ]; then
        $exe localhost 2121
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
        launchClient
    elif [ $1 == "server" ]; then
        if [ $2 == "kill" ]; then
            killServer
        else
            launchServer
        fi 
    else
        echo "Unrecognized argument $1. Stop."
    fi
fi