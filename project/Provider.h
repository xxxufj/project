#pragma once
#include "Database.h"

#define PROVIDER_TABLE_NAME "provider"

class Provider
{
	static const char* const createString;

	static const char* const insertStmtString;
	static const int INSERT_PARAM_COUNT;

	static const char* const updateStmtString;
	static const int UPDATE_PARAM_COUNT;

	static const char* const deleteStmtString;
	static const int DELETE_PARAM_COUNT;

	static const char* const selectIdStmtString;
	static const int SELECT_ID_PARAM_COUNT;

	MYSQL* mysql_ptr;

	void Update(const string& id, const string& name, const string& address,
		const string& phone, const string& email, const string& website);
	void Insert(const string& id, const string& name, const string& address,
		const string& phone, const string& email, const string& website);

public:
	Provider(Database& database);

	void InsertOrUpdate(const string& id, const string& name, const string& address,
	const string& phone, const string& email, const string& website);
	bool Exists(const string& id);
	void Delete(const string& id);

	// TODO:
	// 一些查询函数
};