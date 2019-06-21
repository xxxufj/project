#pragma once
#include "Database.h"

#define RETURN_SUMMARY_FORM "returnSummary"
#define RETURN_DETAILED_FORM "returnDetails"

struct ReturnSummaryTuple
{
	char rid[31];
	char remark[101];
};

struct ReturnDetailTuple {
	char rid[31];
	char bid[31];
	char reason[31];
	int bcount;
	float totalAmount;
};


class ReturnForm {
	static const char* const createSummaryFrom;
	static const char* const createDetailedFrom;

	static const char* const dropInsertTrigger;
	static const char* const createInsertTrigger;

	static const char* const dropUpdateTrigger;
	static const char* const createUpdateTrigger;

	static const char* const dropDeleteTrigger;
	static const char* const createDeleteTrigger;

	static const char* const insertSummaryString;
	static const int INSERT_SUMMARY_PARAM_COUNT;

	static const char* const insertDetailedString;
	static const int INSERT_DETAILED_PARAM_COUNT;

	static const char* const deleteSummaryString;
	static const int DELETE_SUMMARY_PARAM_COUNT;

	static const char* const deleteDetailedString1;
	static const int DELETE_DETAILED_PARAM_COUNT1;

	static const char* const deleteDetailedString2;
	static const int DELETE_DETAILED_PARAM_COUNT2;

	static const char* const updateStmtString;
	static const int UPDATE_PARAM_COUNT;

	static void SetSummaryInfoResultBind(MYSQL_BIND* resultBind, ReturnSummaryTuple& item);
	static void SetDetailInfoResultBind(MYSQL_BIND* resultBind, ReturnDetailTuple& item);

	MYSQL* mysql_ptr;

public:
	ReturnForm() = delete;
	ReturnForm(Database& database);
	void InsertIntoSummaryFrom(const string& rid, const string& remark);
	void InsertIntoDetailedFrom(const string& rid, const string& bid, int bcount, float totalAmount, const string& reason);

	void deleteFromSummaryFrom(const string& rid);
	void deleteFromDetailedFrom(const string& rid, const string& bid);
	void deleteFromDetailedFrom(const string& rid);
	void updateSummaryForm(string rid, string remark);

	ReturnDetailTuple* queryDetailBySerial(const string& serial)const;////// unfinished
	ReturnSummaryTuple* queryAll()const;
};