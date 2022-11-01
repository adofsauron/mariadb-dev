#!/bin/bash

PID=`ps -ef | grep mariadbd | grep -v grep | awk -F ' ' '{print $2}'`

gdb -p $PID


