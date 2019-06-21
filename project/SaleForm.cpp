#include "SaleForm.h"
#include "Book.h"

const char* const SaleForm::createSummaryString = "CREATE TABLE " DATABASE "." SALE_SUMMARY_TABLE "(\n\
sid CHAR(30) NOT NULL,\n\
wid CHAR(30) NOT NULL,\n\
totalAmount FLOAT NOT NULL,\n\
payMethod VARCHAR(100) NOT NULL,\n\
date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,\n\
remark VARCHAR(100) NULL,\n\
PRIMARY KEY(sid),\n\
FOREIGN KEY(wid) REFERENCES staff(sid),\n\
INDEX saleSmyIndex(sid ASC));\n\
";

const char* const SaleForm::createDetailString = "CREATE TABLE " DATABASE "." SALE_DETAIL_TABLE "(\n\
sid CHAR(30) NOT NULL,\n\
bid CHAR(30) NOT NULL,\n\
count INT NOT NULL DEFAULT 1,\n\
amount FLOAT NOT NULL,\n\
PRIMARY KEY(sid, bid),\n\
FOREIGN KEY(sid) REFERENCES salesummary(sid),\n\
FOREIGN KEY(bid) REFERENCES book(bid),\n\
INDEX saleDtlIndex(sid ASC));\n\
";

const char* const SaleForm::dropInsertTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" SALE_DETAIL_TABLE "_BEFORE_INSERT`";

const char* const SaleForm::createInsertTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" SALE_DETAIL_TABLE "_BEFORE_INSERT` \
BEFORE INSERT ON `" SALE_DETAIL_TABLE "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` - NEW.bcount WHERE(`bid` = NEW.bid);\n\
END";

const char* const SaleForm::dropUpdateTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" SALE_DETAIL_TABLE "_BEFORE_UPDATE`";

const char* const SaleForm::createUpdateTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" SALE_DETAIL_TABLE "_BEFORE_UPDATE` \
BEFORE UPDATE ON `" SALE_DETAIL_TABLE "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` - NEW.bcount WHERE(`bid` = NEW.bid);\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` + OLD.bcount WHERE(`bid` = OLD.bid);\n\
END";

const char* const SaleForm::dropDeleteTrigger = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" SALE_DETAIL_TABLE "_BEFORE_DELETE`";

const char* const SaleForm::createDeleteTrigger = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" SALE_DETAIL_TABLE "_BEFORE_DELETE`\
BEFORE DELETE ON `" SALE_DETAIL_TABLE "` FOR EACH ROW\n\
BEGIN\n\
UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET `blibcount` = `blibcount` + OLD.bcount WHERE(`bid` = OLD.bid);\n\
END";

const char* const SaleForm::insertSummaryStmtString = "INSERT INTO " DATABASE "." SALE_SUMMARY_TABLE "(\n\
`sid`, `wid`, `totalAmount`, `payMethod`, `remark`) VALUES(\n\
?, ?, ?, ?, ?);";
const int SaleForm::INSERT_SUMMARY_PARAM_COUNT = 5;

const char* const SaleForm::insertDetailStmtString = "INSERT INTO " DATABASE "." SALE_DETAIL_TABLE "(\n\
`sid`, `bid`, `count`, `amount`) VALUES(\n\
?, ?, ?, ?);";
const int SaleForm::INSERT_DETAIL_PARAM_COUNT = 4;

const char* const SaleForm::selectSummaryByDateStmtString = "SELECT * FROM `" DATABASE "`.`" SALE_SUMMARY_TABLE "` WHERE DATE_FORMAT(date, '%Y-%m-%d') = ?-?-? ";
const int SaleForm::SELECT_SUMMARY_BY_DATE_PARAM_COUNT = 3;


SaleForm::SaleForm(Database& database) : mysql_ptr(database.GetMysql())
{
	if (mysql_list_tables(mysql_ptr, SALE_SUMMARY_TABLE)->row_count == 0) {
		mysql_query(mysql_ptr, createSummaryString);
	}
	if (mysql_list_tables(mysql_ptr, SALE_DETAIL_TABLE)->row_count == 0) {
		mysql_query(mysql_ptr, createDetailString);
		mysql_query(mysql_ptr, dropInsertTrigger);
		mysql_query(mysql_ptr, createInsertTrigger);
		mysql_query(mysql_ptr, dropUpdateTrigger);
		mysql_query(mysql_ptr, createUpdateTrigger);
		mysql_query(mysql_ptr, dropDeleteTrigger);
		mysql_query(mysql_ptr, createDeleteTrigger);
	}
}

void SaleForm::InsertIntoSummaryForm(const string& sid, const string& wid, float totalAmount, const string& payMethod, const string& remark)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertSummaryStmtString, INSERT_SUMMARY_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)sid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = sid.length();

	bind[1].buffer = (void*)wid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = wid.length();

	bind[2].buffer = (void*)& totalAmount;
	bind[2].buffer_type = MYSQL_TYPE_FLOAT;
	bind[2].buffer_length = sizeof(totalAmount);

	bind[3].buffer = (void*)payMethod.c_str();
	bind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[3].buffer_length = payMethod.length();

	bind[4].buffer = (void*)remark.c_str();
	bind[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[4].buffer_length = remark.length();

	Database::ExecuteStmt(stmt, bind);
}

void SaleForm::InsertIntoDetailForm(const string& sid, const string& bid, int count, float amount)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertDetailStmtString, INSERT_DETAIL_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)sid.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = sid.length();

	bind[1].buffer = (void*)bid.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bid.length();

	bind[2].buffer = (void*)& count;
	bind[2].buffer_type = MYSQL_TYPE_LONG;
	bind[2].buffer_length = sizeof(count);

	bind[3].buffer = (void*)& amount;
	bind[3].buffer_type = MYSQL_TYPE_FLOAT;
	bind[3].buffer_length = sizeof(amount);

	Database::ExecuteStmt(stmt, bind);
}

void SaleForm::QuerySummaryByDate(const string& year, const string& month, const string& day)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, selectSummaryByDateStmtString, SELECT_SUMMARY_BY_DATE_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)year.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = year.length();

	bind[1].buffer = (void*)month.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = month.length();

	bind[2].buffer = (void*)day.c_str();
	bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[2].buffer_length = day.length();

	Database::ExecuteStmt(stmt, bind);
}
