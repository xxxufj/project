#pragma once
#include "Database.h"

#define SALE_SUMMARY_TABLE "saleSummary"
#define SALE_DETAIL_TABLE "saleDetail"

class SaleForm {
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


	static const char* const selectSummaryByDateStmtString;
	static const int SELECT_SUMMARY_BY_DATE_PARAM_COUNT;

	MYSQL* mysql_ptr;

public:
	SaleForm(Database& database);

	void InsertIntoSummaryForm(const string& sid, const string& wid, float totalAmount, const string& payMethod, const string& remark);
	void InsertIntoDetailForm(const string& sid, const string& bid, int count, float amount);
	void QuerySummaryByDate(const string& year, const string& month, const string& day);
	void printSale() {}

	// TODO
	// 查询月销售情况
	// 查询销售id的具体信息
};