#include "ImportForm.h"
#include "Book.h"

const char* const ImportForm::createSummaryString = "CREATE TABLE " DATABASE "." IMPORT_SUMMARY_TABLE "(\n\
iid CHAR(30) NOT NULL,\n\
wid CHAR(30) NOT NULL,\n\
totalAmount FLOAT NOT NULL,\n\
date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,\n\
remark VARCHAR(100) NULL,\n\
PRIMARY KEY(iid),\n\
FOREIGN KEY(wid) REFERENCES staff(sid),\n\
INDEX iptSmyIndex (iid ASC));\n\
";

const char* const ImportForm::createDetailString = "CREATE TABLE " DATABASE "." IMPORT_DETAIL_TABLE "(\n\
iid CHAR(30) NOT NULL,\n\
bid CHAR(30) NOT NULL,\n\
pid CHAR(30) NOT NULL,\n\
count INT NOT NULL DEFAULT 1,\n\
amount FLOAT NOT NULL,\n\
PRIMARY KEY(iid, bid),\n\
FOREIGN KEY(iid) REFERENCES importsummary(iid),\n\
FOREIGN KEY(bid) REFERENCES book(bid),\n\
FOREIGN KEY(pid) REFERENCES provider(pid),\n\
INDEX iptDtlIndex (iid ASC));\n\
";

const char* const ImportForm::dropInsertTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" IMPORT_DETAIL_TABLE "_BEFORE_INSERT`";

const char* const ImportForm::createInsertTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" IMPORT_DETAIL_TABLE "_BEFORE_INSERT` \
BEFORE INSERT ON `" IMPORT_DETAIL_TABLE "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` + NEW.count WHERE(`bid` = NEW.bid);\n\
END";

const char* const ImportForm::dropUpdateTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" IMPORT_DETAIL_TABLE "_BEFORE_UPDATE`";

const char* const ImportForm::createUpdateTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" IMPORT_DETAIL_TABLE "_BEFORE_UPDATE` \
BEFORE UPDATE ON `" IMPORT_DETAIL_TABLE "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` + NEW.count WHERE(`bid` = NEW.bid);\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` - OLD.count WHERE(`bid` = OLD.bid);\n\
END";

const char* const ImportForm::dropDeleteTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" IMPORT_DETAIL_TABLE "_BEFORE_DELETE`";

