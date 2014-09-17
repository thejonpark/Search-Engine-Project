#include "../head/web_page_item.h"

WebPageItem::WebPageItem(QListWidget* parent, WebPage* page)
	: QListWidgetItem(page->filename(), parent)
{
	myPage = page;
}

WebPage* WebPageItem::getPage()
{
	return myPage;
}