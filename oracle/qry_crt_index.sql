--查看索引的建索引语句
select dbms_metadata.get_ddl('INDEX','T_ACC_FILE_REG_IDX') from dual;
