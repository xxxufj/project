#include "ManageSystem.h"

ManageSystem::ManageSystem()
	:database(), staff(database), book(database), provider(database), quotation(database), 
		saleForm(database), returnForm(database),importForm(database)
{
	identity = VISITOR;
	staffid = "";
}

void ManageSystem::start()
{
	while (true) {
		login();
		while(identity != VISITOR){
			while (true) {
				mainMenu();
				string req;
				cin >> req;
				switch (req[0]) {
				case '1':
					import(); break;
				case '2':
					returnBack(); break;
				case '3':
					statistics(); break;
				case '4':
					sale(); break;
				case '5':
					logout(); break;
				default:
					cout << "没有该选项，请重新输入: "; break;
				}
				if (identity == VISITOR)
					break;
			}
		}
	}
}

bool ManageSystem::login() {
	cout << "欢迎进入登录界面" << endl;
	cout << "员工通道 ----- 1" << endl
		<< "经理通道 ----- 2" << endl
		<< "离开 ----- q" << endl;

	string ch;
	cin >> ch;
	switch (ch[0]) {
	case '1':
		if (staffLogin(staff)) {
			identity = STAFF;
			return true;
		}
		else {
			cout << "登陆失败，请检查账号与密码是否正确" << endl;
		}
		break;
	case '2':
		if (managerLogin(staff)) {
			identity = MANAGER;
			return true;
		}
		else {
			cout << "登陆失败，请检查账号与密码是否正确" << endl;
		}
		break;
	case 'q':
		exit(1);
		break;
	default:
		break;
	}
	return false;
}

void ManageSystem::logout() {
	string req;
	cout << "确定要退出当前登录的账号吗? (Y/N): ";
	cin >> req;
	if (req[0] == 'y' || req[0] == 'Y') {
		identity = VISITOR;
	}
}

void ManageSystem::printDetails() {
	cout << "注册 ----- 1" << endl
		<< "登录 ----- 2" << endl
		<< "返回 ----- q" << endl;
}

bool ManageSystem::staffLogin(Staff staff) {
	printDetails();
	string ch;
	while (true) {
		cin >> ch;
		if (ch[0] == '1') {
			cout << "请依次输入账号，密码，姓名，性别，年龄，电话号码 : ";
			string sid, sex, name, phoneNumber, password;
			int age;
			cin >> sid >> password >> name >> sex >> age >> phoneNumber;
			staff.Insert(sid, sex, age, name, phoneNumber, password);
			return true;
		}
		else if (ch[0] == '2') {
			cout << "请依次输入账号与密码 : ";
			string password;
			cin >> staffid >> password;
			return(staff.identify(staffid, password));
		}
		else if (ch[0] == 'q')break;
	}
	return false;
}

bool ManageSystem::managerLogin(Staff staff) {
	printDetails();
	string ch;
	while (true) {
		cin >> ch;
		if (ch[0] == '1') {
			cout << "请依次输入账号，密码，姓名，性别，年龄，电话号码 : ";
			string sid, sex, name, phoneNumber, password;
			int age;
			cin >> sid >> password >> sex >> age >> name >> phoneNumber;
			staff.Insert(sid, sex, age, name, phoneNumber, password, true);//添加manager属性
			return true;
		}
		else if (ch[0] == '2') {
			cout << "请依次输入账号与密码 : ";
			string password;
			cin >> staffid >> password;
			return(staff.identify(staffid, password, true));//添加manager属性
		}
		else if (ch[0] == 'q')break;
	}
	return false;
}

void ManageSystem::mainMenu()
{
	cout << "欢迎进入图书销售管理系统" << endl;
	cout << "进货\t-----\t1 " << endl
		<< "退货\t-----\t2" << endl
		<< "统计\t-----\t3" << endl
		<< "销售\t-----\t4" << endl
		<< "退出账号\t----\t5" << endl;
	cout << "请输入操作序号 ： " << endl;
}

void ManageSystem::addQuotation() {
	cout << "请依次输入供应商编号，书籍ID，单价，最小购买数量 ：";
	string providerId, bookId;
	double price;
	int minNumber;
	cin >> providerId >> bookId >> price >> minNumber;
	quotation.InsertOrUpdate(providerId, bookId, price, minNumber);
}

void ManageSystem::deleteQuotation() {
	cout << "请依次输入供应商编号，书籍ID ：";
	string providerId, bookId;
	cin >> providerId >> bookId;
	quotation.Delete(providerId, bookId);
}

void ManageSystem::importInfo() {
	cout << "欢迎进入进货页面" << endl;
	cout << "添加报价 ----- 1" << endl
		<< "删除报价 ----- 2" << endl
		<< "查看书籍库存量 ----- 3" << endl
		<< "查看书籍销售情况 ----- 4" << endl
		<< "查看上一季度进货情况 ----- 5" << endl
		<< "进货 ----- 6（需要经理权限）" << endl
		<< "离开 ----- q" << endl;
	cout << "请输入操作序号 ： " << endl;
}

void ManageSystem::importQuery() {
	string ch, bookId, providerId, date;
	while (true) {
		printImportQuery();
		cin >> ch;
		switch (ch[0]) {
		case '1':
			cin >> bookId;
			//importForm.queryByBookId(bookId);
			break;
		case '2':
			cin >> providerId;
			importForm.queryByProviderId(providerId);
			break;
		case '3':
			//importForm.queryByDate();//query by date，参数待定
			break;
		case 'q':return;
		default:break;
		}
		cout << "继续查询？ y or n ：";
		cin >> ch;
		if (ch[0] == 'n')break;
	}
}

