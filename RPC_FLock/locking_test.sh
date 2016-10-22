#!/bin/bash

lock_file="public/file.txt"

sleep 2

for i in `seq 1 30`; do
    lock_type=$(( ( RANDOM % 2 )  + 1 ))
    lock_start=$(( ( RANDOM % ( $((`wc -c <"$lock_file"` - 20)) ) )  + 1 ))
    lock_length=$(( ( RANDOM % 380 ) + 20 ))
    bin/RPC_FLock_client localhost $lock_file $lock_type $lock_start $lock_length &
    sleep 0.3
done

sleep 12
