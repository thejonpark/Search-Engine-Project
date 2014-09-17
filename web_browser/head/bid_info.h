#ifndef BID_INFO_H
#define BID_INFO_H

#include <QString> 
 
using namespace std; 



class bidInfo
{
	public:
		bidInfo();
		bidInfo(QString keyWord, double price, QString company); 
		~bidInfo();
		QString getKeyWord() const;
		double getPrice() const;
		QString getCompany() const; 

	private:
		QString _keyWord; 
		double _price; 
		QString _company; 
};

#endif