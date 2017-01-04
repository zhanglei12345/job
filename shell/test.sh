#!/bin/bash

# 编写函数
function example { 
	echo "Hello world!" 
}

example

# 如何连接两个字符串
V1="Hello" 
V2="World" 
V3=${V1}${V2} 
echo $V3

# 如何获取变量长度
echo ${#V3}

# 如果之前没有给V3赋值则输出8;如果有赋值则输出该变量
echo ${V3:-8}
echo ${V4:-8}

# 输出V3的最后8个字符
echo ${V3:0-8}

# 两个整数相加
A=5
B=6
let C=$A+$B
echo $C		# 方法 1
echo $(($A+$B)) # 方法 2 
echo $[$A+$B] # 方法 3 
expr $A + $B # 方法 4 
echo $A+$B | bc # 方法 5 
awk 'BEGIN{print '"$A"'+'"$B"'}' # 方法 6

# 利用echo命令获取字符串变量的一部分
variable="My name is Petras, and I am developer." 
echo ${variable:11:6} # 变量:起始位置:长度.会显示 Petras

var="http://www.aaa.com/123.htm"
echo ${var#*//} # var 是变量名，# 号是运算符，*// 表示从左边开始删除第一个 // 号及左边的所有字符
echo ${var##*/} # ##*/ 表示从左边开始删除最后（最右边）一个 / 号及左边的所有字符
echo ${var%/*}  # %/* 表示从右边开始，删除第一个 / 号及右边的字符
echo ${var%%/*} # %%/* 表示从右边开始，删除最后（最左边）一个 / 号及右边的字符
echo ${var:0:5} # 0 表示左边第一个字符开始，5 表示字符的总个数
echo ${var:7}	# 7 表示左边第8个字符开始，一直到结束
echo ${var:0-7:3} #  0-7 表示右边算起第七个字符开始，3 表示字符的个数
echo ${var:0-7}	# 从右边第七个字符开始，一直到结束

# 利用echo命令替换字符串的一部分
str="apple, tree, apple tree, tree.apple"
echo ${str/apple/APPLE}	# 使用APPLE来替换第一个匹配的apple
echo ${str//apple/APPLE}	# 使用APPLE来替换所有匹配的apple
echo ${str/#apple/APPLE}	# 如果变量str的前缀匹配apple，那么就用APPLE来代替匹配到的apple
echo ${str/%tree\.apple/TREE\.APPLE} # 如果变量str的后缀匹配tree.apple，那么就用TREE.APPLE来代替匹配到的tree.apple

# 定义数组
array=("Hi" "my" "name" "is")
echo ${array[0]}	# 打印数组的第一个元素
echo ${array[@]}	# 打印数组的所有元素
echo ${!array[@]}	# 输出所有数组索引
unset array[2]		# 移除数组中索引为2的元素
echo ${array[@]}	# 打印数组的所有元素