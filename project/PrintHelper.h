#pragma once
#include "Quotation.h"
#include "ImportForm.h"
#include "ReturnForm.h"

class PrintHelper
{
	static void PrintBookHeader();
	static void PrintBookInfo(const BookTuple& book);

	static void PrintQuotationHeader();
	static void PrintQuotationInfo(const QuotationTuple& quotation);

	static void PrintImportSummaryHeader();
	static void PrintImportDetailHeader();
	static void PrintImportSummaryInfo(const ImportSummaryTuple& book);
	static void PrintImportDetailInfo(const ImportDetailTuple& item);

	static void PrintReturnSummaryHeader();
	static void PrintReturnDetailHeader();
	static void PrintReturnSummaryInfo(const ReturnSummaryTuple&Tuple);
	static void PrintReturnDetailInfo(const ReturnDetailTuple& item);

public:
	static void PrintBookInfo(const Book& bookTable, const string& id);
	static void PrintBookStock(const Book& bookTable);

	static void PrintQuotationInfo(const string &bookId, const Quotation& quotationTable);

	static void PrintImportDetailByBookId(const ImportForm& importTable, const string& bid);

	static void PrintReturnForm(const ReturnForm&);
	static void PrintReturnDetailsBySerailNum(const string &serial, const ReturnForm& form);
};