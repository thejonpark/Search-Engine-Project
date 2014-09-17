#ifndef WEB_PAGE_ITEM_H
#define WEB_PAGE_ITEM_H

// WebPage class
#include "web_page.h"

// QString class and the super class, QListWidgetItem
#include <QString>
#include <QListWidgetItem>

class WebPageItem : public QListWidgetItem
{
	private:
		WebPage* myPage;

	public:
		WebPageItem(QListWidget* parent, WebPage* page);

		WebPage* getPage();
};

#endif