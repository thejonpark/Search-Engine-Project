#ifndef PAGE_RANK_H
#define PAGE_RANK_H

#include "web_page.h"
#include "matrix_functions.h"
#include <vector>
#include <QString>
#include <cstddef>

const double RANDOM_RESTART = 0.15;
const size_t NUM_ITERATIONS = 30;

// makes the matrix of the pages (helper method)
Matrix makeMatrix(std::vector<WebPage*>& pages);

// makes the starting vector (helper method)
std::vector<double> makeVector(size_t n);

// makes the page ranks (method that should actually be called)
void generatePageRanks(std::vector<WebPage*>& pages);

#endif
