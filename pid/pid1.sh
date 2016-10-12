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

#fork���ӽ���ִ����Ϻ󷵻ظ����̣��ӽ��̴Ӹ����̼̳л��������������ӽ����еĻ�������������ظ�������
#exec��fork��ͬ������Ҫ�¿�һ��sub-shell��ִ�б����õĽű���
#����ʹ�� exec����һ���½ű��Ժ󣬸��ű���exec��֮������ݾͲ���ִ��(��pid1.sh���������echo����ִ��),����exec��source������
#sourceҲ�����¿�һ��sub-shell��ִ�б����õĽű� 
