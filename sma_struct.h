
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

#define DATELENGTH 18

typedef struct{
    unsigned int 	key1;
    unsigned int 	key2;
    char		description[40];
    char		units[20];
    float		divisor;
    int  		decimal;
    int			datalength;
    int			recordgap;
    int			persistent;
} ReturnType;

typedef struct {
      time_t date;
      char   inverter[30];
      unsigned long long serial;
      float  accum_value;
      float  current_value;
} ArchDataType;

typedef struct {
      time_t date;
      char   inverter[30];
      unsigned long long serial;
      char   Description[30];
      char   Value[30];
      char   Units[30];
      int    Persistent;
} LiveDataType;

typedef struct{
    char BTAddress[20];         /*--address  	-a 	*/
    int  bt_timeout;		/*--timeout  	-t 	*/
    char Password[20];          /*--password 	-p 	*/
    char Config[80];            /*--config   	-c 	*/
    char File[80];              /*--file     	-f 	*/
    char Xml[80];               /*--xml     	-x 	*/
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
    unsigned int MySUSyID[2];   /*SUSyID  of this app*/
    unsigned int MySerial[4];   /*Serial  of this app*/
    unsigned int MyBTAddress[6];   /*Serial  of this app*/
    unsigned int NetID;         /* Network ID of Inverter*/
    ReturnType *returnkeylist;  /* pointer to return key list */
    unsigned int num_return_keys;   /* number of items in list */
    char datefrom[DATELENGTH];  /* is system using a daterange */
    char dateto[DATELENGTH];     /* is system using a daterange */
} ConfType;

typedef struct{
    unsigned int debug;         /* debug flag */
    unsigned int verbose;       /* verbose flag */
    unsigned int daterange;     /* is system using a daterange */
    unsigned int location;     /* is system using a daterange */
    unsigned int test;     /* is system using a daterange */
    unsigned int mysql;     /* is system using a daterange */
    unsigned int file;     /* is system using a daterange */
    unsigned int post;     /* is system using a daterange */
    unsigned int repost;     /* is system using a daterange */
} FlagType;

typedef struct{
    char Inverter[20]; 		/*--inverter 	-i 	*/
    char Name[20];
    char SerialStr[20];
    unsigned char Address[4];    /*--address  	-a 	*/
    unsigned char SUSyID[2];     /*SUSyID  of the inverter*/
    unsigned char Serial[4];     /*Serial  of this app*/
    unsigned char NetID;         /* Network ID of Inverter*/
} UnitType;

typedef struct{
    unsigned char source[6];		/*Read Source		*/
    unsigned char Destination[6];  	/*Read Destination	*/
    unsigned char Control[2];		/*Control Data		*/
    int		  DataSize;		/*Date Packet Size	*/
    unsigned char Ctrl1[2];		/*Ctrl1			*/
    unsigned char SUSyID;		/*SUSyID		*/
    int		  Data2;		/*Boolean for data2+ record */
    unsigned char SUSSerial[5];		/*SUS Inverter Bus ID	*/
    unsigned int  Status[2];		/*Return Status		*/
    unsigned char data[255];		/*Data to be analysed	*/
} ReadRecordType;

#endif
