#ifndef SET_H
#define SET_H

#include <exception>

#include <set>

using namespace std;

template<class T>
class Set : public set<T>
{
	public:
		Set() : set<T>() {}
		Set(const Set<T> & other) 
			: set<T>(other) {}
		template<class InputIterator>
		Set(InputIterator first, InputIterator last) 
			: set<T>(first, last) {}

		// gets the intersection of this set with other
		Set<T> setIntersection(const Set<T> & other) const
		{
			// the intersection is the elements in both this
			// and other. So filter the stuff in other by
			// what is in this.
			Set<T> ans;
			typename set<T>::iterator i;
			for (i = this->begin(); i != this->end(); ++ i)
			{
				if (other.find(*i) != other.end())
				{
					ans.insert(*i);
				}
			}
			return ans;
		}

		// gets the union of this set with other
		Set<T> setUnion(const Set<T> & other) const
		{
			// the union is the elements in either this
			// or other. So copy other, and then add whatever
			// isn't in this to that copy.
			Set<T> ans(other);
			typename set<T>::iterator i;
			for (i = this->begin(); i != this->end(); ++ i)
			{
				ans.insert(*i);
			}
			return ans;
		}
};

#endif
