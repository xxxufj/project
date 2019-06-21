#include "ReturnForm.h"
#include "Book.h"

const int ReturnForm::INSERT_SUMMARY_PARAM_COUNT = 2;
const int ReturnForm::INSERT_DETAILED_PARAM_COUNT = 5;
const int ReturnForm::DELETE_SUMMARY_PARAM_COUNT = 1;
const int ReturnForm::DELETE_DETAILED_PARAM_COUNT1 = 2;
const int ReturnForm::DELETE_DETAILED_PARAM_COUNT2 = 1;
const int ReturnForm::UPDATE_PARAM_COUNT = 6;

const char* const ReturnForm::createSummaryFrom = "CREATE TABLE `" DATABASE "`.`" RETURN_SUMMARY_FORM "` ("
"rid CHAR(8) NOT NULL,"
"remark VARCHAR(100) NULL,"
"time datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,"
"PRIMARY KEY(rid),"
"INDEX returnIndex(rid)"
")";


const char* const ReturnForm::createDetailedFrom = "CREATE TABLE `" DATABASE "`.`" RETURN_DETAILED_FORM "` ("
"rid CHAR(8) NOT NULL,"
"bid CHAR(30) NOT NULL,"
"bcount INT NOT NULL DEFAULT 1,"
"totalAmount FLOAT NOT NULL,"
"reason VARCHAR(100),"
"PRIMARY KEY(rid, bid),"
"FOREIGN KEY(rid) REFERENCES returnSummary(rid),"
"FOREIGN KEY(bid) REFERENCES book(bid),"
"INDEX detailIndex(rid)"
")";

const char* const ReturnForm::dropInsertTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" RETURN_DETAILED_FORM "_BEFORE_INSERT`";

const char* const ReturnForm::createInsertTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" RETURN_DETAILED_FORM "_BEFORE_INSERT` \
BEFORE INSERT ON `" RETURN_DETAILED_FORM "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` + NEW.bcount WHERE(`bid` = NEW.bid);\n\
END";

const char* const ReturnForm::dropUpdateTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" RETURN_DETAILED_FORM "_BEFORE_UPDATE`";

const char* const ReturnForm::createUpdateTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" RETURN_DETAILED_FORM "_BEFORE_UPDATE` \
BEFORE UPDATE ON `" RETURN_DETAILED_FORM "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` + NEW.bcount WHERE(`bid` = NEW.bid);\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` - OLD.bcount WHERE(`bid` = OLD.bid);\n\
END";

const char* const ReturnForm::dropDeleteTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" RETURN_DETAILED_FORM "_BEFORE_DELETE`";

