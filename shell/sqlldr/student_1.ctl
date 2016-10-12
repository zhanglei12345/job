load data
 infile '/app/accsedyw/priv/zl/student.txt'
 append into table student
(stuno position(01:04),
stuname position(06:09),
sex position(11:12),
old position(14:15)
)
