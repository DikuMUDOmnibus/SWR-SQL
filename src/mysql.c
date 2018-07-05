#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <mysql/mysql.h>
#include "mud.h"

/* you can look up on how to fully install MySQL support on mysql.com
   you'll need to add some linkage files to your makefile to get this to work
*/

#define DB_HOST "localhost" //If it's not then put in the host address
#define DB_USER "username" //The username of your database account..NOT SHELL
#define DB_PASS "password" //the password to said account
#define DB_DB   "swrsql" //The name of the database within the DBMS
#define DB_PORT        3306      //The port on which the db accepts connections
#define DB_SOCKET      0      //Shouldnt have to mess with
#define DB_CLIENT_FLAG 0      //once again...dont mess with it

/* DB_CONNECT_TIME_LIMIT is the amount of time in minutes 
   you want the connection to the DB to stay active
*/
#define DB_CONNECT_TIME_LIMIT 5

/* use whatever log code you want here */ 
#define SYS_LOG(x)  log_string(x)

/* these SHOULD be able to local */
/* i would add the following to your global header if your lazy
   or where applicable if your not. query_db should go in main as it
   will be used where ever you decided to load from the DB.
*/

/* opens a connection to the database.  This does not need to be
   called on system startup as the query function will check this
   for you.
*/
void connect_db()
{
    char output[MAX_STRING_LENGTH];
	if ((db = mysql_init(NULL)) == NULL)
	{
		SYS_LOG("connect_db: error on initialize");
		return;
	}
	if (!mysql_real_connect(db, DB_HOST, DB_USER, DB_PASS,
		DB_DB, DB_PORT, DB_SOCKET, DB_CLIENT_FLAG))
	{
		/*I have a seperate loging system for boot up
                  remove or change as needed
                */
		log_string("connect_db: error on connect");
		sprintf(output, "Error: %s", mysql_error(db));
		log_string(output);
 		return;
	}
        db_connect = TRUE;
        db_last_action = 0;
        	log_string("mysql connected");

	return;
}

/* closes the database connection */
void disconnect_db()
{
	mysql_close(db);
	db_connect = FALSE;
	db = NULL;
	log_string("mysql disconnected");
	return;
}

/* Function that physically send the query to the database and
   returns the results.  query parameter should look like any other
   sql query. ie SELECT name FROM t_player_name WHERE player_id > 0
   NOTE: you will still have to check to see if valid results are returned
   and then parse results by row. You can also use this for all sql statements
   not just selects.
*
MYSQL_RES *query_db(char *query)
{
        va_list         argp;
        int             i = 0;
        double          j = 0;
        char            *s = 0, *out = 0, *p = 0;
        char            safe [MAX_STRING_LENGTH];
        char            query [MAX_STRING_LENGTH];
                                
        *query = '\0';
        *safe = '\0';

        va_start (argp, fmt);
                        
        for ( p = fmt, out = query; *p != '\0'; p++ ) {
                if ( *p != '%' ) {
			*out++ = *p;
                        continue;
                }
        
                switch ( *++p ) {
                        case 'c':
                                i = va_arg (argp, int);
				out += sprintf (out, "%c", i);
                                break;
                        case 's':
                                s = va_arg (argp, char *);
				if ( !s ) {
					out += sprintf (out, " ");
					break;
				}
                                mysql_real_escape_string (db, safe, s, strlen(s));
                                out += sprintf (out, "%s", safe);
                                *safe = '\0';
                                break;
                        case 'd':
                                i = va_arg (argp, int);
                                out += sprintf (out, "%d", i);
                                break;
                        case 'f':
                                j = va_arg (argp, double);
                                out += sprintf (out, "%f", j);
                                break;
                        case '%':
                                out += sprintf (out, "%%");
                                break;
                }
        }
                 
	*out = '\0';

        va_end (argp);

        return mysql_store_result(mysql_real_query (db, query, strlen(query)));
}*/

MYSQL_RES *query_db(char *query)
{
	char buf[MAX_INPUT_LENGTH];
	buf[0] = '\0';

	if( !db_connect )
		connect_db();
	if( db == NULL )
	{
		SYS_LOG("NULL db in query_db.");
		return NULL;
	}
	if( query == NULL || query[0] == '\0')
	{
		SYS_LOG("Null query in query_db()");
		return NULL;
	}
 	if (mysql_real_query(db, query, strlen(query)))
	{
		SYS_LOG("query_db(): error on query");
		SYS_LOG("-----------Error in MySQL------------");
		SYS_LOG(query);
		SYS_LOG( mysql_error(db));
		return NULL;
	}
 	return mysql_store_result(db);
}
/*
 The call to this function should be made somewhere in your
   update calls..which every area you like that is called approx
   once per minute.  This kills the database connection if it has
   been idle for set period of time. This doesnt not have to be added
   but your sys admin might appreciate it.
*/
void check_db_disconnect()
{
	/* first check if we're even connected */
	if( !db_connect)
		return;
	/* No we see if we have been connected for too long,
	   if we have, lets disconnect and save resources
        */
	if( db_last_action >= DB_CONNECT_TIME_LIMIT)
	{
		disconnect_db();
		return;
	}
	/* at this point we must still be connected, so lets increment our time */
	db_last_action++;
	return;
}

/* function to make strings all nice and friendly with proper
   escape sequences and such.  Memory allocation should not be
   meddled with unless you know what you are doing.  Currently it assumes
   worst case..an escape for ever character plus the null terminator.
   I know this looks wierd..but it works. Use this when you are putting
   data into the database.
*/
char *sql_escape_string( char *source) 
{
	char *buf = '\0';
	char *ret = '\0';
	int len = 0;

	len = strlen( source );
	buf = malloc( (2 *len) + 1);
	mysql_real_escape_string( db, buf, source, len);
	ret = str_dup( buf );
	free( buf );
	return ret;
}


/* use this to see if any data was returned */
bool sql_data_returned( MYSQL_RES *result ) 
{
	if( mysql_num_rows( result ) )
		return TRUE;
	else
		return FALSE;
}

/* NOTE, these macros have not been tested, i simply added them in
   in the hope that they should work and allow you to understand how
   to use the results of a query
*/
/* start the loop */
#define PARSE_ROW( row, res) while( (row = mysql_fetch_row(res)) ) {
/* for getting string data back */
#define GET_ROW_S_DATA( item, field, row, num) (item)->field = str_dup( row[num] )
/* for getting numerical data back */
#define GET_ROW_N_DATA( item, field, row, num) (item)->field = atoi( row[num] )

/******************* EXAMPLE ON HOW TO USE *******************************
 MYSQL_ROW row;
 MYSQL_RES *result = NULL;
 SOME_DATA *thing = NULL;
 if( (result = query_db( some query text )) == FALSE ) {
   send some message some where
   exit out..or do whatever you want to do
 }
 -----with macros...assuming they work..you should be able to figure out what
 is going on from the macro definition
 PARSE_ROW( row, result)
    create memory for new item if needed...link to lists if needed
    GET_ROW_S_DATA( thing, name, row, 0);
    GET_ROW_N_DATA( thing, id_num, row, 1);
 }
 **********NOTE************
 the numbers for the row are dependant on how you have the columns
 set up in your database, if id_num came before name for example then switch the
 numbers.  Personally i would make a seperate header file to contain preprocessor
 defines for all the database row numbers...looking them up constantly is a pain
 in the ass
***************************************************************************/
