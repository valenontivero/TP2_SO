#!/bin/bash

# Check if docker so_tp2 exists
if [ ! "$(docker ps -a -q -f name=so_tp2)" ]; then
    # Create docker so_tp2
    docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name so_tp2 agodio/itba-so-multi-platform:3.0
fi

docker start so_tp2
docker exec -it so_tp2 make clean -C/root/Toolchain
docker exec -it so_tp2 make clean -C/root/
docker exec -it so_tp2 make -C/root/Toolchain
if [[ "$1" = "buddy" ]]; then
    docker exec -it so_tp2 env GCC_COLORS="error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01" make buddy -C/root/
else
    docker exec -it so_tp2 env GCC_COLORS="error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01" make -C/root/
fi
docker exec -it so_tp2 chmod -R g+wx /root/
# docker stop so_tp2
docker exec -it so_tp2 chmod -R 777 /root/Image
bash run.sh