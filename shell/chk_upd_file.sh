if [ -z "$1" -o -z "$2" -o -z "$3" -o -z "$4" -o -z "$5" -o -z "$6" ]
then
    echo "Usage:`basename $0` file_path(�ļ�·��) out_file_path(�����ļ�·����file_name(�ļ���) oracle_name(���ݿ���) oracle_pwd(���ݿ�����) oracle_sid(���ݿ�ʵ��)"
    exit -1;
fi

file_path=$1
out_file_path=$2
file_name=$3
oracle_name=$4
oracle_pwd=$5
oracle_sid=$6

exit_value=0

#ƴװ����ļ����� 
kj_file_name=${file_name:0:32}0${file_name:34:2}.kj
lj_file_name=${file_name}.lj
res_file_name=${file_name}.res
echo "$kj_file_name"
echo "$lj_file_name"
echo "$res_file_name"

rm -f $file_path/$lj_file_name

date
echo "������ʼ .... "
echo "oracle_name:${oracle_name}" >>$out_file_path/out.txt
echo "oracle_pwd:${oracle_pwd}" >>$out_file_path/out.txt
echo "oracle_sid:${oracle_sid}" >>$out_file_path/out.txt
#��ȡ���κ�  ��Ϊ������ 
part_num=0${file_name:0-11:2}
echo "part_num:$part_num" >>$out_file_path/out.txt

sql_ldf="SELECT a.acc,
		a.sys_code,
		' ',
		a.curr_type,
		NVL(b.int_bgn_date,' '),
		NVL(a.due_date,' '),
		NVL(b.rate_type,' '),
		NVL(b.nom_rate1,0.00),
		NVL(b.nom_rate_no1,' '),
		NVL(a.ce_flag,' '),
		NVL(a.acc_type,' '),
		NVL(a.product_type,' '),
		NVL(a.deadline,' '),
		NVL(a.cstm_trade,' '),
		NVL(a.amt_use,' '),
		NVL(a.out_flag,' '),
		'01',
		NVL(a.itm_no,' '),
		NVL(a.acc_inst,' ')
	FROM t_acc_fix_ledger partition(PART$part_num) a,t_acc_fix_int_ledger partition(PART$part_num) b
	WHERE  a.acc = b.acc
	AND a.stat = '0' "

while [ -f $out_file_path/${kj_file_name}.tmp ]
do
	sleep 5
	echo "���ڽ��е���1" >> $out_file_path/ps.txt
done

if [ ! -f $out_file_path/$kj_file_name ];then
	rm -f $out_file_path/${kj_file_name:0:20}*${kj_file_name:30:5}.kj
	touch $out_file_path/$kj_file_name 
	touch $out_file_path/${kj_file_name}.tmp 
	#�����ű� �����ݿ����ݿ��ٵ��� 
	echo "�����ļ�������" >> $out_file_path/out.txt
	date
	output_data userid=${oracle_name}/${oracle_pwd}${oracle_sid} sqlstmt="${sql_ldf}"  arraysize=99 > $out_file_path/${kj_file_name}.01
	#echo "$?" >>$out_file_path/output_data.txt
	exit_value=$?
	echo "$exit_value" >$out_file_path/output_data.txt
	#if [$? -ne 0];then
	#	exit -1 
	#fi
	date
	echo "�����ļ����" >> $out_file_path/out.txt
	#����ո�
	sed 's/|-|/|/g;s/ //g;s/+/ /g' $out_file_path/${kj_file_name}.01 | sort > $out_file_path/$kj_file_name
	rm -f $out_file_path/${kj_file_name}.01
	date
	echo "�ļ���Ϊ$out_file_path/$kj_file_name ������ɣ� " >> $out_file_path/out.txt

	rm -f $out_file_path/${kj_file_name}.tmp
else 
	echo "�����ļ��Ѵ���" >> $out_file_path/out.txt
fi

if [ "$exit_value" !=  "0" ];then
	exit 1 
fi

while [ -f $out_file_path/${kj_file_name}.tmp ]
do
	sleep 5
	echo "���ڽ��е���2" >> $out_file_path/ps.txt
done

echo "diff ��ʼ"
#�Ƚ����ļ���ͬ 
sed  's/|-|.*$/|/g;s/ //g;s/+/ /g' $file_path/$file_name | sort >$file_path/$lj_file_name
#diff $file_path/$lj_file_name $out_file_path/$kj_file_name| grep "< " > $file_path/${res_file_name}.01
join -j1 1 -j2 1 $file_path/$lj_file_name $out_file_path/$kj_file_name |awk '$2!=$18||$3!=$19||$4!=$20||$5!=$21||$6!=$22||$7!=$23||$8!=$24||$9!=$25||$10!=$26||$11!=$27||$12!=$28||$13!=$29||$14!=$30||$15!=$31||$16!=$32||$17!=$33{print $0}'>$file_path/${res_file_name}.02
sed 's/< //g;s/$/-|/g;s/ /+/g' $file_path/${res_file_name}.02 > $file_path/$res_file_name
rm -f $file_path/${res_file_name}.01
rm -f $file_path/${res_file_name}.02
rm -f $file_path/$lj_file_name
date
echo "���! ����ļ� $file_path/$res_file_name !"
