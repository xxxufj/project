#include "Book.h"
#include <iostream>
#include <cstring>

using namespace std;

const char* const Book::createString = "CREATE TABLE `" DATABASE "`.`" BOOK_TABLE_NAME "` (\n\
`bid` CHAR(30) NOT NULL,\n\
`bname` VARCHAR(100) NOT NULL,\n\
`bauthor` VARCHAR(100) NULL,\n\
`bpublisher` VARCHAR(100) NULL,\n\
`byear` INT NULL,\n\
`blibcount` INT NOT NULL DEFAULT 0,\n\
`bsellprice` DECIMAL(10, 2) NOT NULL DEFAULT 0,\n\
PRIMARY KEY(`bid`),\n\
INDEX `bnameindex` (`bname` ASC) VISIBLE)\n\
";

const char* const Book::insertStmtString = "INSERT INTO `" DATABASE "`.`" BOOK_TABLE_NAME "` (\n\
`bid`, `bname`, `bauthor`, `bpublisher`, `byear`, `bsellprice`) VALUES (?, ?, ?, ?, ?, ?)";

const int Book::INSERT_PARAM_COUNT = 6;

const char* const Book::updateStmtString = "UPDATE `" DATABASE "`.`" BOOK_TABLE_NAME "` SET \
`bname` = ?, `bauthor` = ?, `bpublisher` = ?, `byear` = ?, `bsellprice` = ? WHERE (\
`bid` = ?)";

const int Book::UPDATE_PARAM_COUNT = 6;

const char* const Book::deleteStmtString = "DELETE FROM `" DATABASE "`.`" BOOK_TABLE_NAME "` WHERE (`bid` = ?)";

const int Book::DELETE_PARAM_COUNT = 1;

const char* const Book::selectIdStmtString = "SELECT * FROM `" DATABASE "`.`" BOOK_TABLE_NAME "` WHERE (`bid` = ?)";

const int Book::SELECT_ID_PARAM_COUNT = 1;

void Book::Update(const string& id, const string& name, const string& author, const string& publisher, int year, double sellPrice)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, updateStmtString, UPDATE_PARAM_COUNT, &stmt);

	bind[5].buffer = (void*)id.c_str();
	bind[5].buffer_type = MYSQL_TYPE_STRING;
	bind[5].buffer_length = id.length();

	bind[0].buffer = (void*)name.c_str();
	bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[0].buffer_length = name.length();

	bind[1].buffer = (void*)author.c_str();
	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[1].buffer_length = author.length();

	bind[2].buffer = (void*)publisher.c_str();
	bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[2].buffer_length = publisher.length();

	bind[3].buffer = (void*)& year;
	bind[3].buffer_type = MYSQL_TYPE_LONG;
	bind[3].buffer_length = sizeof(year);

	bind[4].buffer = (void*)& sellPrice;
	bind[4].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[4].buffer_length = sizeof(sellPrice);

	Database::ExecuteStmt(stmt, bind);
}

void Book::Insert(const string& id, const string& name, const string& author, const string& publisher, int year, double sellPrice)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertStmtString, INSERT_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	bind[1].buffer = (void*)name.c_str();
	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[1].buffer_length = name.length();

	bind[2].buffer = (void*)author.c_str();
	bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[2].buffer_length = author.length();

	bind[3].buffer = (void*)publisher.c_str();
	bind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[3].buffer_length = publisher.length();

	bind[4].buffer = (void*)& year;
	bind[4].buffer_type = MYSQL_TYPE_LONG;
	bind[4].buffer_length = sizeof(year);

	bind[5].buffer = (void*)& sellPrice;
	bind[5].buffer_type = MYSQL_TYPE_DOUBLE;
	bind[5].buffer_length = sizeof(sellPrice);

	Database::ExecuteStmt(stmt, bind);
}

