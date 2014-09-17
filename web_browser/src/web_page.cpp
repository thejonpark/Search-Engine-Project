#include "../head/web_page.h"
#include "../head/char_functions.h"
#include <QString>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void WebPage::addWord(char curr, istream& in)
{
	QString currWord("");
	currWord += lower(curr);
	myText += curr;
	while (in.get(curr) && !isSpecial(curr))
	{
		currWord += lower(curr);
		myText += curr;
	}
	myWords.insert(currWord);
	in.putback(curr);
}

WebPage::WebPage()
{
	// do nothing
}

WebPage::WebPage(QString filename)
{
	// set myName
	myName = filename;
}

void WebPage::parse(const map<QString, WebPage*>& pages)
{
	// open the file and check for errors
	ifstream in(myName.toStdString().c_str());
	if (in.fail())
	{
		throw file_error();
	}

	// get all of the words and text
	char curr;
	while (in.get(curr))
	{
		// handle words
		if (!isSpecial(curr))
		{
			addWord(curr, in);
		}
		// handle links
		else if (curr == '[')
		{
			// get the anchor text
			stringstream anchor;
			char buffer[1001];
			in.getline(buffer, 1001, ']');
			anchor << buffer;

			// get the words out of the anchor
			while (anchor.get(curr))
			{
				if (!isSpecial(curr))
				{
					addWord(curr, anchor);
				}
				else
				{
					myText += curr;
				}
			}

			// move past the '('
			in.get(curr);

			// get the link
			in.getline(buffer, 1001, ')');
			map<QString, WebPage*>::const_iterator it = pages.find(QString(buffer));
			if (it != pages.end())
			{
				myOutgoingLinks.insert(it->second);
				it->second->myIncomingLinks.insert(this);
			}
		}
		// some other character
		else
		{
			myText += curr;
		}
	}
}

WebPage::~WebPage()
{
	// don't need to do anything
}

const Set<WebPage*>& WebPage::allOutgoingLinks() const
{
	return myOutgoingLinks;
}

const Set<WebPage*>& WebPage::allIncomingLinks() const
{
	return myIncomingLinks;
}

QString WebPage::filename() const
{
	return myName;
}

const Set<QString>& WebPage::allWords() const
{
	return myWords;
}

QString WebPage::getText() const
{
	return myText;
}

ostream & operator<< (ostream & os, const WebPage & page)
{
	// open the file and error check
	QString::const_iterator i = page.myText.begin();
	while (i != page.myText.end())
	{
		os << i->toAscii();
		++ i;
	}
	return os;
}

bool operator== (const WebPage & a, const WebPage & b)
{
	return a.myName == b.myName;
}

bool operator<  (const WebPage & a, const WebPage & b)
{
	return a.myName <  b.myName;
}

double WebPage::getPageRank() const
{
	return myPageRank;
}

void WebPage::setPageRank(double & a)
{
	myPageRank = a;
}
