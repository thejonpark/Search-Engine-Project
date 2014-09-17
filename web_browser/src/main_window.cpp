#include "../head/main_window.h"

// debug
#include <QDebug>

// all of this is for the search engine
#include "../head/char_functions.h"
#include <string>
#include <cstring>
#include <vector>
#include "../head/page_rank.h"
#include "../head/web_page_comp.h"
#include "../lib/msort.h"

// for all of the web page lists
#include "../head/web_page_item.h"

//for all ad lists
#include "../head/ad_item.h"
#include "../head/ad_comp.h"

using namespace std;

// this is also for the search engine
Set<WebPage*> intersect(Set<WebPage*> a, Set<WebPage*> b)
{
	return a.setIntersection(b);
}
Set<WebPage*> unify(Set<WebPage*> a, Set<WebPage*> b)
{
	return a.setUnion(b);
}

MainWindow::MainWindow(QWidget* parent, map<QString, Set<WebPage*> > pageIndex,
							map<QString, WebPage*> allPages, vector<bidInfo> adList, map<QString, double>& bill)
	:	QWidget(parent),
		myPageIndex(pageIndex),
		myPages(allPages),
		myAds(adList),
		myBill(bill)
{
	// set the window title
	setWindowTitle(QString("Web Browser"));

	// set up all of the individual parts
	setUpBar();
	setUpSearchResults();
	setUpBrowser();
	setUpAdWindow(); 

	// set up the main layout
	myEverything = new QVBoxLayout(NULL);
	myEverything->addLayout(myBarGroup);
	myNonBarGroup = new QHBoxLayout(NULL);
	myNonBarGroup->addLayout(mySearchResultsGroup);
	myPageList->setVisible(true);
	myAdList->setVisible(true);
	myNonBarGroup->addLayout(myBrowserGroup);
	myEverything->addLayout(myNonBarGroup);

	// set the layout for this window
	setLayout(myEverything);

	// show the window
	show();
}

void MainWindow::setUpBar()
{
	// make the layouts
	myBarGroup = new QVBoxLayout(NULL);
	myBarTypingLine = new QHBoxLayout(NULL);
	myBarOrderLine = new QHBoxLayout(NULL);

	/** Bar line setup **/
	// set up the AND/OR boxes
	myAndBox = new QCheckBox(QString("AND"), this);
	myAndBox->setTristate(false);
	myAndBox->setCheckState(Qt::Checked);
	myOrBox  = new QCheckBox(QString("OR") , this);
	myOrBox->setTristate(false);

	// bar edit setup
	for (auto pagePair : myPages)
	{
		myCompletions << pagePair.second->filename();
	}
	myBarEdit = new QLineEdit(this);
	myBarCompleter = new QCompleter(myCompletions);
	myBarCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	myBarEdit->setCompleter(myBarCompleter);

	// add these widgets to the bar line
	myBarTypingLine->addWidget(myAndBox);
	myBarTypingLine->addWidget(myOrBox);
	myBarTypingLine->addWidget(myBarEdit);

	// connect the interactions of the search bar
	connect(myAndBox, SIGNAL(clicked()), myOrBox , SLOT(toggle()));
	connect(myOrBox , SIGNAL(clicked()), myAndBox, SLOT(toggle()));
	connect(myAndBox, SIGNAL(clicked()), this, SLOT(respondToBarText()));
	connect(myOrBox, SIGNAL(clicked()), this, SLOT(respondToBarText()));
	connect(myBarEdit, SIGNAL(editingFinished()), this, SLOT(respondToBarText()));

	/** Set up the order line **/
	// set up the boxes
	myOrderLabel = new QLabel(QString("Order:"), this);
	myPageRankBox = new QCheckBox(QString("Use PageRank"), this);
	myPageRankBox->setTristate(false);
	myPageRankBox->setCheckState(Qt::Checked);
	myAlphabeticalBox = new QCheckBox(QString("Use Alphabetical"), this);
	myAlphabeticalBox->setTristate(false);

	// add these widgets to the order bar line
	myBarOrderLine->addWidget(myOrderLabel);
	myBarOrderLine->addWidget(myPageRankBox);
	myBarOrderLine->addWidget(myAlphabeticalBox);

	// connect the interactions of the order line
	connect(myPageRankBox, SIGNAL(clicked()), myAlphabeticalBox, SLOT(toggle()));
	connect(myAlphabeticalBox, SIGNAL(clicked()), myPageRankBox, SLOT(toggle()));
	connect(myPageRankBox, SIGNAL(clicked()), this, SLOT(respondToBarText()));
	connect(myAlphabeticalBox, SIGNAL(clicked()), this, SLOT(respondToBarText()));

	/** Add the layouts to the group **/
	myBarGroup->addLayout(myBarOrderLine);
	myBarGroup->addLayout(myBarTypingLine);
}

