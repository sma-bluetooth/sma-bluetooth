#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef H_SMASTRUCT
    #include "sma_struct.h"
#endif


MYSQL *conn;
MYSQL_RES *res;
MYSQL_RES *res1;
MYSQL_RES *res2;


void OpenMySqlDatabase(char *, char *, char *, char * );
void CloseMySqlDatabase();
int DoQuery(char *);
int DoQuery1(char *);
int DoQuery2(char *);
int install_mysql_tables( ConfType *, char *, int );
void update_mysql_tables( ConfType *, int );
int check_schema( ConfType *, char *, int );

void OpenMySqlDatabase (char *server, char *user, char *password, char *database)
{
   
	
	conn = mysql_init(NULL);
   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(0);
   }
}

void CloseMySqlDatabase()
{
   /* Release memory used to store results and close connection */
   mysql_free_result(res);
   mysql_free_result(res1);
   mysql_free_result(res2);
   mysql_close(conn);
}

int DoQuery (char *query){
	/* execute query */
	
	if (mysql_real_query(conn, query, strlen(query))){
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
	res = mysql_store_result(conn);
	return *mysql_error(conn);
}

int DoQuery1 (char query[1000]){
	/* execute query */
	
	if (mysql_real_query(conn, query, strlen(query))){
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
	res1 = mysql_store_result(conn);
	return *mysql_error(conn);
}

int DoQuery2 (char query[1000]){
	/* execute query */
	
	if (mysql_real_query(conn, query, strlen(query))){
		fprintf(stderr, "%s\n", mysql_error(conn));
	}
	res2 = mysql_store_result(conn);
	return *mysql_error(conn);
}

int install_mysql_tables( ConfType * conf, char *SCHEMA, int debug )
/*  Do initial mysql table creationsa */
{
    int	        found=0;
    MYSQL_ROW 	row;
    char 	SQLQUERY[1000];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, "mysql");
    //Get Start of day value
    sprintf(SQLQUERY,"SHOW DATABASES" );
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    while ((row = mysql_fetch_row(res)))  //if there is a result, update the row
    {
       if( strcmp( row[0], conf->MySqlDatabase ) == 0 )
       {
          found=1;
          printf( "Database exists - exiting" );
       }
    }
    if( found == 0 )
    {
       sprintf( SQLQUERY,"CREATE DATABASE IF NOT EXISTS %s", conf->MySqlDatabase );
       if (debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);

       sprintf( SQLQUERY,"USE  %s", conf->MySqlDatabase );
       if (debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);

       sprintf( SQLQUERY,"CREATE TABLE `Almanac` ( `id` bigint(20) NOT NULL \
          AUTO_INCREMENT, \
          `date` date NOT NULL,\
          `sunrise` datetime DEFAULT NULL,\
          `sunset` datetime DEFAULT NULL,\
          `CHANGETIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, \
           PRIMARY KEY (`id`),\
           UNIQUE KEY `date` (`date`)\
           ) ENGINE=MyISAM" );

       if (debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);
  
       sprintf( SQLQUERY, "CREATE TABLE `DayData` ( \
           `DateTime` datetime NOT NULL, \
           `Inverter` varchar(10) NOT NULL, \
           `Serial` varchar(40) NOT NULL, \
           `CurrentPower` int(11) DEFAULT NULL, \
           `ETotalToday` DECIMAL(10,3) DEFAULT NULL, \
           `Voltage` DECIMAL(10,3) DEFAULT NULL, \
           `PVOutput` datetime DEFAULT NULL, \
           `CHANGETIME` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP, \
           PRIMARY KEY (`DateTime`,`Inverter`,`Serial`) \
           ) ENGINE=MyISAM" );

       if (debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);

       sprintf( SQLQUERY, "CREATE TABLE `settings` ( \
           `value` varchar(128) NOT NULL, \
           `data` varchar(500) NOT NULL, \
           PRIMARY KEY (`value`) \
           ) ENGINE=MyISAM" );

       if (debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);
        
       
       sprintf( SQLQUERY, "INSERT INTO `settings` SET `value` = \'schema\', `data` = \'%s\' ", SCHEMA );

       if (debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);
    }
    mysql_close(conn);

    return found;
}

void update_mysql_tables( ConfType * conf, int debug )
/*  Do mysql table schema updates */
{
    int		schema_value=0;
    MYSQL_ROW 	row;
    char 	SQLQUERY[1000];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, "mysql");
    sprintf( SQLQUERY,"USE  %s", conf->MySqlDatabase );
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    /*Check current schema value*/
    sprintf(SQLQUERY,"SELECT data FROM settings WHERE value=\'schema\' " );
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    if ((row = mysql_fetch_row(res)))  //if there is a result, update the row
    {
       schema_value=atoi(row[0]);
    }
    mysql_free_result(res);
    if( schema_value == 1 ) { //Upgrade from 1 to 2
        sprintf(SQLQUERY,"ALTER TABLE `DayData` CHANGE `ETotalToday` `ETotalToday` DECIMAL(10,3) NULL DEFAULT NULL" );
        if (debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
        sprintf( SQLQUERY, "UPDATE `settings` SET `value` = \'schema\', `data` = 2 " );
        if (debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
    }
    /*Check current schema value*/

    sprintf(SQLQUERY,"SELECT data FROM settings WHERE value=\'schema\' " );
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    if ((row = mysql_fetch_row(res)))  //if there is a result, update the row
    {
       schema_value=atoi(row[0]);
    }
    mysql_free_result(res);
    if( schema_value == 2 ) { //Upgrade from 2 to 3
        sprintf(SQLQUERY,"CREATE TABLE `LiveData` ( \
		`id` BIGINT NOT NULL AUTO_INCREMENT , \
           	`DateTime` datetime NOT NULL, \
           	`Inverter` varchar(10) NOT NULL, \
           	`Serial` varchar(40) NOT NULL, \
		`Description` char(20) NOT NULL , \
		`Value` INT NOT NULL , \
		`Units` char(20) NOT NULL , \
           	`CHANGETIME` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP, \
           	UNIQUE KEY (`DateTime`,`Inverter`,`Serial`,`Description`), \
		PRIMARY KEY ( `id` ) \
		) ENGINE = MYISAM" );
        if (debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
        sprintf( SQLQUERY, "UPDATE `settings` SET `value` = \'schema\', `data` = 3 " );
        if (debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
    }
    
    mysql_close(conn);
}

int check_schema( ConfType * conf, char *SCHEMA, int debug )
/*  Check if using the correct database schema */
{
    int	        found=0;
    MYSQL_ROW 	row;
    char 	SQLQUERY[200];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, conf->MySqlDatabase);
    //Get Start of day value
    sprintf(SQLQUERY,"SELECT data FROM settings WHERE value=\'schema\' " );
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    if ((row = mysql_fetch_row(res)))  //if there is a result, update the row
    {
       if( strcmp( row[0], SCHEMA ) == 0 )
          found=1;
    }
    mysql_free_result(res);
    mysql_close(conn);
    if( found != 1 )
    {
       printf( "Please Update database schema use --UPDATE\n" );
    }
    return found;
}


void live_mysql( ConfType * conf, int year, int month, int day, int hour, int minute, int second, char * inverter, long long serial, char * description, float value, char * units, int debug )
/* Live inverter values mysql update */
{
    char 	SQLQUERY[2000];
    char	datetime[20];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, conf->MySqlDatabase);
    sprintf(datetime, "%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second );
    sprintf(SQLQUERY,"INSERT INTO LiveData ( DateTime, Inverter, Serial, Description, Value, Units ) VALUES ( \'%s\', \'%s\', %lld, \'%s\', %.0f, \'%s\'  ) ON DUPLICATE KEY UPDATE DateTime=Datetime, Inverter=VALUES(Inverter), Serial=VALUES(Serial), Description=VALUES(Description), Description=VALUES(Description), Value=VALUES(Value), Units=VALUES(Units)", datetime, inverter, serial, description, value, units);
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    mysql_close(conn);
}

