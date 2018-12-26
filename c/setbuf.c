#include <stdio.h>

char outbuf[BUFSIZ];//必须是BUFSIZ，否则存在缓冲溢出可能

int main(void)
{
	/* 将outbuf与stdout输出流相连接 */
	setbuf(stdout,outbuf);
	//setbuf(stdout,NULL);
	/* 向stdout中放入一些字符串 */
	puts("This is a test of buffered output.");//体现stdout的行缓冲性质
	fprintf(stdout,"hello world.\n");
	//sprintf(outbuf,"test!\n");
	/*刷新流*/
	fflush(stdout);
	/* 以下是outbuf中的内容 */
	puts(outbuf);
	return 0;
}

/**
 * void setbuf(FILE *steam, char *buf);
 * setbuf函数具有打开和关闭缓冲机制。
 * 将buf参数设置为NULL 时,关闭缓冲。
 *
 * int fflush( FILE *stream );
 * 对于输出流，将输出缓冲区的数据写入到输出设备
**/
