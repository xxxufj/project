#pragma once
#include "Database.h"

#define BOOK_TABLE_NAME "book"

struct BookTuple
{
	char bid[31];
	char bname[301];
	char bauthor[301];
	char bpublisher[301];
	int byear;
	int blibcount;
	double bsellprice;
};

class Book
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

	void Update(const string& id, const string& name, const string& author, const string& publisher, int year, double sellPrice);
	void Insert(const string& id, const string& name, const string& author, const string& publisher, int year, double sellPrice);

	static void SetInfoResultBind(MYSQL_BIND* resultBind, BookTuple& item);

public:
	Book(Database& database);

	void InsertOrUpdate(const string& id, const string& name, const string& author, const string& publisher, int year, double sellPrice);
	bool Exists(const string& id);
	void Delete(const string& id);
	int GetBookCount(const string& id) const;

	BookTuple GetInfo(const string& id) const;
	BookTuple* GetStock(my_ulonglong* outSize) const;
	// TODO:
	// 一些查询函数
};