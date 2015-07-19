red='\033[1;31m'
green='\033[1;32m'
NC='\033[0m' # No Color
echo  "${red}WARNING: data-base have been freed !\n\
- Data-base backup available (memorie_dump/rescue.bin)${NC}"
wgdb export -f memorie_dump/rescue.bin
echo  "${green}- Shared memorie allocated${NC}"
sudo sysctl kernel.shmmax=1000000000 > /dev/null
wgdb AI_gs free
