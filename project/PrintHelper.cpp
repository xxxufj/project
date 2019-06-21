#include "PrintHelper.h"
#include <iostream>

using namespace std;

void PrintHelper::PrintBookHeader()
{
	cout << "\n索书号\t书名\t作者\t出版社\t年份\t库存\t售价" << endl;
}

void PrintHelper::PrintBookInfo(const BookTuple& book)
{
	cout << book.bid << '\t' << book.bname << '\t' << book.bauthor << '\t' << book.bpublisher
		<< '\t' << book.byear << '\t' << book.blibcount << '\t' << book.bsellprice << endl;
}

void PrintHelper::PrintQuotationHeader()
{
	cout << "\n索书号\t书名\t供应商编号\t供应商\t批发价\t最少批发量\t日期" << endl;
}

void PrintHelper::PrintQuotationInfo(const QuotationTuple& quotation)
{
	cout << quotation.bid << '\t' << quotation.bname << '\t' << quotation.pid << '\t' << quotation.pname
		<< '\t' << quotation.qprice << '\t' << quotation.qminnumber << '\t' << quotation.qtime << endl;
}

void PrintHelper::PrintImportSummaryHeader()
{
	cout << "\n进货流水号\t负责员工工号\t进货总额\t进货时间\t备注" << endl;
}

void PrintHelper::PrintImportDetailHeader()
{
	cout << "\n进货流水号\t索书号\t供货商编号\t数量\t总金额" << endl;
}

void PrintHelper::PrintReturnSummaryHeader()
{
	cout << "\n退货流水号\t备注" << endl;
}

void PrintHelper::PrintReturnDetailHeader() 
{
	cout << "\n退货流水号\t索书号\t数量\t总金额\t退货原因" << endl;
}


void PrintHelper::PrintImportSummaryInfo(const ImportSummaryTuple& item)
{
	cout << item.iid << '\t' << item.wid << '\t' << item.totalAmount << '\t' << item.date
		<< '\t' << item.remark << endl;
}

void PrintHelper::PrintImportDetailInfo(const ImportDetailTuple& item)
{
	cout << item.iid << '\t' << item.bid << '\t' << item.pid << '\t' << item.count
		<< '\t' << item.amount << endl;
}

void PrintHelper::PrintBookInfo(const Book& bookTable, const string& id)
{
	PrintBookHeader();

	BookTuple book = bookTable.GetInfo(id);
	PrintBookInfo(book);
}

void PrintHelper::PrintReturnSummaryInfo(const ReturnSummaryTuple& item)
{
	cout << item.rid << '\t' << item.remark << endl;
}

void PrintHelper::PrintReturnDetailInfo(const ReturnDetailTuple& item)
{
	cout << item.rid << '\t' << item.bid << '\t' << item.bcount << '\t' << item.totalAmount
		<< '\t' << item.reason << endl;
}

void PrintHelper::PrintBookStock(const Book& bookTable)
{
	PrintBookHeader();

	my_ulonglong size;
	BookTuple* books = bookTable.GetStock(&size);
	for (my_ulonglong i = 0; i < size; i++)
	{
		PrintBookInfo(books[i]);
	}
	delete[] books;
}

void PrintHelper::PrintQuotationInfo(const string& bookId, const Quotation& quotationTable)
{
	PrintQuotationHeader();

	my_ulonglong size;
	QuotationTuple* items = quotationTable.GetInfo(bookId, &size);

	for (my_ulonglong i = 0; i < size; i++)
	{
		PrintQuotationInfo(items[i]);
	}

	delete[] items;
}

void PrintHelper::PrintImportDetailByBookId(const ImportForm& importTable, const string& bid)
{
	PrintImportDetailHeader();

	my_ulonglong size;
	ImportDetailTuple* records = importTable.queryDetailByBookId(bid, &size);
	for (my_ulonglong i = 0; i < size; i++)
	{
		PrintImportDetailInfo(records[i]);
	}
	delete[] records;

}

void PrintHelper::PrintReturnForm(const ReturnForm& form) {
	PrintReturnDetailHeader();
	//打印所有退货表的简单信息
	my_ulonglong size;
	ReturnSummaryTuple* records = form.queryAll();
	for (my_ulonglong i = 0; i < size; i++){
		PrintReturnSummaryInfo(records[i]);
	}
	delete[] records;
}

void PrintHelper::PrintReturnDetailsBySerailNum(const string &serial, const ReturnForm& form) {
	PrintReturnDetailHeader();
	//打印流水号的退货信息
	my_ulonglong size;
	ReturnDetailTuple* records = form.queryDetailBySerial(serial);
	for (my_ulonglong i = 0; i < size; i++){
		PrintReturnDetailInfo(records[i]);
	}
	delete[] records;
}
