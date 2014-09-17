// for reading the files
#include <iostream>
#include <fstream>

// for holding file names
#include <string>
#include <set>

// for exceptions regarding files
#include <exception>

using namespace std;

struct file_error : public exception
{
    virtual const char* what() const throw()
    {
        return "File could not be opened.";
    }
};

void readFile(string fileToRead, ofstream& fileToWrite, set<string>& previouslyDone)
{
    // open the file and check for errors
    ifstream in(fileToRead.c_str());
    if (in.fail())
    {
        throw file_error();
    }

    // write to the output file (and add it to the list of read files)
    previouslyDone.insert(fileToRead);
    fileToWrite << fileToRead << endl;

    // follow all of the links
    char* buffer = new char[10001];
    while (in)
    {
        // move until a link appears
        in.getline(buffer, 10001, '[');

        // move past the anchor text
        in.getline(buffer, 10001, ']');

        // move past the '('
        in.get(*buffer);

        // check to see if we are done
        if (in.fail())
        {
            return;
        }

        // get the link
        in.getline(buffer, 10001, ')');
        string link = buffer;

        // follow the link
        set<string>::iterator i = previouslyDone.find(link);
        if (i == previouslyDone.end())
        {
            readFile(link, fileToWrite, previouslyDone);
        }
    }
    delete [] buffer;
}

int main(int argc, char* argv[])
{
    // check for usage issues
    if (argc < 3)
    {
        cout << "Usage: ./web_crawler seed_file index_file" << endl;
        return 0;
    }

    // open the input and output files
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    set<string> previouslyDone;

    // check for errors
    if (in.fail() || out.fail())
    {
        cout << "ERROR: Bad argument files" << endl;
        return 1;
    }

    // go through the seed file to get (and write) all pages
    char* buffer = new char[10001];
    while (in.getline(buffer, 10001))
    {
        readFile(string(buffer), out, previouslyDone);
    }

    return 0;
}
