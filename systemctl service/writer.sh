#!/bin/sh

counter=0

while : ; do
    echo "$counter: Hi! I'm systemctl service"
    counter=$((counter+1)) 
    sleep 5
done
