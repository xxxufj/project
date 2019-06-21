#pragma once

#include "Staff.h"
#include "ImportForm.h"
#include "Quotation.h"
#include "SaleForm.h"
#include "ReturnForm.h"
#include "PrintHelper.h"
#include <iostream>

enum Identity { STAFF, MANAGER, VISITOR };

class ManageSystem {
public:
	ManageSystem();
	void start();

private:
	Database database;
	Staff staff;
	Book book;
	Provider provider;
	Quotation quotation;
	SaleForm saleForm;
	ReturnForm returnForm;
	ImportForm importForm;

	//用户身份以及员工id
	Identity identity;
	string staffid;

	bool login();
	void logout();
	void printDetails();
	bool staffLogin(Staff staff);
	bool managerLogin(Staff staff);

	void mainMenu();

	void statistics() {}

	void addQuotation();
	void deleteQuotation();

	void import();
	bool importBook();
	void importInfo();
	void printImportQuery();
	void importQuery();

	void sale();

	void returnBack();
	void returnInfo();
	void insertIntoReturn();
	void deleteFromReturn();
};