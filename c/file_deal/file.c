#include <stdio.h>
#include <string.h>
#include "getfile.h"
char src_buf[512];
long p;
int main()
{
    long count=0;  //文件中每行数据用|+|分割的字段数
    long l=0;     //文件行数
    long ret=0;
    char *bgn;
    char *end;
    FILE *fp=NULL;
    FILE *print_out=NULL;
    char tmp_str[512];
    fp=fopen("/Users/zhanglei/Study/job/c/file_deal/file.txt","r");
    print_out=fopen("/Users/zhanglei/Study/job/c/file_deal/file_out.txt","a+");
    if(fp==NULL || print_out==NULL)
    {
        return 1;
    }
    fprintf(print_out,"--------------------------------------------\n");
    //循环处理每行数据
    while(1)
    {
        memset(src_buf,0x00,sizeof(src_buf));
        p=0;
		count=0;
		//fgets读取文件的时候，若bufsize大于该行的字符总数加2('\n','\0'),文件仅仅只是这一行读取完，随后指向文件的指针会自动偏移至下一行
        if(fgets(src_buf,sizeof(src_buf),fp)==NULL)
        {
            if(0==l)
			{
				fprintf(print_out,"文件为空!");
				return 1;
			}
			else
			{
                fprintf(print_out,"文件读取完毕!\n");
                break;
	    	}
        }
		l++;
    //src_buf存储每行数据
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
            //获取每个字段
	    	ret=getfl(tmp_str);
	    	if(0!=ret)
			fprintf(print_out,"getfl调用失败!");
	    	fprintf(print_out,"tmp_str:%s\n",tmp_str);
		}
    }
    fprintf(print_out,"文件行数:[%d]\n",l);
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

//输出结果
/*
src_buf:[asd|+|qwe|+|zxc|-|
]
p=[15],src_buf[p]=[|],count=[3]
src_buf:[123|+|456|-|
]
p=[9],src_buf[p]=[|],count=[2]
gets fail
文件行数:[2]
*/
