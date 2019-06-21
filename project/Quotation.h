#pragma once
#include "Database.h"
#include "Book.h"
#include "Provider.h"

#define QUOTATION_TABLE_NAME "quotation"

struct QuotationTuple
{
	char bid[31];
	char bname[301];
	char pid[31];
	char pname[301];
	double qprice;
	int qminnumber;
	char qtime[31];
};

class Quotation
{
	static const char* const createString;
	static const char* const dropInsertTriggerString;
	static const char* const createInsertTriggerString;
	static const char* const dropUpdateTriggerString;
	static const char* const createUpdateTriggerString;

	static const char* const insertStmtString;
	static const int INSERT_PARAM_COUNT;

	static const char* const updateStmtString;
	static const int UPDATE_PARAM_COUNT;

	static const char* const deleteStmtString;
	static const int DELETE_PARAM_COUNT;

	static const char* const selectIdStmtString;
	static const int SELECT_ID_PARAM_COUNT;

	MYSQL* mysql_ptr;

	void Update(const string& providerId, const string& bookId, double price, int minNumber);
	void Insert(const string& providerId, const string& bookId, double price, int minNumber);

	static void SetInfoResultBind(MYSQL_BIND* resultBind, QuotationTuple& item);

public:
	Quotation(Database& database);

	void InsertOrUpdate(const string& providerId, const string& bookId, double price, int minNumber);
	bool Exists(const string& providerId, const string& bookId);
	void Delete(const string& providerId, const string& bookId);
	// void PrintQuotation();
	// void PrintQuotation(string bookId);
	QuotationTuple* GetInfo(const string& bookId, my_ulonglong* outSize) const;
};