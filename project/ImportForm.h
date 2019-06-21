#pragma once
#include "Database.h"

#define IMPORT_SUMMARY_TABLE "importSummary"
#define IMPORT_DETAIL_TABLE "importDetail"

struct ImportSummaryTuple
{
	char iid[31];
	char wid[31];
	float totalAmount;
	char date[21];
	char remark[101];
};

struct ImportDetailTuple {
	char iid[31];
	char bid[31];
	char pid[31];
	int count;
	float amount;
};

class ImportForm {
	static const char* const createSummaryString;
	static const char* const createDetailString;

	static const char* const dropInsertTrigger;
	static const char* const createInsertTrigger;

	static const char* const dropUpdateTrigger;
	static const char* const createUpdateTrigger;

	static const char* const dropDeleteTrigger;
	static const char* const createDeleteTrigger;

	static const char* const insertSummaryStmtString;
	static const int INSERT_SUMMARY_PARAM_COUNT;

	static const char* const insertDetailStmtString;
	static const int INSERT_DETAIL_PARAM_COUNT;

	static const char* const queryByBookIdStmt;
	static const int QUERY_BY_BOOKID_PARAM_COUNT;

	static const char* const queryByProviderIdStmt;
	static const int QUERY_BY_PROVIDERID_PARAM_COUNT;

	static const char* const queryByDateStmt;
	static const int QUERY_BY_DATE_PARAM_COUNT;

	static const char* const queryByImportIdStmt;
	static const int QUERY_BY_IMPORTID_PARAM_COUNT;

	static void SetSummaryInfoResultBind(MYSQL_BIND* resultBind, ImportSummaryTuple& item);
	static void SetDetailInfoResultBind(MYSQL_BIND* resultBind, ImportDetailTuple& item);

	MYSQL* mysql_ptr;
public:

	ImportForm(Database& database);
	void InsertIntoSummaryForm(const string& iid, const string& wid, float totalAmount, const string& remark);
	void InsertIntoDetailForm(const string& iid, const string& bid, const string& pid, int count, float amount);
	void printImportForm() {}

	ImportDetailTuple* queryDetailByBookId(const string& bid, my_ulonglong*) const;
	void queryByProviderId(string pid);
	void queryByDate(string date);
	void queryDetail(string iid);

	// TODO
	// 查询季度进货情况
	// 查询进货id的具体信息

};