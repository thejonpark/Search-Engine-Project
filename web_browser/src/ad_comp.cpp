#include "../head/ad_comp.h"


bool keyWordCompEq::operator()(const QString& lhs, const QString& rhs)
{
	QString leftKeyWord = lhs; 
	QString rightKeyWord = rhs; 

	leftKeyWord = leftKeyWord.toLower();
	rightKeyWord = rightKeyWord.toLower();

	return leftKeyWord == rightKeyWord;
}

bool companyCompGreater::operator()(const bidInfo& lhs, const bidInfo& rhs)
{
	return lhs.getCompany() > rhs.getCompany();
}

bool bidCompGreater::operator()(const bidInfo& lhs, const bidInfo& rhs)
{
	return lhs.getPrice() > rhs.getPrice();
}
