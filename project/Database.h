#pragma once
#include <mysql.h>
#include <string>
#include <cstring>
#include <cassert>

#define DATABASE "exp"
#define DATABASE_HOST "localhost"
#define DATABASE_USER "root"
#define DATABASE_PASSWD "xfJ588,."
#define DATABASE_PORT 3306

using namespace std;

class Database
{
	MYSQL mysql;
	void init();
public:
	Database();
	MYSQL* GetMysql();
	string GetSerialNumber(const string& orderPre);
	static MYSQL_BIND* StartStmt(MYSQL* mysql, const char* command, const int EXPECTED_PARAM_COUNT, MYSQL_STMT** out_stmt);
	static void ExecuteStmt(MYSQL_STMT* stmt, MYSQL_BIND* bind, bool autoFree = true);
	static void FreeStmt(MYSQL_STMT* stmt, MYSQL_BIND* bind);

	~Database();
};