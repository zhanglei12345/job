#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

int test_table_drop();
int test_table_create();
int test_table_insert();
int test_table_update();
int test_table_select_use();
int test_table_select_store();
int test_table_delete();

MYSQL mysql;

int main(void) 
{
    char *temp=NULL;
    char host[30];
    char user[20];
    char passwd[20];
    char db[20];
    char cport[10];
    unsigned int iport = 0;
    int ret = 0;


    /*获取环境变量*/
    memset(host,0x00,sizeof(host));
    if(!(temp = getenv("DBHOST")))
    {
        printf("not set host\n");
        exit(1);
    }
    memcpy(host,temp,strlen(temp));

    memset(user,0x00,sizeof(user));
    if(!(temp = getenv("DBUSER")))
    {
        printf("not set user\n");
        exit(1);
    }
    memcpy(user,temp,strlen(temp));

    memset(passwd,0x00,sizeof(passwd));
    if(!(temp = getenv("DBPASSWD")))
    {
        printf("not set passwd\n");
        exit(1);
    }
    memcpy(passwd,temp,strlen(temp));

    memset(db,0x00,sizeof(db));
    if(!(temp = getenv("DBBASE")))
    {
        printf("not set db\n");
        exit(1);
    }
    memcpy(db,temp,strlen(temp));

    memset(cport,0x00,sizeof(cport));
    if(!(temp = getenv("DBPORT")))
    {
        printf("not set cport\n");
        exit(1);
    }
    memcpy(cport,temp,strlen(temp));
    iport = atoi(cport);

    /*Initialize the MySQL C API library*/
    if (mysql_library_init(0, NULL, NULL)) {
        printf("could not initialize MySQL client library\n");
        exit(1);
    }

    /*Gets or initializes a MYSQL structure*/
    mysql_init(&mysql);

    /*Sets connect options for mysql_real_connect()*/
    /*mysql 默认自动提交，此处设置为不自动提交*/
    mysql_options(&mysql,MYSQL_INIT_COMMAND,"SET autocommit=0");

    /*Connects to a MySQL server*/
    if (!mysql_real_connect(&mysql,host,user,passwd,db,iport,NULL,0))
    {
        printf("Failed to connect to database: Error: %s\n", mysql_error(&mysql));
        exit(1);
    }

    printf("Succeed to connect to database\n");

    /*drop table*/
    ret = test_table_drop();
    if (ret) {
        printf("test_table_drop err,ret = %d\n",ret);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    /*create table*/
    ret = test_table_create();
    if (ret) {
        printf("test_table_create err,ret = %d\n",ret);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    /*insert*/
    ret = test_table_insert();
    if (ret) {
        printf("test_table_insert err,ret = %d\n",ret);
        mysql_rollback(&mysql);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    /*update*/
    ret = test_table_update();
    if (ret) {
        printf("test_table_update err,ret = %d\n",ret);
        mysql_rollback(&mysql);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    /*select use*/
    ret = test_table_select_use();
    if (ret) {
        printf("test_table_select_use err,ret = %d\n",ret);
        mysql_rollback(&mysql);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    /*select store*/
    ret = test_table_select_store();
    if (ret) {
        printf("test_table_select_store err,ret = %d\n",ret);
        mysql_rollback(&mysql);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    /*delete*/
    ret = test_table_delete();
    if (ret) {
        printf("test_table_delete err,ret = %d\n",ret);
        mysql_rollback(&mysql);
        mysql_close(&mysql);
        mysql_library_end();
        exit(1);
    }

    mysql_commit(&mysql);
    mysql_close(&mysql);
    mysql_library_end();

    return 0;
}

int test_table_drop()
{
    char sql_str[100];

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"DROP TABLE `t_bse_user`");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        /*表不存在*/
        if(mysql_errno(&mysql) == 1051)
        {
            printf("drop table 1051\n\n");
            return 0;
        }else{
            return mysql_errno(&mysql);
        }
    }

    printf("drop table success\n\n");

    return 0;
}

int test_table_create()
{
    char sql_str[500];

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"CREATE TABLE `t_bse_user` ( \
                        `user_id` int(11) NOT NULL, \
                        `user_title` varchar(100) NOT NULL, \
                        `user_author` varchar(40) NOT NULL, \
                        `add_date` date DEFAULT NULL, \
                        PRIMARY KEY (`user_id`) \
                        ) ENGINE=InnoDB DEFAULT CHARSET=utf8");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        return mysql_errno(&mysql);
    }

    printf("create table success\n\n");

    return 0;
}

int test_table_insert()
{
    char sql_str[500];

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"INSERT INTO `t_bse_user` (`user_id`, `user_title`, `user_author`, `add_date`) \
                     VALUES \
                           (1, '生活', 'user1', NOW()), \
                           (2, '工作', 'user2', NOW())");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        return mysql_errno(&mysql);
    }

    printf("insert [%ld] data success\n\n",(long)mysql_affected_rows(&mysql));

    return 0;
}

