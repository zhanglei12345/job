FUNC_ALTER_EXP_STAT_FLAG(){
sqlplus -s $INIT_USING_DBASE/$INIT_USING_DBASEPWD <<EOF
set pages 0;
set feedback off;
select trim(count(*)) from  t_int_file_reg   
        WHERE  file_date = '20151117' 
        AND     deal_flag = '0';
commit;
EOF
}

stat1=`FUNC_ALTER_EXP_STAT_FLAG`
echo "$stat1"

#set pages 0;
#set heading off;
