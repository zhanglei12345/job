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


# 0(标准输入) 1(标准输出) 2(标准错误输出)
# exec命令：常用来替代当前shell并重新启动一个shell，但并没有启动子shell。
#           使用这一命令时任何现有环境都将被清楚，exec在对文件描述符进行操作的时候，也只有在这时，exec不会覆盖当前shell环境。

# exec 0<infilename ：打开文件infilename作为stdin
# <&n :标准输入复制自文件描述符n
# exec 0<&- ：关闭FD0(文件描述符)

# >&n  :复制文件描述符n并把结果用作标准输出
# <&n  :标准输入复制自文件描述符n
# <&-  :关闭标准输入(键盘)
# n<&- :n号输入关闭
# >&-  :关闭标准输出
# n>&- :n号输出关闭
