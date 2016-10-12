#include <stdio.h>
#include <string.h>
#include "getfile.h"
char src_buf[512];
long p;
int main()
{
    long count=0;
    long l=0;
    long ret=0;
    char *bgn;
    char *end;
    FILE *fp=NULL;
    FILE *print_out=NULL;
    char tmp_str[512];
    fp=fopen("/app/intsedyw/priv/zhanglei/file.txt","r");
    print_out=fopen("/app/intsedyw/priv/zhanglei/file_out.txt","a+");
    fprintf(print_out,"--------------------------------------------\n");
    if(fp==NULL || print_out==NULL)
    {
	fprintf(print_out,"open fail\n");
        return 1;
    }
    while(1)
    {
        memset(src_buf,0x00,sizeof(src_buf));
        p=0;
	count=0;
	//fgets��ȡ�ļ���ʱ����bufsize���ڸ��е��ַ�������2('\n','\0'),�ļ�����ֻ����һ�ж�ȡ�꣬���ָ���ļ���ָ����Զ�ƫ������һ��
        if(fgets(src_buf,sizeof(src_buf),fp)==NULL)
        {
            if(0==l)
	    {
		fprintf(print_out,"�ļ�Ϊ��!");
		return 1;
	    }
	    else
	    {
                fprintf(print_out,"�ļ���ȡ���!\n");
                break;
	    }
        }
	l++;
	rtrim(src_buf);
        fprintf(print_out,"src_buf:[%s]\n",src_buf);
        end=strstr(src_buf,"|-|");
        while(1)
        {
            bgn = (char *)&src_buf[p];
            char *prt=strstr(bgn,"|+|");
            if( prt != NULL)
            {
                p+=prt-bgn+3;
                count++;
            }
            else
            {
                p+=end-bgn;
                count++;
                break;
            }
        }
        fprintf(print_out,"p=[%d],src_buf[p]=[%c],count=[%d]\n",p,src_buf[p],count);
	p=0;
	for(;count>0;count--)
	{
            memset(tmp_str,0x00,sizeof(tmp_str));
	    ret=getfl(tmp_str);
	    if(0!=ret)
		fprintf(print_out,"getfl����ʧ��!");
	    fprintf(print_out,"tmp_str:%s\n",tmp_str);
	}
    }
    fprintf(print_out,"�ļ�����:[%d]\n",l);
    fprintf(print_out,"--------------------------------------------\n");
    fclose(fp);
    fclose(print_out);
    return 0;
}

//file.txt
/*
asd|+|qwe|+|zxc|-|
123|+|456|-|
*/

//������
/*
src_buf:[asd|+|qwe|+|zxc|-|
]
p=[15],src_buf[p]=[|],count=[3]
src_buf:[123|+|456|-|
]
p=[9],src_buf[p]=[|],count=[2]
gets fail
�ļ�����:[2]
*/