void Book::SetInfoResultBind(MYSQL_BIND* resultBind, BookTuple& item)
{
	resultBind[0].buffer = item.bid;
	resultBind[0].buffer_type = MYSQL_TYPE_STRING;
	resultBind[0].buffer_length = 30;

	resultBind[1].buffer = item.bname;
	resultBind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[1].buffer_length = 300;

	resultBind[2].buffer = item.bauthor;
	resultBind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[2].buffer_length = 300;

	resultBind[3].buffer = item.bpublisher;
	resultBind[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	resultBind[3].buffer_length = 300;

	resultBind[4].buffer = &item.byear;
	resultBind[4].buffer_type = MYSQL_TYPE_LONG;
	resultBind[4].buffer_length = sizeof(item.byear);

	resultBind[5].buffer = &item.blibcount;
	resultBind[5].buffer_type = MYSQL_TYPE_LONG;
	resultBind[5].buffer_length = sizeof(item.blibcount);

	resultBind[6].buffer = &item.bsellprice;
	resultBind[6].buffer_type = MYSQL_TYPE_DOUBLE;
	resultBind[6].buffer_length = sizeof(item.bsellprice);
}

Book::Book(Database& database) : mysql_ptr(database.GetMysql())
{
	if (mysql_list_tables(mysql_ptr, BOOK_TABLE_NAME)->row_count == 0)
	{
		mysql_query(mysql_ptr, createString);
	}
}

void Book::InsertOrUpdate(const string& id, const string& name, const string& author, const string& publisher, int year, double sellPrice)
{
	if (!Exists(id))
	{
		Insert(id, name, author, publisher, year, sellPrice);
	}
	else
	{
		Update(id, name, author, publisher, year, sellPrice);
	}
}

bool Book::Exists(const string& id)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, selectIdStmtString, SELECT_ID_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	Database::ExecuteStmt(stmt, bind, false);

	int c = mysql_stmt_num_rows(stmt);

	Database::FreeStmt(stmt, bind);
	return c != 0 && c != -1;
}

void Book::Delete(const string& id)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteStmtString, DELETE_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	Database::ExecuteStmt(stmt, bind);
}

int Book::GetBookCount(const string& id) const
{
	static const char* const stmtString = "SELECT blibcount FROM " DATABASE "." BOOK_TABLE_NAME " WHERE bid = ?";
	static const int EXPECTED_PARAM_COUNT = 1;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, EXPECTED_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	Database::ExecuteStmt(stmt, bind, false);

	int ans;

	if (mysql_stmt_num_rows(stmt) == 0)
	{
		ans = 0;
	}
	else
	{
		MYSQL_BIND* resultBind = new MYSQL_BIND[1]();
		resultBind[0].buffer = &ans;
		resultBind[0].buffer_type = MYSQL_TYPE_LONG;
		resultBind[0].buffer_length = sizeof(ans);
		mysql_stmt_bind_result(stmt, resultBind);

		mysql_stmt_fetch(stmt);
		delete[] resultBind;
	}
	Database::FreeStmt(stmt, bind);
	return ans;
}

BookTuple Book::GetInfo(const string& id) const
{
	static const char* const stmtString = "SELECT * FROM " DATABASE "." BOOK_TABLE_NAME " WHERE bid = ?";
	static const int EXPECTED_PARAM_COUNT = 1;
	static const int RESULT_COUNT = 7;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, EXPECTED_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	Database::ExecuteStmt(stmt, bind, false);

	BookTuple item;

	if (mysql_stmt_num_rows(stmt) != 0)
	{
		MYSQL_BIND* resultBind = new MYSQL_BIND[RESULT_COUNT]();

		SetInfoResultBind(resultBind, item);
		mysql_stmt_bind_result(stmt, resultBind);
		mysql_stmt_fetch(stmt);

		delete[] resultBind;
	}
	Database::FreeStmt(stmt, bind);
	return item;
}

BookTuple* Book::GetStock(my_ulonglong* outSize) const
{
	static const char* const stmtString = "SELECT * FROM " DATABASE "." BOOK_TABLE_NAME;
	static const int EXPECTED_PARAM_COUNT = 0;
	static const int RESULT_COUNT = 7;

	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, stmtString, EXPECTED_PARAM_COUNT, &stmt);

	Database::ExecuteStmt(stmt, bind, false);

	*outSize = mysql_stmt_num_rows(stmt);
	BookTuple* items = new BookTuple[*outSize]();
	BookTuple item;

	MYSQL_BIND* resultBind = new MYSQL_BIND[RESULT_COUNT]();

	SetInfoResultBind(resultBind, item);
	mysql_stmt_bind_result(stmt, resultBind);

	for (my_ulonglong i = 0; i < *outSize; i++)
	{
		mysql_stmt_fetch(stmt);
		memcpy(items + i, &item, sizeof(item));
	}

	delete[] resultBind;
	Database::FreeStmt(stmt, bind);

	return items;
}
