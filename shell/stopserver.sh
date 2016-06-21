#/bin/sh
kill -9 $( ps -e|grep wmq |awk '{print $1}')
