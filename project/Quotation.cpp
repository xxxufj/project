#include "Quotation.h"

const char* const Quotation::createString = "CREATE TABLE `" DATABASE "`.`" QUOTATION_TABLE_NAME "` (\n\
`pid` CHAR(30) NOT NULL,\n\
`bid` CHAR(30) NOT NULL,\n\
`qprice` DECIMAL(10, 2) NOT NULL DEFAULT 0,\n\
`qminnumber` INT NOT NULL DEFAULT 1,\n\
`qtime` DATETIME NOT NULL DEFAULT NOW(),\n\
PRIMARY KEY(`pid`, `bid`),\n\
INDEX `bidref_idx` (`bid` ASC) VISIBLE,\n\
INDEX `pidref_idx` (`pid` ASC) VISIBLE,\n\
CONSTRAINT `pidref`\n\
  FOREIGN KEY(`pid`)\n\
  REFERENCES `" DATABASE "`.`" PROVIDER_TABLE_NAME "` (`pid`)\n\
  ON DELETE NO ACTION\n\
  ON UPDATE NO ACTION, \n\
CONSTRAINT `bidref`\n\
  FOREIGN KEY(`bid`)\n\
  REFERENCES `" DATABASE "`.`" BOOK_TABLE_NAME "` (`bid`)\n\
  ON DELETE NO ACTION\n\
  ON UPDATE NO ACTION)";

const char* const Quotation::createInsertTriggerString = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" QUOTATION_TABLE_NAME "_BEFORE_INSERT` \
BEFORE INSERT ON `" QUOTATION_TABLE_NAME "` FOR EACH ROW\n\
BEGIN\n\
SET NEW.qtime = NOW();\n\
END";

const char* const Quotation::dropInsertTriggerString = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" QUOTATION_TABLE_NAME "_BEFORE_INSERT`";

const char* const Quotation::createUpdateTriggerString = "CREATE DEFINER = CURRENT_USER TRIGGER `" DATABASE "`.`" QUOTATION_TABLE_NAME "_BEFORE_UPDATE` \
BEFORE UPDATE ON `" QUOTATION_TABLE_NAME "` FOR EACH ROW\n\
BEGIN\n\
SET NEW.qtime = NOW();\n\
END";

const char* const Quotation::dropUpdateTriggerString = "DROP TRIGGER IF EXISTS `" DATABASE "`.`" QUOTATION_TABLE_NAME "_BEFORE_UPDATE`";

const char* const Quotation::insertStmtString = "INSERT INTO `" DATABASE "`.`" QUOTATION_TABLE_NAME "` (\
`pid`, `bid`, `qprice`, `qminnumber`) VALUES (\
?, ?, ?, ?)";

const int Quotation::INSERT_PARAM_COUNT = 4;

const char* const Quotation::updateStmtString = "UPDATE `" DATABASE "`.`" QUOTATION_TABLE_NAME "` SET \
`qprice` = ?, `qminnumber` = ? WHERE (`pid` = ?) and (`bid` = ?)";

const int Quotation::UPDATE_PARAM_COUNT = 4;

const char* const Quotation::deleteStmtString = "DELETE FROM `" DATABASE "`.`" QUOTATION_TABLE_NAME "` WHERE (`pid` = ?) and (`bid` = ?)";

const int Quotation::DELETE_PARAM_COUNT = 2;

const char* const Quotation::selectIdStmtString = "SELECT * FROM `" DATABASE "`.`" QUOTATION_TABLE_NAME "` WHERE (`pid` = ?) and (`bid` = ?)";

const int Quotation::SELECT_ID_PARAM_COUNT = 2;

void Quotation::Update(const string& providerId, const string& bookId, double price, int minNumber)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, updateStmtString, UPDATE_PARAM_COUNT, &stmt);

	bind[2].buffer = (void*)providerId.c_str();
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer_length = providerId.length();

	bind[3].buffer = (void*)bookId.c_str();
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer_length = bookId.length();

	bind[0].buffer = (void*)& price;
	bind[0].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[0].buffer_length = sizeof(price);

	bind[1].buffer = (void*)& minNumber;
	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer_length = sizeof(minNumber);

	Database::ExecuteStmt(stmt, bind);
}

