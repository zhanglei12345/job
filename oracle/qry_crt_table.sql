--查看数据表的建表语句
select dbms_metadata.get_ddl('TABLE','T_ACC_FILE_REG') from dual;
