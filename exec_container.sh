#!/bin/bash

# if arg is gpu or cpu, ok, else exit
if [ "$1" != "cpu" ] && [ "$1" != "gpu" ]; then
  echo "Invalid argument supplied"
  echo "Usage: ./exec_container.sh [cpu|gpu]"
  exit 1
fi

# set display environment variable
xhost +
DISPLAY=:0

SERVICE_NAME="opencv"
CONTAINER_NAME="opencv-$1"

if [ "$(docker ps -aq -f name=$SERVICE_NAME)" ]; then
  # Check if the container is running
  if [ "$(docker inspect -f '{{.State.Running}}' $CONTAINER_NAME)" = "true" ]; then
    # Container is already running, attach to its terminal
    docker attach $CONTAINER_NAME
  else
    # Container is stopped, start it and attach to its terminal
    docker start $CONTAINER_NAME
    docker attach $CONTAINER_NAME
  fi
else
  # Container doesn't exist, run it
  docker compose -f docker-compose.$1.yml run --name $CONTAINER_NAME $SERVICE_NAME
fi
