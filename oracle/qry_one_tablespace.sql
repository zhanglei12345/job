--�鿴ĳ��ռ��б��������ռ�õĿռ�
select substr(owner,1,10),substr(SEGMENT_NAME,1,30),SUM(bytes/1024/1024)
from dba_segments where TABLESPACE_NAME ='TBS_IDX02'
  GROUP BY substr(owner,1,10),substr(SEGMENT_NAME,1,30);
select * from dba_segments;