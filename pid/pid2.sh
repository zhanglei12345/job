#!/bin/bash
echo "PID for pid2.sh:$$"
echo "pid2.sh get \$a=$a from pid1.sh"
a=2
export a
echo "pid2.sh:\$a is $a"