const char* const ReturnForm::createDeleteTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" RETURN_DETAILED_FORM "_BEFORE_DELETE`\
BEFORE DELETE ON `" RETURN_DETAILED_FORM "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` - OLD.bcount WHERE(`bid` = OLD.bid);\n\
END";

const char* const ReturnForm::insertSummaryString = "INSERT INTO `" DATABASE "`.`" RETURN_SUMMARY_FORM "` (\n\
`rid`, `remark`) VALUES (\n\
?, ?)";

const char* const ReturnForm::insertDetailedString = "INSERT INTO `" DATABASE "`.`" RETURN_DETAILED_FORM "` (\n\
`rid`, `bid`, `bcount`,`totalAmount`, `reason`) VALUES (\n\
?, ?, ?, ?, ?)";

const char* const ReturnForm::deleteSummaryString = "DELETE FROM `" DATABASE "`.`" RETURN_SUMMARY_FORM "` WHERE (`rid` = ?)";

const char* const ReturnForm::deleteDetailedString1 = "DELETE FROM `" DATABASE "`.`" RETURN_DETAILED_FORM "` WHERE (`rid` = ?) AND (`bid` = ?)";

const char* const ReturnForm::deleteDetailedString2 = "DELETE FROM `" DATABASE "`.`" RETURN_DETAILED_FORM "` WHERE (`rid` = ?)";

const char* const ReturnForm::updateStmtString = "UPDATE `" DATABASE "`.`" RETURN_SUMMARY_FORM "` SET `remark` = ? WHERE (`rid` = ?)";


ReturnForm::ReturnForm(Database& database) :mysql_ptr(database.GetMysql()) {
	if (mysql_list_tables(mysql_ptr, RETURN_SUMMARY_FORM)->row_count == 0) {
		mysql_query(mysql_ptr, createSummaryFrom);
	}
	if (mysql_list_tables(mysql_ptr, RETURN_DETAILED_FORM)->row_count == 0) {
		mysql_query(mysql_ptr, createDetailedFrom);
		mysql_query(mysql_ptr, dropInsertTrigger);
		mysql_query(mysql_ptr, createInsertTrigger);
		mysql_query(mysql_ptr, dropUpdateTrigger);
		mysql_query(mysql_ptr, createUpdateTrigger);
		mysql_query(mysql_ptr, dropDeleteTrigger);
		mysql_query(mysql_ptr, createDeleteTrigger);
	}
}

void ReturnForm::InsertIntoSummaryFrom(const string& rid, const string& remark) {
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertSummaryString, INSERT_SUMMARY_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)rid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = rid.length();

	bind[1].buffer = (void*)remark.c_str();
	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[1].buffer_length = remark.length();

	Database::ExecuteStmt(stmt, bind);
}

void ReturnForm::InsertIntoDetailedFrom(const string& rid, const string& bid, int bcount, float totalAmount, const string& reason) {
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertDetailedString, INSERT_DETAILED_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)rid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = rid.length();

	bind[1].buffer = (void*)bid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bid.length();

	bind[2].buffer = (void*)& bcount;
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer_length = sizeof(bcount);

	bind[3].buffer = (void*)& totalAmount;
	bind[3].buffer_type = MYSQL_TYPE_FLOAT;
	bind[3].buffer_length = sizeof(totalAmount);

	bind[4].buffer = (void*)reason.c_str();
	bind[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[4].buffer_length = reason.length();

	Database::ExecuteStmt(stmt, bind);
}

void ReturnForm::deleteFromSummaryFrom(const string& rid) {
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteSummaryString, DELETE_SUMMARY_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)rid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = rid.length();

	Database::ExecuteStmt(stmt, bind);
}

void ReturnForm::deleteFromDetailedFrom(const string& rid, const string& bid) {
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteDetailedString1, DELETE_DETAILED_PARAM_COUNT1, &stmt);

	bind[0].buffer = (void*)rid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = rid.length();

	bind[1].buffer = (void*)bid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bid.length();

	Database::ExecuteStmt(stmt, bind);
}

ReturnDetailTuple* ReturnForm::queryDetailBySerial(const string& rid)const {
	static const char* const stmtString = "SELECT * FROM " DATABASE "." RETURN_DETAILED_FORM " WHERE rid = ?";
	static const int PARAM_COUNT = 1;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)rid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = rid.length();

	Database::ExecuteStmt(stmt, bind, false);

	*outSize = mysql_stmt_num_rows(stmt);
	ReturnDetailTuple* items = new ReturnDetailTuple[*outSize]();
	ReturnDetailTuple item;

	MYSQL_BIND* resultBind = new MYSQL_BIND[5]();

	SetDetailInfoResultBind(resultBind, item);
	mysql_stmt_bind_result(stmt, resultBind);

	for (my_ulonglong i = 0; i < *outSize; i++) {
		mysql_stmt_fetch(stmt);
		memcpy(items + i, &item, sizeof(item));
	}
	Database::FreeStmt(stmt, bind);
	return items;
}

void ReturnForm::deleteFromDetailedFrom(const string& rid) {
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteDetailedString2, DELETE_DETAILED_PARAM_COUNT2, &stmt);

	bind[0].buffer = (void*)rid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = rid.length();

	Database::ExecuteStmt(stmt, bind);
}

void ReturnForm::updateSummaryForm(string rid, string remark) {
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, updateStmtString, UPDATE_PARAM_COUNT, &stmt);

	bind[1].buffer = (void*)rid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = rid.length();

	bind[0].buffer = (void*)remark.c_str();
	bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[0].buffer_length = remark.length();

	Database::ExecuteStmt(stmt, bind);
}

ReturnSummaryTuple* ReturnForm::queryAll()const {

}

void ReturnForm::SetSummaryInfoResultBind(MYSQL_BIND* resultBind, ReturnSummaryTuple& item)
{
	resultBind[0].buffer = item.rid;
	resultBind[0].buffer_type = MYSQL_TYPE_STRING;
	resultBind[0].buffer_length = 30;

	resultBind[1].buffer = item.remark;
	resultBind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[1].buffer_length = 100;

}

void ReturnForm::SetDetailInfoResultBind(MYSQL_BIND* resultBind, ReturnDetailTuple& item)
{
	resultBind[0].buffer = item.rid;
	resultBind[0].buffer_type = MYSQL_TYPE_STRING;
	resultBind[0].buffer_length = 30;

	resultBind[1].buffer = item.bid;
	resultBind[1].buffer_type = MYSQL_TYPE_STRING;
	resultBind[1].buffer_length = 30;

	resultBind[2].buffer = &item.bcount;
	resultBind[2].buffer_type = MYSQL_TYPE_LONG;
	resultBind[2].buffer_length = sizeof(item.bcount);

	resultBind[3].buffer = &item.totalAmount;
	resultBind[3].buffer_type = MYSQL_TYPE_FLOAT;
	resultBind[3].buffer_length = sizeof(item.totalAmount);

	resultBind[4].buffer = &item.reason;
	resultBind[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[4].buffer_length = 100;

}