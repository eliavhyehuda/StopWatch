#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Please supply the name of an app (e.g., $0 a1)"
    echo "to run with debug logging enabled."
    exit
fi

make
# Set to 100, higher than anything we've defined
APP_NAME=$1
shift
ARGS=$@
GLOG_v=100 $APP_NAME $ARGS