void MainWindow::setUpSearchResults()
{
	mySearchResultsGroup = new QVBoxLayout(NULL);
	myPageList = new QListWidget(this);
	myAdList = new QListWidget(this);
	connect(myAdList, SIGNAL(itemActivated(QListWidgetItem*)), 
			this, SLOT(openAd(QListWidgetItem*)));
	connect(myPageList, SIGNAL(itemActivated(QListWidgetItem*)),
			this, SLOT(openPage(QListWidgetItem*)));
	myPageList->setVisible(false);
	myAdList->setVisible(false);
	mySearchResultsGroup->addWidget(myPageList);
	mySearchResultsGroup->addWidget(myAdList);
}

void MainWindow::setUpBrowser()
{
	myCurrentPage = NULL;

	/** Create the layouts **/
	myBrowserGroup = new QHBoxLayout(NULL);
	myLinksGroup = new QVBoxLayout(NULL);

	/** set up all of the widgets **/
	// text box
	myWebPageText = new QTextEdit(this);
	myWebPageText->setReadOnly(true);
	myWebPageText->setAcceptRichText(false);
	myWebPageText->setVisible(false);

	// link boxes
	myIncomingLabel = new QLabel(QString("Incoming"), this);
	myIncomingLabel->setVisible(false);
	myIncomingLinks = new QListWidget(this);
	myIncomingLinks->setVisible(false);
	myOutgoingLabel = new QLabel(QString("Outgoing"), this);
	myOutgoingLabel->setVisible(false);
	myOutgoingLinks = new QListWidget(this);
	myOutgoingLinks->setVisible(false);

	// connect link boxes to opening pages
	connect(myIncomingLinks, SIGNAL(itemActivated(QListWidgetItem*)),
			this, SLOT(openPage(QListWidgetItem*)));
	connect(myOutgoingLinks, SIGNAL(itemActivated(QListWidgetItem*)),
			this, SLOT(openPage(QListWidgetItem*)));

	/** Add it all to the layouts **/
	myBrowserGroup->addWidget(myWebPageText);
	myBrowserGroup->addLayout(myLinksGroup);
	myLinksGroup->addWidget(myIncomingLabel);
	myLinksGroup->addWidget(myIncomingLinks);
	myLinksGroup->addWidget(myOutgoingLabel);
	myLinksGroup->addWidget(myOutgoingLinks);
}

void MainWindow::openPage(WebPage* page)
{
	// set the current page
	myCurrentPage = page;

	// set the text
	myWebPageText->setText(page->getText());

	// make the incoming links
	myIncomingLinks->clear();
	Set<WebPage*> ins = page->allIncomingLinks();
	Set<WebPage*>::iterator i;
	for (i = ins.begin(); i != ins.end(); ++ i)
	{
		myIncomingLinks->addItem(new WebPageItem(myIncomingLinks, *i));
	}

	// make the outgoing links
	myOutgoingLinks->clear();
	Set<WebPage*> outs = page->allOutgoingLinks();
	for (i = outs.begin(); i != outs.end(); ++ i)
	{
		myOutgoingLinks->addItem(new WebPageItem(myOutgoingLinks, *i));
	}

	// make all browser stuff visible
	myWebPageText->setVisible(true);
	myIncomingLabel->setVisible(true);
	myIncomingLinks->setVisible(true);
	myOutgoingLabel->setVisible(true);
	myOutgoingLinks->setVisible(true);

	// make all search results stuff invisible
	myPageList->setVisible(false);
	myAdList->setVisible(false);
}

