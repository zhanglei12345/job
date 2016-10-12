#!/bin/bash
a=1
echo "PID for pid1.sh before exec/source/fork:$$"
export a
echo "pid1.sh:\$a is $a"
case $1 in
    "exec")
        echo "using exec..."
        exec ./pid2.sh
    ;;
    "source")
        echo "using source..."
        source ./pid2.sh
    ;;
    *)
        echo "using fork by default..."
        ./pid2.sh
    ;;
esac
echo "PID for pid1.sh after exec/source/fork:$$"
echo "pid1.sh:\$a is $a"

#fork：子进程执行完毕后返回父进程，子进程从父进程继承环境变量，但是子进程中的环境变量不会带回给父进程
#exec与fork不同，不需要新开一个sub-shell来执行被调用的脚本。
#但是使用 exec调用一个新脚本以后，父脚本中exec行之后的内容就不再执行(即pid1.sh中最后两个echo不会执行),这是exec和source的区别
#source也不会新开一个sub-shell来执行被调用的脚本 
