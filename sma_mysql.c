#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sma_struct.h"
#include <time.h>


MYSQL *conn;
MYSQL_RES *res;
MYSQL_RES *res1;
MYSQL_RES *res2;


void OpenMySqlDatabase(char *, char *, char *, char * );
void CloseMySqlDatabase();
int DoQuery(char *);
int DoQuery1(char *);
int DoQuery2(char *);
int install_mysql_tables( ConfType *, FlagType *, char * );
void update_mysql_tables( ConfType *, FlagType *  );
int check_schema( ConfType *, FlagType *, char * );

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

int install_mysql_tables( ConfType * conf, FlagType * flag, char *SCHEMA )
/*  Do initial mysql table creationsa */
{
    int	        found=0;
    MYSQL_ROW 	row;
    char 	SQLQUERY[1000];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, "mysql");
    //Get Start of day value
    sprintf(SQLQUERY,"SHOW DATABASES" );
    if (flag->debug == 1) printf("%s\n",SQLQUERY);

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
       if (flag->debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);

       sprintf( SQLQUERY,"USE  %s", conf->MySqlDatabase );
       if (flag->debug == 1) printf("%s\n",SQLQUERY);
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

       if (flag->debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);
  
       sprintf( SQLQUERY, "CREATE TABLE `DayData` ( \
           `DateTime` datetime NOT NULL, \
           `Inverter` varchar(30) NOT NULL, \
           `Serial` varchar(40) NOT NULL, \
           `CurrentPower` int(11) DEFAULT NULL, \
           `ETotalToday` DECIMAL(10,3) DEFAULT NULL, \
           `Voltage` DECIMAL(10,3) DEFAULT NULL, \
           `PVOutput` datetime DEFAULT NULL, \
           `CHANGETIME` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP, \
           PRIMARY KEY (`DateTime`,`Inverter`,`Serial`) \
           ) ENGINE=MyISAM" );

       if (flag->debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);

       sprintf( SQLQUERY, "CREATE TABLE `LiveData` ( \
           `id` bigint(20) NOT NULL  AUTO_INCREMENT, \
           `DateTime` datetime NOT NULL, \
           `Inverter` varchar(30) NOT NULL, \
           `Serial` varchar(40) NOT NULL, \
           `Description` varchar(30) NOT NULL, \
           `Value` varchar(30) NOT NULL, \
           `Units` varchar(20) DEFAULT NULL, \
           `CHANGETIME` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' ON UPDATE CURRENT_TIMESTAMP, \
           PRIMARY KEY (`id`), \
           UNIQUE KEY `DateTime`(`DateTime`,`Inverter`,`Serial`,`Description`) \
           ) ENGINE=MyISAM" );
       if (flag->debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);

       sprintf( SQLQUERY, "CREATE TABLE `settings` ( \
           `value` varchar(128) NOT NULL, \
           `data` varchar(500) NOT NULL, \
           PRIMARY KEY (`value`) \
           ) ENGINE=MyISAM" );

       if (flag->debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);
        
       
       sprintf( SQLQUERY, "INSERT INTO `settings` SET `value` = \'schema\', `data` = \'%s\' ", SCHEMA );

       if (flag->debug == 1) printf("%s\n",SQLQUERY);
       DoQuery(SQLQUERY);
    }
    mysql_close(conn);

    return found;
}

