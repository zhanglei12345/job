#include <stdio.h>
#include <string.h>
char src_buf[512];
long p;
int getfl(char *tmp_str)
{
    char *bgn=(char*)&src_buf[p];
    char *end=strstr((char*)&src_buf[p],"|-|");
    //printf("p:%d\n",p);
    if(end != NULL)
    {
        char *prt=strstr(bgn,"|+|");
        if( prt != NULL)
        {
            if(prt != bgn)
                strncpy(tmp_str,bgn,prt-bgn);
            p+=prt-bgn+3;
        }
        else
        {
            if(bgn != end)
                strncpy(tmp_str,bgn,end-bgn);
            p+=end-bgn+3;
         }         
    }
    else//拼接|-|后面的数据,文件数据以|-|结束
    {
         strcpy(tmp_str,bgn);
         p+=strlen(bgn);
    }
    //printf("str:%s\n",tmp_str);
    return 0;
}
