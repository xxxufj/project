#include "Provider.h"

const char* const Provider::createString = "CREATE TABLE `" DATABASE "`.`" PROVIDER_TABLE_NAME "` (\n\
`pid` CHAR(30) NOT NULL,\n\
`pname` VARCHAR(100) NOT NULL,\n\
`paddress` TINYTEXT NOT NULL,\n\
`pphone` CHAR(20) NOT NULL,\n\
`pemail` CHAR(50) NULL,\n\
`pwebsite` CHAR(50) NULL,\n\
PRIMARY KEY(`pid`),\n\
INDEX `pnameindex` (`pname` ASC) VISIBLE)";

const char* const Provider::insertStmtString = "INSERT INTO `" DATABASE "`.`" PROVIDER_TABLE_NAME "` (\
`pid`, `pname`, `paddress`, `pphone`, `pemail`, `pwebsite`) VALUES (\
?, ?, ?, ?, ?, ?)";

const int Provider::INSERT_PARAM_COUNT = 6;

const char* const Provider::updateStmtString = "UPDATE `" DATABASE "`.`" PROVIDER_TABLE_NAME "` SET \
`pname` = ?, `paddress` = ?, `pphone` = ?, `pemail` = ?, `pwebsite` = ? WHERE (`pid` = ?)";

const int Provider::UPDATE_PARAM_COUNT = 6;

const char* const Provider::deleteStmtString = "DELETE FROM `" DATABASE "`.`" PROVIDER_TABLE_NAME "` WHERE (`pid` = ?)";

const int Provider::DELETE_PARAM_COUNT = 1;

const char* const Provider::selectIdStmtString = "SELECT * FROM `" DATABASE "`.`" PROVIDER_TABLE_NAME "` WHERE (`pid` = ?)";

const int Provider::SELECT_ID_PARAM_COUNT = 1;

void Provider::Update(const string& id, const string& name, const string& address, const string& phone, const string& email, const string& website)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, updateStmtString, UPDATE_PARAM_COUNT, &stmt);

	bind[5].buffer = (void*)id.c_str();
	bind[5].buffer_type = MYSQL_TYPE_STRING;
	bind[5].buffer_length = id.length();

	bind[0].buffer = (void*)name.c_str();
	bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[0].buffer_length = name.length();

	bind[1].buffer = (void*)address.c_str();
	bind[1].buffer_type = MYSQL_TYPE_BLOB;
	bind[1].buffer_length = address.length();

	bind[2].buffer = (void*)phone.c_str();
	bind[2].buffer_type = MYSQL_TYPE_STRING;
	bind[2].buffer_length = phone.length();

	bind[3].buffer = (void*)email.c_str();
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer_length = email.length();

	bind[4].buffer = (void*)website.c_str();
	bind[4].buffer_type = MYSQL_TYPE_STRING;
	bind[4].buffer_length = website.length();

	Database::ExecuteStmt(stmt, bind);
}

void Provider::Insert(const string& id, const string& name, const string& address, const string& phone, const string& email, const string& website)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, insertStmtString, INSERT_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	bind[1].buffer = (void*)name.c_str();
	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	bind[1].buffer_length = name.length();

	bind[2].buffer = (void*)address.c_str();
	bind[2].buffer_type = MYSQL_TYPE_BLOB;
	bind[2].buffer_length = address.length();

	bind[3].buffer = (void*)phone.c_str();
	bind[3].buffer_type = MYSQL_TYPE_STRING;
	bind[3].buffer_length = phone.length();

	bind[4].buffer = (void*)email.c_str();
	bind[4].buffer_type = MYSQL_TYPE_STRING;
	bind[4].buffer_length = email.length();

	bind[5].buffer = (void*)website.c_str();
	bind[5].buffer_type = MYSQL_TYPE_STRING;
	bind[5].buffer_length = website.length();

	Database::ExecuteStmt(stmt, bind);
}

Provider::Provider(Database& database) : mysql_ptr(database.GetMysql())
{
	if (mysql_list_tables(mysql_ptr, PROVIDER_TABLE_NAME)->row_count == 0)
	{
		mysql_query(mysql_ptr, createString);
	}
}

void Provider::InsertOrUpdate(const string& id, const string& name, const string& address, const string& phone, const string& email, const string& website)
{
	if (!Exists(id))
	{
		Insert(id, name, address, phone, email, website);
	}
	else
	{
		Update(id, name, address, phone, email, website);
	}
}

bool Provider::Exists(const string& id)
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

void Provider::Delete(const string& id)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND* bind = Database::StartStmt(mysql_ptr, deleteStmtString, DELETE_PARAM_COUNT, &stmt);

	bind[0].buffer = (void*)id.c_str();
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer_length = id.length();

	Database::ExecuteStmt(stmt, bind);
}

