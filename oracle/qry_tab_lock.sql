查看当前系统中的索表情况：
select sess.sid,sess.serial#,sess.machine,
	lck.oracle_username,lck.os_user_name,lck.locked_mode,
	obj.object_name,obj.object_type
from v$locked_object lck,v$session sess,dba_objects obj
where obj.object_id = lck.object_id
and lck.session_id = sess.sid
order by obj.object_id;

查询是什么SQL引起了锁表的原因：
select l.session_id, 
       s.serial#, 
       l.locked_mode, 
       l.oracle_username, 
       s.user#, 
       l.os_user_name, 
       s.machine, 
       s.terminal, 
       a.sql_text, 
       a.action 
from v$sqlarea a, v$session s, v$locked_object l 
where l.session_id = s.sid 
and s.prev_sql_addr = a.address 
order by l.session_id, s.serial#;

解锁的方法：
alter system kill session 'sid,serial#';
