passwd=accsedywdb
user=accsedywdb@kjcondb2

next_date=`echo "set head off;
set pages 0;
set feedback off;
select to_char(to_date(acc_date,'yyyymmdd')+1,'yyyymmdd') from t_pub_sysdate;
exit" | sqlplus -S $user/$passwd`

echo "next_date=[$next_date]"
