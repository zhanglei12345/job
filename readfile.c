#include <stdio.h>
#include <string.h>
char src_buf[512];
int main()
{
    long count=0;
    long p=0;
    long l=0;
    char *bgn;
    char *end;
    FILE *fp=NULL;
    fp=fopen("/app/intsedyw/priv/zhanglei/file.txt","r");
    if(fp==NULL)
    {
	printf("open fail\n");
        return 1;
    }
    while(1)
    {
        memset(src_buf,0x00,sizeof(src_buf));
        p=0;
	count=0;
        if(fgets(src_buf,sizeof(src_buf),fp)==NULL)
        {
            if(0==l)
	    {
		printf("文件为空!");
		return 1;
	    }
	    else
	    {
                printf("gets fail\n");
                break;
	    }
        }
	l++;
        printf("src_buf:[%s]\n",src_buf);
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
        printf("p=[%d],src_buf[p]=[%c],count=[%d]\n",p,src_buf[p],count);
    }
    printf("文件行数:[%d]\n",l);
    fclose(fp);
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