void Quotation::Insert(const string& providerId, const string& bookId, double price, int minNumber)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertStmtString, INSERT_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)providerId.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = providerId.length();

	bind[1].buffer = (void*)bookId.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bookId.length();

	bind[2].buffer = (void*)& price;
	bind[2].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[2].buffer_length = sizeof(price);

	bind[3].buffer = (void*)& minNumber;
	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer_length = sizeof(minNumber);

	Database::ExecuteStmt(stmt, bind);
}

void Quotation::SetInfoResultBind(MYSQL_BIND* resultBind, QuotationTuple& item)
{
	resultBind[0].buffer = item.bid;
	resultBind[0].buffer_type = MYSQL_TYPE_STRING;
	resultBind[0].buffer_length = 30;

	resultBind[1].buffer = item.bname;
	resultBind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[1].buffer_length = 300;

	resultBind[2].buffer = item.pid;
	resultBind[2].buffer_type = MYSQL_TYPE_STRING;
	resultBind[2].buffer_length = 30;

	resultBind[3].buffer = item.pname;
	resultBind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[3].buffer_length = 300;

	resultBind[4].buffer = &item.qprice;
	resultBind[4].buffer_type = MYSQL_TYPE_DOUBLE;
	resultBind[4].buffer_length = sizeof(item.qprice);

	resultBind[5].buffer = &item.qminnumber;
	resultBind[5].buffer_type = MYSQL_TYPE_LONG;
	resultBind[5].buffer_length = sizeof(item.qminnumber);

	resultBind[6].buffer = item.qtime;
	resultBind[6].buffer_type = MYSQL_TYPE_DATETIME;
	resultBind[6].buffer_length = 30;
}

Quotation::Quotation(Database& database) : mysql_ptr(database.GetMysql())
{
	if (mysql_list_tables(mysql_ptr, QUOTATION_TABLE_NAME)->row_count == 0)
	{
		mysql_query(mysql_ptr, createString);
		mysql_query(mysql_ptr, dropInsertTriggerString);
		mysql_query(mysql_ptr, createInsertTriggerString);
		mysql_query(mysql_ptr, dropUpdateTriggerString);
		mysql_query(mysql_ptr, createUpdateTriggerString);
	}
}

void Quotation::InsertOrUpdate(const string& providerId, const string& bookId, double price, int minNumber)
{
	if (!Exists(providerId, bookId))
	{
		Insert(providerId, bookId, price, minNumber);
	}
	else
	{
		Update(providerId, bookId, price, minNumber);
	}
}

bool Quotation::Exists(const string& providerId, const string& bookId)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, selectIdStmtString, SELECT_ID_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)providerId.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = providerId.length();

	bind[1].buffer = (void*)bookId.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bookId.length();

	Database::ExecuteStmt(stmt, bind, false);

	int c = mysql_stmt_num_rows(stmt);

	Database::FreeStmt(stmt, bind);
	return c != 0 && c != -1;
}

void Quotation::Delete(const string& providerId, const string& bookId)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteStmtString, DELETE_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)providerId.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = providerId.length();

	bind[1].buffer = (void*)bookId.c_str();
	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer_length = bookId.length();

	Database::ExecuteStmt(stmt, bind);
}

QuotationTuple* Quotation::GetInfo(const string& bookId, my_ulonglong* outSize) const
{
	static const char* const stmtString = "SELECT B.bid, B.bname, P.pid, P.pname, Q.qprice, Q.qminnumber, Q.qtime\n\
FROM `" DATABASE "`.`" BOOK_TABLE_NAME "` B, `" DATABASE "`.`" QUOTATION_TABLE_NAME "` Q, `" DATABASE "`.`" PROVIDER_TABLE_NAME "` P\n\
WHERE B.bid = ? AND Q.bid = B.bid AND Q.pid = P.pid";
	static const int EXPECTED_PARAM_COUNT = 1;
	static const int RESULT_COUNT = 7;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, EXPECTED_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)bookId.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = bookId.length();

	Database::ExecuteStmt(stmt, bind, false);
	QuotationTuple item;
	MYSQL_BIND* resultBind = new MYSQL_BIND[RESULT_COUNT]();
	SetInfoResultBind(resultBind, item);
	mysql_stmt_bind_result(stmt, resultBind);
	*outSize = mysql_stmt_num_rows(stmt);
	QuotationTuple* items = new QuotationTuple[*outSize]();

	for (my_ulonglong i = 0; i < *outSize; i++)
	{
		mysql_stmt_fetch(stmt);
		memcpy(items + i, &item, sizeof(item));
	}
	delete[] resultBind;
	Database::FreeStmt(stmt, bind);
	return items;
}