void ManageSystem::import()
{
	string ch;
	while (true) {
		importInfo();
		cin >> ch;
		switch (ch[0]) {
		case 1:addQuotation(); break;
		case 2:deleteQuotation(); break;
		//case 3:book.printStock(); break;
		//case 4: 统计;
		//case 5:importForm.printImportForm(); break;
		case 6:importBook(); break;
		case 'q':return;
		default:break;
		}
		cout << "是否离开进货页面？y or n ：";
		cin >> ch;
		if (ch[0] == 'n')break;
	}
}

void ManageSystem::printImportQuery()
{
	cout << "请输入查询方法 ：" << endl;
	cout << "通过书籍ID查询 ----- 1" << endl
		<< "通过供货商ID查询 ----- 2" << endl
		<< "通过时间查询 ----- 3" << endl
		<< "离开 ----- q" << endl;
	cout << "请输入操作序号 ： " << endl;
}

bool ManageSystem::importBook()
{
	if (identity != MANAGER) {
		cout << "权限不够：需要经理权限！";
		return false;
	}
	string importSerialNum = database.GetSerialNumber("import");
	importForm.InsertIntoSummaryForm(importSerialNum, staffid, 0, "");

	float totalAmount = 0;
	string ch, bookId, providerId, remark = "";
	int count, importPrice;
	while (true) {
		cout << "请输入想要进货的书籍索书号: ";
		cin >> bookId;
		//quotation.printQuotation(bookId);

		cout << "输入供货商ID: ";
		cin >> providerId;
		cout << "请输入本次要进的数量: ";
		cin >> count;
		importPrice = 0;//////////////////////////////
		//importPrice = ();
		importForm.InsertIntoDetailForm(importSerialNum, bookId, providerId, count, count * importPrice);

		cout << "继续本次进货？ y/n ：";
		if (ch[0] == 'n')break;
	}
	cout << "本次进货结束，是否为本次进货添加备注信息？(y/n) ";
	cin >> ch;
	if (ch[0] == 'y') {
		cout << "备注信息: ";
		cin >> remark;
	}
	//update summary;


	//进货完成
	//打印本次进货信息  querySerial();
	return true;
}

void ManageSystem::sale() {
	string saleSerialNum = database.GetSerialNumber("sale");
	saleForm.InsertIntoSummaryForm(saleSerialNum, staffid, 0, "", "");

	float totalAmount = 0;

	string ch, bookId, payMethod, remark = "";
	int count, salePrice;
	while (true) {
		cout << "请输入顾客购买书籍的索书号: ";
		cin >> bookId;

		cout << "请输入顾客要购买的数量: ";
		cin >> count;
		//salePrice = book.price();
		//saleForm.InsertIntoDetailForm(saleSerialNum, bookId, count, count*salePrice);

		cout << "本次还有书籍？ y/n ：";
		if (ch[0] == 'n')break;
	}
	cout << "顾客的支付方式是? ";
	cin >> payMethod;
	cout << "本次销售结束，是否为本次销售添加备注信息？(y/n) ";
	cin >> ch;
	if (ch[0] == 'y') {
		cout << "备注信息: ";
		cin >> remark;
	}

	//update
	
	//销售完成
	//打印本次销售信息  querySerial();
}

void ManageSystem::returnBack()
{	
	while (true) {
		returnInfo();
		string ch, serial;
		cin >> ch;
		switch (ch[0]) {
		case '1':insertIntoReturn(); break;
		case '2':deleteFromReturn(); break;
		case '3':PrintHelper::PrintReturnForm(returnForm); break;
		case '4':
				cout << "请输入退货流水号 ：";
				cin >> serial;
				PrintHelper::PrintReturnDetailsBySerailNum(serial, returnForm);
				break;
		case 'q':return;
		default:break;
		}
		cout << "是否离开进货页面？y or n ：";
		cin >> ch;
		if (ch[0] == 'n')break;
	}
}

void ManageSystem::returnInfo()
{
	cout << "欢迎进入退货页面" << endl;
	cout << "添加退货信息 ----- 1" << endl
		<< "删除退货信息 ----- 2" << endl
		<< "查看退货表 ----- 3" << endl
		<< "通过流水号查询退货信息 ----- 4" << endl
		<< "离开 ----- q" << endl;
	cout << "请输入操作序号 ： " << endl;
}

void ManageSystem::insertIntoReturn() {
	string rid = database.GetSerialNumber("import");
	returnForm.InsertIntoSummaryFrom(rid, "");
	while (true) {
		string bid, reason;
		int bcount;
		float totalAmount;
		cout << "请依次输入退货书籍名，退货数量，总金额，退货原因。输入'q'退出" << endl;
		cin >> bid;
		if (bid == "q")break;
		cin >> bcount >> totalAmount >> reason;
		returnForm.InsertIntoDetailedFrom(rid, bid, bcount, totalAmount, reason);
	}
	cout << "是否添加退货备注？ (y/n) : " << endl;
	string ch;
	cin >> ch;
	if (ch[0] == 'y') {
		string remark;
		cin >> remark;
		returnForm.updateSummaryForm(rid, remark);
	}
}

void ManageSystem::deleteFromReturn() {
	cout << "请输入要删除的退货表编号 ： " << endl;
	string rid;
	cin >> rid;
	cout << "将该退货表全部删除？ (y/n) : " << endl;
	string c;
	cin >> c;
	if (c[0] == 'y') {
		returnForm.deleteFromDetailedFrom(rid);
		returnForm.deleteFromSummaryFrom(rid);
	}
	else {
		while (true) {
			cout << "请依次输入要删除的退货流水号，索书号。输入'q'退出" << endl;
			string bid, rid;
			cin >> rid;
			if (rid == "q")break;
			cin >> bid;
			returnForm.deleteFromDetailedFrom(rid, bid);
		}
	}
}
