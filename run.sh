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
        launchServer
    else
        echo "Unrecognized argument $1. Stop."
    fi
fi