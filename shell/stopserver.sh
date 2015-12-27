#关闭wfs进程
ps -ef |grep wfs |awk '{print $2}' | while read pid
do
        kill -9 $pid
done
