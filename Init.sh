#!/bin/bash

red='\e[31m'
yellow='\e[33m'
green='\e[32m'
NC='' # No Color
echo -e "${yellow}WARNING: data-base have been freed !\n\
- Data-base backup available (memorie_dump/rescue.bin)${NC}"
wgdb export -f memorie_dump/rescue.bin

if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform        
	sudo sysctl -w kern.sysv.shmmax=1073741824
	sudo sysctl -w kern.sysv.shmall=262144
	echo -e "${green}- Shared memorie allocated(OSX)${NC}"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under Linux platform
	sudo sysctl kernel.shmmax=1000000000
	echo -e "${green}- Shared memorie allocated(linux)${NC}"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under Windows NT platform
	echo -e "${red}[ERROR]No windows support !${NC}"
fi
wgdb aeonDB free
wgdb create -l
