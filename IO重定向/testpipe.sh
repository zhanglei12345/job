#!/bin/bash
if [ $# -gt 0 ];then
    #�����ļ��󶨵���׼����
    exec 0<$1
fi

while read line
do
    echo "[$line]"
#��׼���븴�����ļ������� 0
done <&0

#�����׼�����
exec 0<&-


#exec 0<infilename �����ļ�infilename��Ϊstdin
#<&n :��׼���븴�����ļ�������n
#exec 0<&- ���ر�FD0(�ļ�������)
