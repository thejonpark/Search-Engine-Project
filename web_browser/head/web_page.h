#ifndef WEB_PAGE_H
#define WEB_PAGE_H

#include <QString>
#include <exception>

#include <iostream>

#include "../lib/set.h"
#include <map>

using namespace std;

struct file_error : public exception
{
	virtual const char* what() const throw()
	{
		return "File could not be opened.";
	}
};

class WebPage
{
	public:
		WebPage ();    // constructor that creates an empty page

		WebPage (QString filename);
		/* constructor that initializes the page from the given file.
		   Should throw an exception if the file does not exist
		   or has other problems. */

		void parse(const map<QString, WebPage*>& pages);

		~WebPage ();   // destructor

		const Set<WebPage*>& allOutgoingLinks() const;
		/* Returns a list of all outgoing links (in order of appearance) */

		const Set<WebPage*>& allIncomingLinks() const;
		/* Returns a list of all incoming links (int the order of the main data file) */

		double getPageRank() const;
		/* Returns the ranking of the WebPage */
		void setPageRank(double & a);
		/* Sets the ranking of the WebPage */

		QString filename() const;
		/* Returns the filename (including path) of this WebPage */

		const Set<QString>& allWords () const;
		/* Returns a Set containing all individual words on the web page. */

		QString getText() const;
		/* Returns all of the text on this page */

		friend ostream & operator<< (ostream & os, const WebPage & page);
		/* Declares an operator we can use to print the web page. */

		friend bool operator== (const WebPage &, const WebPage &);
		/* Do they both have the same path? */
		
		friend bool operator<  (const WebPage &, const WebPage &);
		/* compares the filenames; makes the Comparator class "less" work for STL */

	private:
		// the full path of the file
		QString myName;
		// the text (accounting for links)
		QString myText;
		// the set of words (accounting for links)
		Set<QString> myWords;
		// the list of outgoing links (in order of appearance)
		Set<WebPage*> myOutgoingLinks;
		// the list of incoming links (in the order of the main data file)
		Set<WebPage*> myIncomingLinks;
		// the page rank (changes based on search criteria)
		double myPageRank;

		// add a word from the given input stream
		void addWord(char curr, istream& in);
};

#endif
