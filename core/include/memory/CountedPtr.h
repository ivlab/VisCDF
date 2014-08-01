/*
 * Copyright Regents of the University of Minnesota, 2014.  This software is released under the following license: http://opensource.org/licenses/lgpl-3.0.html.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Funding for this software development was provided in part by the NSF (IIS-1251069) and NIH (1R01EB018205-01).
 * Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
 *
 * The content is solely the responsibility of the authors and does not necessarily represent the official views of the National Institutes of Health.
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef COUNTEDPTR_H_
#define COUNTEDPTR_H_

namespace viscdfcore {

template <class T> class CountedPtr {
public:
	CountedPtr(T* p = 0)
	{
		if (p != 0)
		{
			refCounter = new counter(p);
		}
		else
		{
			refCounter = 0;
		}
	}

	CountedPtr(const CountedPtr& cp)
	{
		setPointer(cp.refCounter);
	}

	virtual ~CountedPtr()
	{
		if (refCounter != 0)
		{
			releasePointer();
		}
	}

	CountedPtr<T>& operator=(const CountedPtr& cp) {
		if (refCounter != 0)
		{
			releasePointer();
		}
		setPointer(cp.refCounter);
	    return *this;
	}

	T& operator*() const throw() {return *refCounter->pointer;}
	T* operator->() const throw() {return refCounter->pointer;}
	bool operator==(const CountedPtr<T>& rhs) {return refCounter == rhs.refCounter;}
	bool operator!=(const CountedPtr<T>& rhs) {return !(*this == rhs);}

private:
	struct counter
	{
		T* pointer;
		int count;
		counter(T* p) : pointer(p), count(1) {}
		~counter() { delete pointer; }
	}* refCounter;

	void setPointer(counter* c)
	{
		refCounter = c;
		refCounter->count++;
	}

	void releasePointer()
	{
		refCounter->count--;
		if (refCounter->count == 0)
		{
			delete refCounter;
		}
	}
};

} /* namespace viscdfcore */

#endif /* COUNTEDPTR_H_ */
