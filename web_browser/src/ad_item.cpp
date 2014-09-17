#include "../head/ad_item.h"

AdItem::AdItem(QListWidget* parent, QString company, double price)
	: QListWidgetItem(company, parent)
{
	myCompany = company;
	myPrice = price; 
}

QString AdItem::getCompany()
{
	return myCompany;
}

double AdItem::getPrice()
{
	return myPrice; 
}