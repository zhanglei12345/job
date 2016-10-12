--查看数据表的字段
select * from user_tab_columns where TABLE_NAME = UPPER('student') order by column_id;

select decode ( column_id, 1, '',',')||lower(column_name) from user_tab_columns where TABLE_NAME = UPPER('student') order by column_id;