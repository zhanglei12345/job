load data
 infile *
 replace into table student
(stuno terminated by '|+|',
stuname terminated by '-',
sex terminated by '!',
old terminated by '|-|'
)
begindata
1007|+|HEHE-��!15|-|
1008|+|HAHA-Ů!15|-|
