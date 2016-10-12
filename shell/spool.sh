#!/bin/bash
    #��Ϣ�ɱ��е����ܽ���ļ�����
if [ -z "$1" ]; then
    echo "Usage:  sh(������)  date_in(����)"
    exit -1
fi
    
file_out_path="/app/intsedyw/priv/zhanglei/"
date_in=$1
part_num="P${date_in:0:6}"
start_time=$(date)
echo "��Ϣ�ɱ��е����ܽ���ļ�����--������:[$part_num] ��ʼ!��ʼʱ��:$start_time"

sqlplus -S $INIT_USING_DBASE/$INIT_USING_DBASEPWD 2>/dev/null <<EOF
set heading off     
set feedback off
set termout off
set echo off
set pagesize 0
set termout on
set trimspool on
spool ${file_out_path}ledger.txt.tmp
SElECT acc_date||'|+|'||inst_no||'|+|'||itm_no||'|+|'||curr_type||'|+|'||pay_type||'|+|'||int_cost_amt||'|+|'||tol_bal||'|-1'
	FROM t_int_payint_cost_sum2 partition($part_num)
	WHERE acc_date='$1' ORDER BY acc_date,inst_no,itm_no,curr_type,pat_type; 
spool off
EOF
sed "s/ //g" ${file_out_path}ledger.txt.tmp > ${file_out_path}ledger.txt
rm -f ${file_out_path}ledger.txt.tmp
end_time=$(date)
echo "��Ϣ�ɱ��е����ܽ���ļ�����--������:[$part_num] ���!���ʱ��:$end_time"


