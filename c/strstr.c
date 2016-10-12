#include <stdio.h>
#include <string.h>
int main()
{
    char a[50] = ";abc;234;hu7;xx;";
    char *b;
    char c[50];
    if ( strstr(a,";hu3;") != NULL )
	printf("ok!\n");
    else 
	printf("no!\n");
    b = strstr(a,";hu7;");
    printf("b = [%s]\n",b);
    strcpy(c,"");
    printf("c1 = [%s]\n",c);
    if(strcmp("123",c)!=0)
	printf("c2 = [%s]\n",c);
    return 0;
}
