#!/bin/bash

PROCESSES=20

if [ "$#" -eq 1 -a "$1" == "polluks" ]; then
    mpirun -np $PROCESSES -hostfile machines --map-by slot debug/Monitor
else
    mpirun -np $PROCESSES debug/Monitor
fi
