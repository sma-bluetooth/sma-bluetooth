#include <mysql/mysql.h>

extern MYSQL *conn;
extern MYSQL_RES *res;
extern MYSQL_RES *res1;
extern MYSQL_RES *res2;

extern void OpenMySqlDatabase(char *, char *, char *, char * );
extern void CloseMySqlDatabase();
extern int DoQuery(char *);
extern int DoQuery1(char *);
extern int DoQuery2(char *);
extern int install_mysql_tables( ConfType *, char *, int );
extern void update_mysql_tables( ConfType *, int );
extern int check_schema( ConfType *, char *, int );
