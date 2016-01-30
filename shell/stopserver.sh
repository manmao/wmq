#/bin/sh
ps -ef | grep 'wfs' | awk '{print $1}' | while read pid
do
   kill -9 $pid
done
