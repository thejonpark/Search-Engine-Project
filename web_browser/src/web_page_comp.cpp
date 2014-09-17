#include "../head/web_page_comp.h"

bool PageRankComp::operator() (WebPage* a, WebPage* b)
{
	return a->getPageRank() > b->getPageRank();
}

bool FileNameComp::operator() (WebPage* a, WebPage* b)
{
	return a->filename() < b->filename();
}