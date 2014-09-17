#include "../lib/set.h"
#include <map>
#include <exception>

#include "../head/web_page.h"
#include "../head/main_window.h"
#include "../head/bid_info.h"
#include "../lib/msort.h"

#include <QString>
#include <QApplication>

#include <iostream>
#include <fstream>

using namespace std;

// generates a map of words to webpages
map<QString, WebPage*> generateMap(char* filename, map<QString, Set<WebPage*> >& database);

//generates list of advertisers
vector<bidInfo> generateAdList(char* filename);

//intializes bill
map<QString, double> generateBill(vector<bidInfo> adList);

//used to sort bill in alphabetical order
struct AlphaStrComp {
    bool operator()(const pair<QString, double>& lhs, const pair<QString, double>& rhs) 
    {

     	return lhs.first.toStdString() < rhs.first.toStdString(); 
    }
 };

void writeBill(char* filename, map<QString, double> bill);

int main(int argc, char* argv[])
{
	// usage check
	if (argc < 4)
	{
		cout << "Must pass argument for the list of all web pages, list of bids, and output filename" << endl;
		return 0;
	}

	// make the map
	map<QString, Set<WebPage*> > database;
	map<QString, WebPage*> allPages;
	vector<bidInfo> adList; 
	map<QString, double> bill;  

	try
	{
		allPages = generateMap(argv[1], database);
	}
	catch (exception& e)
	{
		cout << "ERROR: Failed to open a webpage file" << endl;
		return 1;
	}

	//read in bidList; 
	try
	{
		adList = generateAdList(argv[2]);
	}

	catch (exception& e)
	{
		cout << "ERROR: Failed to open Bid List file" << endl; 
		return 1; 
	}

	//create a map of advertisers
	bill = generateBill(adList); 

	// run the program
	QApplication app(argc, argv);

	MainWindow* mainWindow = new MainWindow(NULL, database, allPages, adList, bill);

	int ret = app.exec();

	delete mainWindow;

	map<QString, WebPage*>::iterator i = allPages.begin();
	while (i != allPages.end())
	{
		delete i->second;
		++ i;
	}

	writeBill(argv[3], bill);

	return ret;
}

map<QString, WebPage*> generateMap(char* filename, map<QString, Set<WebPage*> >& database)
{
	// open and error check
	ifstream in(filename);
	if (in.fail())
	{
		throw file_error();
	}

	// make all of the initial pages
	map<QString, WebPage*> allPages;
	char currPageName[1001];
	while (in >> currPageName)
	{
		allPages.insert(pair<QString, WebPage*>(QString(currPageName), new WebPage(QString(currPageName))));
	}

	// Go through every page
	map<QString, WebPage*>::iterator it = allPages.begin();
	while (it != allPages.end())
	{
		// make the page
		WebPage* currPage = it->second;
		currPage->parse(allPages);

		// go through every word in the page
		Set<QString> currPageWords = currPage->allWords();
		Set<QString>::iterator i;
		for (i = currPageWords.begin(); i != currPageWords.end(); ++ i)
		{
			// add the word-page pairing
			try
			{
				database.at(*i).insert(currPage);
			}
			catch (exception&)
			{
				Set<WebPage*> s;
				s.insert(currPage);
				database.insert(pair<QString, Set<WebPage*> >(*i, s));
			}
		}
		++ it;
	}

	return allPages;
}

vector<bidInfo> generateAdList(char* filename)
{
	ifstream adFile; 
	adFile.open(filename);

	if(adFile.fail()){
		throw file_error(); 
	}

	int numBids; 
	adFile >> numBids; 
	vector<bidInfo> adList; 

	//enters bid info into bidInfo and adds the class to the vector
	for(int i = 0; i < numBids; i++){
		string tempKeyWord, tempCompany; 
		double price; 
		adFile >> tempKeyWord;
		adFile >> price; 

		//gets the rest of the line (the company) and eliminates the leading whitespace
		getline(adFile, tempCompany);
		int firstChar = tempCompany.find_first_not_of(" \t");
		tempCompany = tempCompany.substr(firstChar);

		QString keyWord = QString::fromStdString(tempKeyWord);
		QString company = QString::fromStdString(tempCompany);

		bidInfo temp(keyWord, price, company);

		adList.push_back(temp);
	}

	return adList; 
}

map<QString, double> generateBill(vector<bidInfo> adList)
{
	map<QString, double> bill; 
	vector<bidInfo>::iterator it; 
	for(it = adList.begin(); it != adList.end(); it++){
		bill.insert(pair<QString, double> (it->getCompany(), 0));
	}

	return bill; 
}

void writeBill(char* filename, map<QString, double> bill)
{
	ofstream billFile;
	billFile.open(filename);
	AlphaStrComp comp1; 
	vector<pair<QString, double> > temp; 

	for(int i = 0; i < temp.size(); i++){
		temp.push_back(temp[i]);
	}

	mergeSort(temp, comp1);

	map<QString, double>::iterator it; 

	for(it = bill.begin(); it != bill.end(); it++){
		billFile << it->first.toStdString() << endl;
		billFile << it->second << endl; 
		billFile << endl;  
	}

}
