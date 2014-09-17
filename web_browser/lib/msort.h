#ifndef SORT_H
#define SORT_H

#include <vector>
#include <cstddef>

// merges the two halves
template <class T, class Comparator>
void merge(std::vector<T>& array, Comparator comp, 
    size_t startInd, size_t endInd, size_t average,
    T* copy)
{
    // initial variables
    size_t i = startInd;
    size_t j = average + 1;
    size_t ind = startInd;
    
    // copy until one of them reaches the end
    while (i <= average && j <= endInd)
    {
        if (comp(array[i], array[j]))
        {
            copy[ind] = array[i];
            ++ i;
        }
        else
        {
            copy[ind] = array[j];
            ++ j;
        }
        ++ ind;
    }

    // copy the remaining stuff from the first half
    while (i <= average)
    {
        copy[ind] = array[i];
        ++ i;
        ++ ind;
    }

    // copy the remaining stuff from the second half
    while (j <= endInd)
    {
        copy[ind] = array[j];
        ++ j;
        ++ ind;
    }

    // copy it all back into array
    for (ind = startInd; ind <= endInd; ++ ind)
    {
        array[ind] = copy[ind];
    }
}

// helper method that does the sorting
template <class T, class Comparator>
void mergeSort(std::vector<T>& array, Comparator comp, 
    size_t startInd, size_t endInd, T* copy)
{
    if (startInd == endInd)
    {
        return;
    }
    else
    {
        // sort both halves
        size_t average = (startInd + endInd) / 2;
        mergeSort(array, comp, startInd, average, copy);
        mergeSort(array, comp, average + 1, endInd, copy);

        // merge
        merge(array, comp, startInd, endInd, average, copy);
    }
}

// method that the user should call
template <class T, class Comparator>
void mergeSort(std::vector<T>& array, Comparator comp)
{
    if (array.size() == 0)
    {
        return;
    }
    T* copy = new T[array.size()];
    mergeSort(array, comp, 0, array.size() - 1, copy);
    delete [] copy;
}

#endif