#ifndef AD_COMP_H
#define AD_COMP_H

#include <QString>
#include "bid_info.h"

struct keyWordCompEq{
	bool operator()(const QString& lhs, const QString& rhs);
};

struct companyCompGreater{
	bool operator()(const bidInfo& lhs, const bidInfo& rhs);
};

struct bidCompGreater{
	bool operator()(const bidInfo& lhs, const bidInfo& rhs);
};

#endif