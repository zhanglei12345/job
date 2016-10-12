#!/bin/bash
    #付息成本承担汇总结果文件生成
if [ -z "$1" ]; then
    echo "Usage:  sh(工具名)  date_in(日期)"
    exit -1
fi
    
file_out_path="/app/intsedyw/priv/zhanglei/"
date_in=$1
part_num="P${date_in:0:6}"
start_time=$(date)
echo "付息成本承担汇总结果文件生成--分区号:[$part_num] 开始!开始时间:$start_time"

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
echo "付息成本承担汇总结果文件生成--分区号:[$part_num] 完成!完成时间:$end_time"


