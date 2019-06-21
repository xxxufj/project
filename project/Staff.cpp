#include "Staff.h"
#include <iostream>

using namespace std;

const char* const Staff::createString = "CREATE TABLE `" DATABASE "`.`" STAFF_TABLE_NAME "`("
"sid CHAR(8) NOT NULL,"
"sex VARCHAR(6) DEFAULT 'male',"
"age INT,"
"name VARCHAR(20) NULL,"
"phoneNumber CHAR(11),"
"password VARCHAR(20),"
"isManager INT DEFAULT 0,"
"PRIMARY KEY(sid)"
");";

const char* const Staff::insertStmtString = "INSERT INTO `" DATABASE "`.`" STAFF_TABLE_NAME "` ("
"`sid`, `sex`, `age`, `name`, `phoneNumber`, `password`, `isManager`) VALUES (?, ?, ?, ?, ?, ?, ?)";

const int Staff::INSERT_PARAM_COUNT = 7;

const char* const Staff::updateStmtString = "UPDATE `" DATABASE "`.`" STAFF_TABLE_NAME "` SET"
"`sid` = ?, `sex` = ?, `age` = ?, `name` = ?, `phoneNumber` = ? , `isManager` = ? WHERE ("
"`sid` = ?)";

const int Staff::UPDATE_PARAM_COUNT = 7;

const char* const Staff::deleteStmtString = "DELETE FROM `" DATABASE "`.`" STAFF_TABLE_NAME "` WHERE (`sid` = ?)";

const int Staff::DELETE_PARAM_COUNT = 1;

const char* const Staff::showStmtString = "SELECT* FROM `" DATABASE "`.`" STAFF_TABLE_NAME "`";

void Staff::Update(const string& sid, const string& sex, int age, const string& name, const string& phoneNumber, const string& password, int isManager)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, updateStmtString, UPDATE_PARAM_COUNT, &stmt);

	bind[6].buffer = (void*)sid.c_str();
	bind[6].buffer_type = MYSQL_TYPE_STRING;
	bind[6].buffer_length = sid.length();

	bind[0].buffer = (void*)sex.c_str();
	bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[0].buffer_length = sex.length();

	bind[1].buffer = (void*)& age;
	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer_length = sizeof(age);

	bind[2].buffer = (void*)name.c_str();
	bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[2].buffer_length = name.length();

	bind[3].buffer = (void*)phoneNumber.c_str();
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer_length = phoneNumber.length();

	bind[4].buffer = (void*)password.c_str();
	bind[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[4].buffer_length = password.length();

	bind[5].buffer = (void*)&isManager;
	bind[5].buffer_type = MYSQL_TYPE_LONG;
	bind[5].buffer_length = sizeof(isManager);

	Database::ExecuteStmt(stmt, bind);
}

void Staff::Insert(const string& sid, const string& sex, int age, const string& name, const string& phoneNumber, const string& password, int isManager)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertStmtString, INSERT_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)sid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = sid.length();

	bind[1].buffer = (void*)sex.c_str();
	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[1].buffer_length = sex.length();

	bind[2].buffer = (void*)& age;
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer_length = sizeof(age);

	bind[3].buffer = (void*)name.c_str();
	bind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[3].buffer_length = name.length();

	bind[4].buffer = (void*)phoneNumber.c_str();
	bind[4].buffer_type = MYSQL_TYPE_STRING;
	bind[4].buffer_length = phoneNumber.length();

	bind[5].buffer = (void*)password.c_str();
	bind[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[5].buffer_length = password.length();

	bind[6].buffer = (void*)&isManager;
	bind[6].buffer_type = MYSQL_TYPE_LONG;
	bind[6].buffer_length = sizeof(isManager);

	Database::ExecuteStmt(stmt, bind);
}

Staff::Staff(Database& database) : mysql_ptr(database.GetMysql())
{
	if (mysql_list_tables(mysql_ptr, STAFF_TABLE_NAME)->row_count == 0)
	{
		mysql_query(mysql_ptr, createString);
	}
}

void Staff::Delete(const string& sid)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteStmtString, DELETE_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)sid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = sid.length();

	Database::ExecuteStmt(stmt, bind);
}

void Staff::showStaff() {
	mysql_query(mysql_ptr, showStmtString);
}

bool Staff::identify(const string& user, const string& password, int isManager) {
	static const char* const stmtString = "SELECT * FROM " DATABASE "." STAFF_TABLE_NAME " WHERE (sid = ?) AND (password = ?) AND (isManager = ?)";
	int PARAM_COUNT = 3;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)user.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = user.length();

	bind[1].buffer = (void*)password.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = password.length();

	bind[2].buffer = (void*)&isManager;
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer_length = sizeof(isManager);

	Database::ExecuteStmt(stmt, bind, false);

	int ret = mysql_stmt_num_rows(stmt);

	Database::FreeStmt(stmt, bind);
	return ret != 0 && ret != -1;
}