const char* const ImportForm::createDeleteTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" IMPORT_DETAIL_TABLE "_BEFORE_DELETE`\
BEFORE DELETE ON `" IMPORT_DETAIL_TABLE "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` - OLD.count WHERE(`bid` = OLD.bid);\n\
END";

const char* const ImportForm::insertSummaryStmtString = "INSERT INTO " DATABASE "." IMPORT_SUMMARY_TABLE "(\n\
iid, wid, totalAmount, remark) VALUES(\n\
?, ?, ?, ?);";
const int ImportForm::INSERT_SUMMARY_PARAM_COUNT = 4;

const char* const ImportForm::insertDetailStmtString = "INSERT INTO " DATABASE "." IMPORT_DETAIL_TABLE "(\n\
iid, bid, pid, count, amount) VALUES(\n\
?, ?, ?, ?, ?);";
const int ImportForm::INSERT_DETAIL_PARAM_COUNT = 5;

static const char* const queryByProviderIdStmt;

static const int QUERY_BY_PROVIDERID_PARAM_COUNT;

static const char* const queryByDateStmt;

static const int QUERY_BY_DATE_PARAM_COUNT;

static const char* const queryByImportIdStmt;

static const int QUERY_BY_IMPORTID_PARAM_COUNT;



ImportForm::ImportForm(Database& database) : mysql_ptr(database.GetMysql())
{
	if (mysql_list_tables(mysql_ptr, IMPORT_SUMMARY_TABLE)->row_count == 0) {
		mysql_query(mysql_ptr, createSummaryString);

	}
	if (mysql_list_tables(mysql_ptr, IMPORT_DETAIL_TABLE)->row_count == 0) {
		mysql_query(mysql_ptr, createDetailString);
		mysql_query(mysql_ptr, dropInsertTrigger);
		mysql_query(mysql_ptr, createInsertTrigger);
		mysql_query(mysql_ptr, dropUpdateTrigger);
		mysql_query(mysql_ptr, createUpdateTrigger);
		mysql_query(mysql_ptr, dropDeleteTrigger);
		mysql_query(mysql_ptr, createDeleteTrigger);
	}
}

void ImportForm::InsertIntoSummaryForm(const string& iid, const string& wid, float totalAmount, const string& remark)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertSummaryStmtString, INSERT_SUMMARY_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)iid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = iid.length();

	bind[1].buffer = (void*)wid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = wid.length();

	bind[2].buffer = (void*)& totalAmount;
	bind[2].buffer_type = MYSQL_TYPE_FLOAT;
	bind[2].buffer_length = sizeof(totalAmount);

	bind[3].buffer = (void*)remark.c_str();
	bind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[3].buffer_length = remark.length();

	Database::ExecuteStmt(stmt, bind);
}

void ImportForm::InsertIntoDetailForm(const string& iid, const string& bid, const string& pid, int count, float amount)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertDetailStmtString, INSERT_DETAIL_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)iid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = iid.length();

	bind[1].buffer = (void*)bid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bid.length();

	bind[2].buffer = (void*)pid.c_str();
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer_length = pid.length();

	bind[3].buffer = (void*)& count;
	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer_length = sizeof(count);

	bind[4].buffer = (void*)& amount;
	bind[4].buffer_type = MYSQL_TYPE_FLOAT;
	bind[4].buffer_length = sizeof(amount);

	Database::ExecuteStmt(stmt, bind);
}

void ImportForm::SetSummaryInfoResultBind(MYSQL_BIND* resultBind, ImportSummaryTuple& item)
{
	resultBind[0].buffer = item.iid;
	resultBind[0].buffer_type = MYSQL_TYPE_STRING;
	resultBind[0].buffer_length = 30;

	resultBind[1].buffer = item.wid;
	resultBind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[1].buffer_length = 30;

	resultBind[2].buffer = &item.totalAmount;
	resultBind[2].buffer_type = MYSQL_TYPE_FLOAT;
	resultBind[2].buffer_length = sizeof(item.totalAmount);

	resultBind[3].buffer = item.date;
	resultBind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[3].buffer_length = 20;

	resultBind[4].buffer = item.remark;
	resultBind[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[4].buffer_length = 100;

}

void ImportForm::SetDetailInfoResultBind(MYSQL_BIND* resultBind, ImportDetailTuple& item)
{
	resultBind[0].buffer = item.iid;
	resultBind[0].buffer_type = MYSQL_TYPE_STRING;
	resultBind[0].buffer_length = 30;

	resultBind[1].buffer = item.bid;
	resultBind[1].buffer_type = MYSQL_TYPE_STRING;
	resultBind[1].buffer_length = 30;

	resultBind[2].buffer = &item.pid;
	resultBind[2].buffer_type = MYSQL_TYPE_STRING;
	resultBind[2].buffer_length = 30;

	resultBind[3].buffer = &item.count;
	resultBind[3].buffer_type = MYSQL_TYPE_LONG;
	resultBind[3].buffer_length = sizeof(item.count);

	resultBind[4].buffer = &item.amount;
	resultBind[4].buffer_type = MYSQL_TYPE_FLOAT;
	resultBind[4].buffer_length = sizeof(item.amount);
}

ImportDetailTuple* ImportForm::queryDetailByBookId(const string& bid, my_ulonglong* outSize) const
{
	static const char* const stmtString = "SELECT * FROM " DATABASE "." IMPORT_DETAIL_TABLE " WHERE bid = ?";
	static const int PARAM_COUNT = 1;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)bid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = bid.length();

	Database::ExecuteStmt(stmt, bind, false);

	*outSize = mysql_stmt_num_rows(stmt);
	ImportDetailTuple* items = new ImportDetailTuple[*outSize]();
	ImportDetailTuple item;

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

void ImportForm::queryByProviderId(string providerId)
{
}

void ImportForm::queryByDate(string date)
{
}

void ImportForm::queryDetail(string iid)
{
}



