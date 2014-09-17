#ifndef WEB_PAGE_COMP
#define WEB_PAGE_COMP

#include "web_page.h"

struct PageRankComp
{
	bool operator() (WebPage* a, WebPage* b);
};

struct FileNameComp
{
	bool operator() (WebPage* a, WebPage* b);
};

#endif