void MainWindow::generatePageList()
{
	myPageList->clear();

	/** Get the pages required **/
	// get the list for the query
	string temp = myBarEdit->text().toStdString();
	char wordsToLookFor[temp.size()];
	strcpy(wordsToLookFor, temp.c_str());
	::lower(wordsToLookFor);

	// get the operation
	Set<WebPage*> (*op) (Set<WebPage*>, Set<WebPage*>);
	if (myAndBox->isChecked())
	{
		op = &intersect;
	}
	else
	{
		op = &unify;
	}

	// go through all of the words
	bool first = true;
	Set<WebPage*> answerPages;
	int i = 0;
	int len = temp.size();
	while (i < len)
	{
		if (isSpecial(wordsToLookFor[i]))
		{
			++ i;
			continue;
		}

		// cut off the word
		int endIndex = i;
		while (endIndex < len && !isSpecial(wordsToLookFor[endIndex]))
		{
			++ endIndex;
		}

		// safely get the set of pages with this word
		wordsToLookFor[endIndex] = '\0';
		Set<WebPage*> pagesWithCurrWord;
		try
		{
			pagesWithCurrWord = myPageIndex.at(QString(wordsToLookFor + i));
		}
		catch (exception&)
		{
			// do nothing
		}

		// add the incoming and outgoing links
		Set<WebPage*> toOp(pagesWithCurrWord);
		Set<WebPage*>::iterator iter = pagesWithCurrWord.begin();
		while (iter != pagesWithCurrWord.end())
		{
			toOp = toOp.setUnion((*iter)->allOutgoingLinks());
			toOp = toOp.setUnion((*iter)->allIncomingLinks());
			++ iter;
		}

		// op it to the rest
		if (first)
		{
			answerPages = toOp;
			first = false;
		}
		else
		{
			answerPages = op(answerPages, toOp);
		}

		// increment
		i = endIndex + 1;
	}

	/** Add the pages to the list **/
	// convert to a vector
	Set<WebPage*>::iterator start = answerPages.begin();
	Set<WebPage*>::iterator end = answerPages.end();
	vector<WebPage*> sorted(start, end);

	// sort the vector based on the status of the ordering boxes
	if (myPageRankBox->isChecked())
	{
		generatePageRanks(sorted);
		mergeSort(sorted, PageRankComp());
	}
	else
	{
		mergeSort(sorted, FileNameComp());
	}

	// add them
	for (size_t ind = 0; ind < sorted.size(); ++ ind)
	{
		myPageList->addItem(new WebPageItem(myPageList, sorted[ind]));
	}

	// make all search results stuff visible
	myPageList->setVisible(true);

	// make all browser stuff invisible
	myWebPageText->setVisible(false);
	myIncomingLabel->setVisible(false);
	myIncomingLinks->setVisible(false);
	myOutgoingLabel->setVisible(false);
	myOutgoingLinks->setVisible(false);
}

void MainWindow::generateAdList()
{
	myAdList->clear();

	QString temp = myBarEdit->text();
	QStringList searchWords = temp.split(" ", QString::SkipEmptyParts);
	vector <bidInfo> ads; 
	keyWordCompEq compKeyWord; 
	bidCompGreater compBid; 
	companyCompGreater compCompany; 
	
	//for each individual word of the search
	for(int i = 0; i < searchWords.size(); i++){
		//pushes all ads matching keyword into ads vector
		for(int j = 0; j < myAds.size(); j++){
			if(compKeyWord(searchWords.at(i), myAds[j].getKeyWord())){
				ads.push_back(myAds[j]);
			}
		}
	}

	//when there are multiples of the same company, erase the lesser one
	for(int i = 0; i < ads.size(); i++){
		for(int j = 0; j < ads.size(); j++){
			if((ads[i].getCompany() == ads[j].getCompany()) && (i != j)){
				if(compCompany(ads[i], ads[j])){
					ads.erase(ads.begin() + j);
				}

				else if(ads[i].getPrice() == ads[j].getPrice()){
					break; 
				}

				else{
					ads.erase(ads.begin() + i);
					i--;
				}
			}
		}
	}


	//sort by alphabetical
	mergeSort(ads, compCompany);

	//sort by bid
	mergeSort(ads, compBid);

	//adds items to Ad area
	for(int i = 0; i < ads.size(); i++){
		myAdList->addItem(new AdItem(myAdList, ads[i].getCompany(), ads[i].getPrice()));
	}

	myAdList->setVisible(true);

}

void MainWindow::respondToBarText()
{
	auto pageForBarText = myPages.find(myBarEdit->text());
	if (pageForBarText != myPages.end())
	{
		openPage(pageForBarText->second);
	}
	else
	{
		if (myBarEdit->text() != "" 
			&& !myCompletions.contains(myBarEdit->text()))
		{
			myCompletions << myBarEdit->text();
			myBarCompleter = new QCompleter(myCompletions);
			myBarEdit->setCompleter(myBarCompleter);
		}

		generatePageList();
		generateAdList(); 
	}
}

void MainWindow::openPage(QListWidgetItem* item)
{
	openPage(((WebPageItem*) item)->getPage());
}

void MainWindow::openAd(QListWidgetItem* item)
{	
	QString message("You have visited the website of ");
	QString company = ((AdItem*) item)->getCompany(); 
	message.append(company);
	myPopUp->setText(message);
	myPopUp->setVisible(true);

	myBill.find(company)->second = myBill.find(company)->second + ((AdItem*) item)->getPrice(); 
}

void MainWindow::setUpAdWindow()
{
	myPopUp = new QMessageBox(); 
	myPopUp->setVisible(false);
}

MainWindow::~MainWindow()
{

}