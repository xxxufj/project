#include "Database.h"
#include <iostream>

using namespace std;

void Database::init()
{
	//mysql_query(&mysql, "SET NAMES GB2312");
	mysql_query(&mysql, "SET NAMES GBK");

	const char* CreateSerialTableStmt = "CREATE TABLE IF NOT EXISTS " DATABASE ".orderSerial("
		"orderType CHAR(10) NOT NULL,"
		"nextSerial INT DEFAULT 0,"
		"PRIMARY KEY(orderType));";

	const char* CreateResetEvent = "CREATE EVENT IF NOT EXISTS resetSerial\n\
		on schedule every 1 day starts '2019-06-18 08:00:00'\n\
		do UPDATE orderSerial SET nextSerial = 0;";

	const char* CreateProcedure = "CREATE PROCEDURE `generate`(in orderPre char(10))"
		"BEGIN "
		"DECLARE currentDate VARCHAR(15);"
		"DECLARE newNo INT DEFAULT 0;"
		"DECLARE newOrderNo varCHAR(25);"
		"SELECT DATE_FORMAT(NOW(), '%Y%m%d%H%i') INTO currentDate;"
		"SELECT nextSerial INTO newNo FROM orderSerial  WHERE orderType = orderPre;"
		"UPDATE orderSerial SET nextSerial = nextSerial + 1 WHERE orderType = orderPre;"
		"SELECT "
		"	CONCAT(orderPre, currentDate, LPAD((newNo), 5, '0')) INTO newOrderNo;"
		"SELECT "
		"	newOrderNo;"
		"END ";


	mysql_query(&mysql, CreateSerialTableStmt);
	mysql_query(&mysql, "INSERT INTO " DATABASE ".orderSerial(orderType) VALUES(\"import\")");
	mysql_query(&mysql, "INSERT INTO " DATABASE ".orderSerial(orderType) VALUES(\"sale\")");
	mysql_query(&mysql, "INSERT INTO " DATABASE ".orderSerial(orderType) VALUES(\"return\")");
	
	mysql_query(&mysql, "DROP EVENT resetSerial");
	mysql_query(&mysql, "set GLOBAL event_scheduler = ON");
	mysql_query(&mysql, CreateResetEvent);
	mysql_query(&mysql, "ALTER EVENT resetSerial ENABLE");

	mysql_query(&mysql, "DROP PROCEDURE generate");
	mysql_query(&mysql, CreateProcedure);
}

Database::Database()
{
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, DATABASE_HOST, DATABASE_USER, DATABASE_PASSWD, DATABASE, DATABASE_PORT, nullptr, 0)) {
		cout << "connect failed" << endl;
	}
	init();
}

MYSQL* Database::GetMysql()
{
	return &mysql;
}

string Database::GetSerialNumber(const string& orderPre)
{
	MYSQL_RES* res;
	string queryStr = "call `generate`('" + orderPre + "');";
	mysql_query(&mysql, queryStr.c_str());
	res = mysql_store_result(&mysql);
	MYSQL_ROW row = mysql_fetch_row(res);
	string serial(row[0]);
	mysql_free_result(res);

	while (mysql_next_result(&mysql) == 0) {
		if (mysql_field_count(&mysql) > 0)
		{
			res = mysql_store_result(&mysql);
			mysql_free_result(res);
		}
	}
	return serial;
}

MYSQL_BIND* Database::StartStmt(MYSQL* mysql, const char* command, const int EXPECTED_PARAM_COUNT, MYSQL_STMT** out_stmt)
{
	MYSQL_STMT* stmt = mysql_stmt_init(mysql);
	mysql_stmt_prepare(stmt, command, strlen(command));
	unsigned long param_count = mysql_stmt_param_count(stmt);
	assert(param_count == EXPECTED_PARAM_COUNT);
	MYSQL_BIND* bind = new MYSQL_BIND[param_count]();
	memset(bind, 0, sizeof(MYSQL_BIND) * param_count);
	*out_stmt = stmt;
	return bind;
}

void Database::ExecuteStmt(MYSQL_STMT* stmt, MYSQL_BIND* bind, bool autoFree)
{
	mysql_stmt_bind_param(stmt, bind);
	mysql_stmt_execute(stmt);
	cout << mysql_stmt_error(stmt);

	if (autoFree)
	{
		mysql_stmt_close(stmt);
		delete[] bind;
	}
	else
	{
		mysql_stmt_store_result(stmt);
	}
}

void Database::FreeStmt(MYSQL_STMT* stmt, MYSQL_BIND* bind)
{
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);
	delete[] bind;
}

Database::~Database()
{
	mysql_close(&mysql);
}
