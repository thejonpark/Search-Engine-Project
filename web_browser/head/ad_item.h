#ifndef AD_ITEM_H
#define AD_ITEM_H

// QString class and the super class, QListWidgetItem
#include <QString>
#include <QListWidgetItem>

class AdItem : public QListWidgetItem
{
	private:
		QString myCompany;
		double myPrice; 

	public:
		AdItem(QListWidget* parent, QString company, double price);

		QString getCompany();
		double getPrice(); 
};

#endif