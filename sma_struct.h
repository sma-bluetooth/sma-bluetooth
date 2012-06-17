
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
#ifndef H_SMASTRUCT
   #define H_SMASTRUCT
#endif

typedef struct{
    char Inverter[20]; 		/*--inverter 	-i 	*/
    char Serial[20]; 		/*derived               */
    char BTAddress[20];         /*--address  	-a 	*/
    int  bt_timeout;		/*--timeout  	-t 	*/
    char Password[20];          /*--password 	-p 	*/
    char Config[80];            /*--config   	-c 	*/
    char File[80];              /*--file     	-f 	*/
    float latitude_f;           /*--latitude  	-la 	*/
    float longitude_f;          /*--longitude 	-lo 	*/
    char MySqlHost[40];         /*--mysqlhost   -h 	*/
    char MySqlDatabase[20];     /*--mysqldb     -d 	*/
    char MySqlUser[80];         /*--mysqluser   -user 	*/
    char MySqlPwd[80];          /*--mysqlpwd    -pwd 	*/
    char PVOutputURL[80];       /*--pvouturl    -url 	*/
    char PVOutputKey[80];       /*--pvoutkey    -key 	*/
    char PVOutputSid[20];       /*--pvoutsid    -sid 	*/
    char Setting[80];           /*inverter model data*/
    unsigned char InverterCode[4]; /*Unknown code inverter specific*/
    unsigned int ArchiveCode;    /* Code for archive data */
} ConfType;
