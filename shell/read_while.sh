#!/bin/sh
count=1
cat read_while.txt | while read line
do
    echo "line $count:$line"
    count=$[$count + 1]
done

cat read_while.txt | while read para1 para2
do
    echo "line $count:$para1"
    echo "line $count:$para2"
    count=$[$count + 1]
done

while read line
do
    echo "line $count:$line"
    count=$[$count + 1]
done < read_while.txt
