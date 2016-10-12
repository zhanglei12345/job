--查看所有表空间的大小
select T.tablespace_name,T.space,F.space1,round(F.space1/T.space,4)*100
from
(select tablespace_name,nvl(round(sum(bytes)/1024/1024,3),0) space from dba_data_files group by tablespace_name) T,
(select tablespace_name,nvl(round(sum(bytes)/1024/1024,3),0) space1 from dba_free_space group by tablespace_name) F
where T.tablespace_name=F.tablespace_name
order by 1 