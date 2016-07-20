#!/bin/bash

git add --all
git commit -m "update"
git push origin master
ssh pi@192.168.0.131 'cd /home/pi/GitRepo/sketchbook/My\ Projects/Bedroom_Control/; git pull'
