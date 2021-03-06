#include <stdio.h>
#include <string.h>
#include <ctype.h>
//清除字符串左边的空格
char *ltrim(char *str)
{
    char *tmp = str;
    while(*tmp)
	{
        if (isspace(*tmp))
            ++tmp;
        else
            break;
	}
    //printf("%d\n",tmp);
    //printf("%d\n",str);
    if(tmp > str)
        strcpy(str,tmp);
    return str;
}

//清除字符串右边的空格
char *rtrim(char *str)
{
    char *tmp = str;
    while(*tmp)
		++tmp;
    --tmp;
    //printf("%d\n",tmp);
    //printf("%d\n",str);
    while(tmp >= str)
	{
        if (isspace(*tmp))
            --tmp;
        else
            break;
	}
    *(tmp+1)='\0';
    return str;
}

//清除字符串两边空格
char *trim(char *str)
{
    ltrim(str);
    rtrim(str);
    return str;
}

/*
int main()
{
    char str1[15]="     asd   ";
    char str2[15]="     asd   ";
    char str3[15]="     asd   ";
    printf("ltrim:[%s]\n",ltrim(str1));
    printf("rtrim:[%s]\n",rtrim(str2));
    printf("trim:[%s]\n",trim(str3));
    return 0;
}
*/
