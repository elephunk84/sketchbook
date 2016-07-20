#!/bin/bash
echo "Update Git Repo"
git add --all
git commit -m "update"
git push origin master
echo "Update Remote"
ssh pi@192.168.0.131 'cd /home/pi/GitRepo/sketchbook/My\ Projects/Bedroom_Control/; git pull'
