char updatequery[MAX_STRING_LENGTH*2];
sprintf(updatequery, "UPDATE table SET blah='%s'", whatever);
query_db(updatequery);

char insertquery[MAX_STRING_LENGTH*2];
sprintf(insertquery, "INSERT INTO table SET blah='%s'", whatever);
query_db(insertquery);

char deletequery[MAX_STRING_LENGTH*2];
sprintf(deletequery, "DELETE FROM table where blah='%s'", whatever);
query_db(deletequery);

char selectquery[MAX_STRING_LENGTH*2];
MYSQL_RES   *qresult;
MYSQL_ROW table;
sprintf(selectquery, "SELECT * FROM table where blah='%s'", whatever);
qresult = query_db(selectquery);
table = mysql_fetch_row(qresult);

if (!sql_data_returned(qresult))
while( (table = mysql_fetch_row(qresult)) )