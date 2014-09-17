#include "page_rank.h"
#include "set.h"

#define DEBUG 0

#if DEBUG
#include <iostream>
#include "web_page_comp.h"
#include "msort.h"
#endif

Matrix makeMatrix(std::vector<WebPage*>& pages)
{
	// sets have intersection (useful)
	Set<WebPage*> setOfPages(pages.begin(), pages.end());

	// make the answer
	size_t n = pages.size();
	Matrix ans(n);

	// for every page
	std::vector<WebPage*>::iterator i, j;
	size_t row = 0, col;
	i = pages.begin();
	while (i != pages.end())
	{
		// reset col
		col = 0;

		// go through every other page
		j = pages.begin();
		Set<WebPage*> out = (*i)->allOutgoingLinks().setIntersection(setOfPages);
		while (j != pages.end())
		{
			// every page can be randomly URL'd to
			ans[col][row] = RANDOM_RESTART / n;

			// if there are no outgoing links,
			// then assume there is a link to everything
			if (out.size() == 0)
			{
				ans[col][row] += (1 - RANDOM_RESTART) / n;
			}
			// if there is a link from i to j
			else if (out.find(*j) != out.end())
			{
				// this page can also be linked to
				ans[col][row] += (1 - RANDOM_RESTART) / out.size();
			}

			// increment
			++ j;
			++ col;
		}

		// increment
		++ i;
		++ row;
	}

	return ans;
}

std::vector<double> makeVector(size_t n)
{
	// just a vector of n 1/n's
	std::vector<double> ans(n);
	double val = 1 / (double) n;
	for (size_t i = 0; i < n; ++ i)
	{
		ans[i] = val;
	}
	return ans;
}

void generatePageRanks(std::vector<WebPage*>& pages)
{
#if DEBUG
	mergeSort(pages, FileNameComp());
#endif

	// prevent divide by zero errors
	if (pages.size() == 0)
	{
		return;
	}

	// get the matrix
	Matrix m = makeMatrix(pages);
	// get the starting vector
	std::vector<double> v = makeVector(pages.size());

#if DEBUG
	// debugging output
	std::cout << "Matrix:" << std::endl;
	for (size_t x = 0; x < m.mySize; ++ x)
	{
		for (size_t y = 0; y < m.mySize; ++ y)
		{
			std::cout << m[x][y] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "Vector:" << std::endl;
	for (size_t x = 0; x < v.size(); ++ x)
	{
		std::cout << v[x] << "\t";
	}
	std::cout << std::endl;
#endif

	// iterate and calculate
	for (size_t i = 0; i < NUM_ITERATIONS; ++ i)
	{
		multiply(m, v);
	}

#if DEBUG
	std::cout << "Answer:" << std::endl;
	for (size_t x = 0; x < v.size(); ++ x)
	{
		std::cout << v[x] << "\t";
	}
	std::cout << std::endl;
#endif

	// put the values in the pages
	std::vector<WebPage*>::iterator i = pages.begin();
	for (size_t ind = 0; ind < v.size(); ++ ind)
	{
		(*i)->setPageRank(v[ind]);
		++ i;
	}
}

