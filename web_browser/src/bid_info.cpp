#include "../head/bid_info.h"

using namespace std; 

bidInfo::bidInfo()
{

}

bidInfo::bidInfo(QString keyWord, double price, QString company) : 
	_keyWord(keyWord), _price(price), _company(company)
{

}

bidInfo::~bidInfo()
{

}


QString bidInfo::getKeyWord() const
{
	return _keyWord; 
}

double bidInfo::getPrice() const
{
	return _price;
}

QString bidInfo::getCompany() const
{
	return _company; 
}

