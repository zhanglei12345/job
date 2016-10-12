#!/bin/bash
RemoveDir=/kjapp/inttest01/tmp/print
declare -i dt=`date +%Y%m%d -d "3 day ago"`
for subdir in `ls $RemoveDir`
do
    if [ ${subdir} -lt ${dt} ];then
        rm -rf $RemoveDir/$subdir
    fi
done

#删除指定目录下的日期目录（指定目录下只存在日期目录）
