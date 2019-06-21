#pragma once
#include "Database.h"

#define STAFF_TABLE_NAME "staff"

class Staff
{
	static const char* const createString;

	static const char* const insertStmtString;
	static const int INSERT_PARAM_COUNT;

	static const char* const updateStmtString;
	static const int UPDATE_PARAM_COUNT;

	static const char* const deleteStmtString;
	static const int DELETE_PARAM_COUNT;

	static const char* const showStmtString;

	MYSQL* mysql_ptr;

public:
	Staff(Database& database);
	void Update(const string& sid, const string& sex, int age, const string& name, const string& phoneNumber, const string& password, int isManager = 0);
	void Insert(const string& sid, const string& sex, int age, const string& name, const string& phoneNumber, const string& password, int isManager = 0);
	void Delete(const string& id);
	bool identify(const string& user, const string& password, int isManager = 0);
	void showStaff();
};