int test_table_update()
{
    char sql_str[500];

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"update `t_bse_user` set `user_title` = '生活，慢一点' where `user_id` = 1");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        return mysql_errno(&mysql);
    }

    if(mysql_affected_rows(&mysql) == 0)
    {
        return 1403;
    }

    printf("update [%ld] data success\n\n",(long)mysql_affected_rows(&mysql));

    return 0;
}

int test_table_delete()
{
    char sql_str[500];

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"delete from `t_bse_user` where `user_id` = 2");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        return mysql_errno(&mysql);
    }

    if(mysql_affected_rows(&mysql) == 0)
    {
        return 1403;
    }

    printf("delete [%ld] data success\n\n",(long)mysql_affected_rows(&mysql));

    return 0;
}

int test_table_select_use()
{
    char sql_str[500];
    MYSQL_RES *result;
    MYSQL_ROW row;
    unsigned int num_fields;
    unsigned long *lengths;
    unsigned int i;

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"select `user_id`, `user_title`, `user_author` from `t_bse_user`");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        return mysql_errno(&mysql);
    }

    if(!(result = mysql_use_result(&mysql)))
    {
        printf("mysql_use_result() err:[%s], errno=[%u]",mysql_error(&mysql),mysql_errno(&mysql));
        return mysql_errno(&mysql);
    }

    num_fields = mysql_num_fields(result);

    while((row = mysql_fetch_row(result)))
    {
        lengths = mysql_fetch_lengths(result);

        for(i = 0; i < num_fields; i++)
        {
            printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
        }

        printf("\n");

    }

    printf("select [%llu] data success\n\n",mysql_num_rows(result));

    mysql_free_result(result);

    return 0;
}

int test_table_select_store()
{
    char sql_str[500];
    MYSQL_RES *result;
    MYSQL_ROW row;
    unsigned int num_fields;
    unsigned long *lengths;
    unsigned int i;

    memset(sql_str,0x00,sizeof(sql_str));

    sprintf(sql_str,"select `user_id`, `user_title`, `user_author` from `t_bse_user`");

    printf("sql_str = [%s]\n",sql_str);

    if(mysql_query(&mysql,sql_str))
    {
        return mysql_errno(&mysql);
    }

    if(!(result = mysql_store_result(&mysql)))
    {
        printf("mysql_store_result() err:[%s], errno=[%u]",mysql_error(&mysql),mysql_errno(&mysql));
        return mysql_errno(&mysql);
    }

    num_fields = mysql_num_fields(result);

    /*只获取一条记录*/
    row = mysql_fetch_row(result);
    if(!row)
    {
        mysql_free_result(result);
        return mysql_errno(&mysql);
    }
    lengths = mysql_fetch_lengths(result);

    for(i = 0; i < num_fields; i++)
    {
        printf("[%.*s] ", (int) lengths[i], row[i] ? row[i] : "NULL");
    }

    printf("\n");

    printf("select [%llu] store data success\n\n",mysql_num_rows(result));

    mysql_free_result(result);

    return 0;
}

/*编译
gcc dbconnect.c -I/usr/local/include/mysql -lmysqlclient
*/