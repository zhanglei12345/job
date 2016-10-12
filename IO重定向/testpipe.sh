#!/bin/bash
if [ $# -gt 0 ];then
    #将该文件绑定到标准输入
    exec 0<$1
fi

while read line
do
    echo "[$line]"
#标准输入复制自文件描述符 0
done <&0

#解除标准输入绑定
exec 0<&-


#exec 0<infilename ：打开文件infilename作为stdin
#<&n :标准输入复制自文件描述符n
#exec 0<&- ：关闭FD0(文件描述符)
