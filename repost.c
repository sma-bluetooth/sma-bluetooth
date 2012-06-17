/* tool to read power production data for SMA solar power convertors 
   Copyright Wim Hofman 2010 
   Copyright Stephen Collier 2010,2011 

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* repost is a utility to check pvoutput data and repost any differences */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/types.h>
#include <curl/curl.h>
#include "sma_struct.h"
#include "sma_mysql.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    size_t written;

    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int sma_repost( ConfType * conf, int debug, int verbose )
{
    FILE* fp;
    CURL *curl;
    CURLcode result;
    char buf[1024], buf1[400];
    char 	SQLQUERY[1000];
    char compurl[400];
    int	 ret, update_data;
    MYSQL_ROW row;

    float dtotal, starttotal;
    float power;
    
    /* Connect to database */
    OpenMySqlDatabase( conf->MySqlHost, conf->MySqlUser, conf->MySqlPwd, conf->MySqlDatabase );
    //Get Start of day value
    starttotal = 0;
    printf("SELECT DATE_FORMAT( dt1.DateTime, \"%%Y%%m%%d\" ), round((dt1.ETotalToday*1000-dt2.ETotalToday*1000),0) FROM DayData as dt1 join DayData as dt2 on dt2.DateTime = DATE_SUB( dt1.DateTime, interval 1 day ) WHERE dt1.DateTime LIKE \"%%-%%-%% 23:55:00\" ' ORDER BY dt1.DateTime DESC" );
    sprintf(SQLQUERY,"SELECT DATE_FORMAT( dt1.DateTime, \"%%Y%%m%%d\" ), round((dt1.ETotalToday*1000-dt2.ETotalToday*1000),0) FROM DayData as dt1 join DayData as dt2 on dt2.DateTime = DATE_SUB( dt1.DateTime, interval 1 day ) WHERE dt1.DateTime LIKE \"%%-%%-%% 23:55:00\" ORDER BY dt1.DateTime DESC" );
    if (debug == 1) printf("%s\n",SQLQUERY);
    DoQuery(SQLQUERY);
    while(( row = mysql_fetch_row(res) ))  //if there is a result, update the row
    {
        startforwait:
        fp=fopen( "/tmp/curl_output", "w+" );
        update_data = 0;
        dtotal = atof(row[1]);
        sleep(2);  //pvoutput limits 1 second output
	ret=sprintf(compurl,"http://pvoutput.org/service/r1/getstatistic.jsp?df=%s&dt=%s&key=%s&sid=%s",row[0],row[0],conf->PVOutputKey,conf->PVOutputSid);
        curl = curl_easy_init();
        if (curl){
	     curl_easy_setopt(curl, CURLOPT_URL, compurl);
	     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	     curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	     //curl_easy_setopt(curl, CURLOPT_FAILONERROR, compurl);
	     result = curl_easy_perform(curl);
             if (debug == 1) printf("result = %d\n",result);
             rewind( fp );
             fgets( buf, sizeof( buf ), fp );
             result = sscanf( buf, "Bad request %s has no outputs between the requested period", buf1 );
             printf( "return=%d buf1=%s\n", result, buf1 );
             if( result > 0 )
             {
                 update_data=1;
                 printf( "test\n" );
             }
             else
             {
                 printf( "buf=%s here 1.\n", buf );
                 result = sscanf( buf, "Forbidden 403: Exceeded 60 requests %s", buf1 );
		 if( result > 0 ) {
		    printf( "Too Many requests in 1hr sleeping for 1hr\n");
                    fclose(fp);
                    sleep(3600);
                    goto startforwait;
                 }
                    
                 printf( "return=%d buf1=%s\n", result, buf1 );
                 if( sscanf( buf, "%f,%s", &power, buf1 ) > 0 ) {
                    printf( "Power %f\n", power );
                    if( power != dtotal )
                    {
                       printf( "Power %f Produced=%f\n", power, dtotal );
                       update_data=1;
                    }
                 }
             }
	     curl_easy_cleanup(curl);
             if( update_data == 1 ) {
                 curl = curl_easy_init();
                 if (curl){
	            ret=sprintf(compurl,"http://pvoutput.org/service/r2/addoutput.jsp?d=%s&g=%f&key=%s&sid=%s",row[0],dtotal,conf->PVOutputKey,conf->PVOutputSid);
                    if (debug == 1) printf("url = %s\n",compurl);
		    curl_easy_setopt(curl, CURLOPT_URL, compurl);
		    curl_easy_setopt(curl, CURLOPT_FAILONERROR, compurl);
		    result = curl_easy_perform(curl);
                    sleep(1);
	            if (debug == 1) printf("result = %d\n",result);
		    curl_easy_cleanup(curl);
                    if( result==0 ) 
                    {
                        sprintf(SQLQUERY,"UPDATE DayData set PVOutput=NOW() WHERE DateTime=\"%s235500\"  ", row[0] );
                        if (debug == 1) printf("%s\n",SQLQUERY);
                        //DoQuery(SQLQUERY);
                    }
                    else
                        break;
                 }
             }
        }
        fclose(fp);
    }
    mysql_close(conn);
}
