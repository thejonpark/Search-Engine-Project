#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// data structures
#include <map>
#include <vector> 
#include "../lib/set.h"
#include "../head/web_page.h"
#include "../head/bid_info.h"

// superclass and Q_OBJECT
#include <QObject>
#include <QWidget>

// layouts
#include <QVBoxLayout>
#include <QHBoxLayout>

// lists
#include <QListWidgetItem>
#include <QListWidget>

// labels and boxes
#include <QLabel>
#include <QCheckBox>

// URL/search bar
#include <QLineEdit>
#include <QCompleter>
#include <QStringList>

// web page text
#include <QTextEdit>

//ad
#include <QMessageBox>

// a window for both searching and browsing
class MainWindow : public QWidget
{
	Q_OBJECT

	public:
		// make from the parent and the web pages
		MainWindow(QWidget* parent, std::map<QString, Set<WebPage*> > pageIndex,
						std::map<QString, WebPage*> allPages, vector<bidInfo> adList, map<QString, double>& bill);

		// destroys this window
		~MainWindow();

		// opens up page
		void openPage(WebPage* page);

		// generates the list of search results
		void generatePageList();

		// generates the list of ads
		void generateAdList(); 

	public slots:
		// responds to the URL-bar text
		void respondToBarText();

		// responds to clicking a link
		void openPage(QListWidgetItem* item);

		//responds to clicking an ad 
		void openAd(QListWidgetItem* item);

	private:
		// list of every existing page
		std::map<QString, Set<WebPage*> > myPageIndex;
		std::map<QString, WebPage*> myPages;
		std::vector<bidInfo> myAds; 
		std::map<QString, double>& myBill;

		/** Search/URL bar **/
		// function to set it up
		void setUpBar();
		// layouts
		QVBoxLayout* myBarGroup;
		QHBoxLayout* myBarTypingLine;
		QHBoxLayout* myBarOrderLine;
		// AND/OR boxes
		QCheckBox* myAndBox;
		QCheckBox* myOrBox;
		// the text box the user types in
		QLineEdit* myBarEdit;
		QCompleter* myBarCompleter;
		QStringList myCompletions;
		// the check boxes for order selection
		QLabel* myOrderLabel;
		QCheckBox* myPageRankBox;
		QCheckBox* myAlphabeticalBox;

		/** Search results stuff **/
		void setUpSearchResults();
		// layout
		QVBoxLayout* mySearchResultsGroup;
		// list of resulting pages
		QListWidget* myPageList;
		//List of ads
		QListWidget* myAdList; 

		/** Web page browsing stuff **/
		// function to set it up
		void setUpBrowser();
		// layouts
		QHBoxLayout* myBrowserGroup;
		QVBoxLayout* myLinksGroup;
		// incoming links
		QLabel* myIncomingLabel;
		QListWidget* myIncomingLinks;
		// outgoing links
		QLabel* myOutgoingLabel;
		QListWidget* myOutgoingLinks;
		// actual text for the page
		QTextEdit* myWebPageText;
		// the current web page object
		WebPage* myCurrentPage;

		/** Advertisement stuff **/
		void setUpAdWindow();
		QMessageBox* myPopUp;  

		/** All stuff common to everything **/
		// layouts
		QVBoxLayout* myEverything;
		QHBoxLayout* myNonBarGroup;
};

#endif