void update_mysql_tables( ConfType * conf, FlagType * flag )
/*  Do mysql table schema updates */
{
    int		schema_value=0;
    MYSQL_ROW 	row;
    char 	SQLQUERY[1000];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, "mysql");
    sprintf( SQLQUERY,"USE  %s", conf->MySqlDatabase );
    if (flag->debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    /*Check current schema value*/
    sprintf(SQLQUERY,"SELECT data FROM settings WHERE value=\'schema\' " );
    if (flag->debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    if ((row = mysql_fetch_row(res)))  //if there is a result, update the row
    {
       schema_value=atoi(row[0]);
    }
    mysql_free_result(res);
    if( schema_value == 1 ) { //Upgrade from 1 to 2
        sprintf(SQLQUERY,"ALTER TABLE `DayData` CHANGE `ETotalToday` `ETotalToday` DECIMAL(10,3) NULL DEFAULT NULL" );
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
        sprintf( SQLQUERY, "UPDATE `settings` SET `value` = \'schema\', `data` = 2 " );
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
    }
    /*Check current schema value*/

    sprintf(SQLQUERY,"SELECT data FROM settings WHERE value=\'schema\' " );
    if (flag->debug == 1) printf("%s\n",SQLQUERY);
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
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
        sprintf( SQLQUERY, "UPDATE `settings` SET `value` = \'schema\', `data` = 3 " );
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
    }

    if( schema_value == 3 ) { //Upgrade from 3 to 4
        sprintf(SQLQUERY,"ALTER TABLE `DayData` CHANGE `Inverter` `Inverter` varchar(30) NOT NULL, CHANGE `Serial` `Serial` varchar(40) NOT NULL" );
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
        sprintf(SQLQUERY,"ALTER TABLE `LiveData` CHANGE `Inverter` `Inverter` varchar(30) NOT NULL, CHANGE `Serial` `Serial` varchar(40) NOT NULL, CHANGE `Description` `Description` varchar(30) NOT NULL, CHANGE `Value` `Value` varchar(30), CHANGE `Units` `Units` varchar(20) NULL DEFAULT NULL " );
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
        sprintf( SQLQUERY, "UPDATE `settings` SET `value` = \'schema\', `data` = 4 " );
        if (flag->debug == 1) printf("%s\n",SQLQUERY);
        DoQuery(SQLQUERY);
    }
    
    mysql_close(conn);
}

int check_schema( ConfType * conf, FlagType * flag, char *SCHEMA )
/*  Check if using the correct database schema */
{
    int	        found=0;
    MYSQL_ROW 	row;
    char 	SQLQUERY[200];

    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, conf->MySqlDatabase);
    //Get Start of day value
    sprintf(SQLQUERY,"SELECT data FROM settings WHERE value=\'schema\' " );
    if (flag->debug == 1) printf("%s\n",SQLQUERY);
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


void live_mysql( ConfType conf, FlagType flag, LiveDataType *livedatalist, int livedatalen )
/* Live inverter values mysql update */
{
    struct tm 	*loctime;
    char 	SQLQUERY[2000];
    char	datetime[20];
    int 	day,month,year,hour,minute,second;
    int		live_data=1;
    int		i;
    MYSQL_ROW 	row;
 
    OpenMySqlDatabase( conf.MySqlHost, conf.MySqlUser, conf.MySqlPwd, conf.MySqlDatabase);
    for( i=0; i<livedatalen; i++ ) {
        loctime = localtime(&(livedatalist+i)->date);
        day = loctime->tm_mday;
        month = loctime->tm_mon +1;
        year = loctime->tm_year + 1900;
        hour = loctime->tm_hour;
        minute = loctime->tm_min;
        second = loctime->tm_sec;

        live_data=1;
        if( (livedatalist+i)->Persistent == 1 ) {
            sprintf( SQLQUERY, "SELECT IF (Value = \"%s\",NULL,Value) FROM LiveData where Inverter=\"%s\" and Serial=%llu and Description=\"%s\" ORDER BY DateTime DESC LIMIT 1", (livedatalist+i)->Value, (livedatalist+i)->inverter, (livedatalist+i)->serial, (livedatalist+i)->Description );
            if (flag.debug == 1) printf("%s\n",SQLQUERY);
            if( DoQuery(SQLQUERY) == 0 ) {
                if ((row = mysql_fetch_row(res)))  //if there is a result, update the row
                {
                     if( row[0] == NULL ) {
                         live_data=0;
                     }
                }
                mysql_free_result(res);
            }
        }
        if( live_data==1 ) {
            sprintf(datetime, "%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second );
            sprintf(SQLQUERY,"INSERT INTO LiveData ( DateTime, Inverter, Serial, Description, Value, Units ) VALUES ( \'%s\', \'%s\', %lld, \'%s\', \'%s\', \'%s\'  ) ON DUPLICATE KEY UPDATE DateTime=Datetime, Inverter=VALUES(Inverter), Serial=VALUES(Serial), Description=VALUES(Description), Description=VALUES(Description), Value=VALUES(Value), Units=VALUES(Units)", datetime, (livedatalist+i)->inverter, (livedatalist+i)->serial, (livedatalist+i)->Description, (livedatalist+i)->Value, (livedatalist+i)->Units);
            if (flag.debug == 1) printf("%s\n",SQLQUERY);
            DoQuery(SQLQUERY);
        }
    }
    mysql_close(conn);
}

