
#ifndef __FREELIST_H_
#define __FREELIST_H_

#include "swartz_mutexEx.h"
#include <stack>

template<class T>
class CObjectPool
{
public:
	//  构造函数，传入列表大小，和批量分配的个数 
	CObjectPool (unsigned int max_free_ = 1024, unsigned int batch_new_ = 1024, unsigned int max_mem_ = 100000)
		: max_free (max_free_)
		, batch_new (batch_new_)
		, max_mem(max_mem_)
		, cur_mem_count(0)
	  {
		  batch_newlist = new (std::nothrow) T*[batch_new_];
	  };


	  ~CObjectPool ()
	  {
		  while(!freelist.empty ())
		  {
			  T* obj = freelist.top ();
			  freelist.pop ();
			  delete obj;
		  }
		  delete [] batch_newlist;
		  batch_newlist = NULL;
	  };


	T* Alloc () 
	{
	    CGuard<CMutex> g(lock);
		

		T* obj = NULL;
		if (!freelist.empty ()) 
		{
			obj = freelist.top ();
			freelist.pop ();
		}
		else
		{

			unsigned int j = 0;
			for (; j<batch_new; ++j) 
			{
				if (cur_mem_count>max_mem)
					break;

				++cur_mem_count;

				batch_newlist[j] = new (std::nothrow) T;

				if (!batch_newlist[j])  break;
			}
			if (j > 0) 
			{
				obj = batch_newlist[0];
				for (unsigned int i = 1; i < j; ++i) 
				{
					freelist.push (batch_newlist[i]);
				}
			}
		}

		return obj;
	};

	void Free (T* obj_) 
	{
		CGuard<CMutex> g(lock);
		if(obj_ == NULL) return;

		if(freelist.size () < max_free)
			freelist.push (obj_);
		else
		{
			delete obj_;
			--cur_mem_count;
		}
	};

    int GetCurMemCount()
    {
        return cur_mem_count;
    };

private:
	CMutex lock;

	std::stack<T* > freelist;    
	T** batch_newlist; 

	const unsigned int max_free;   
	const unsigned int batch_new;  
	const unsigned int max_mem;
	unsigned int cur_mem_count;
};

#endif//__FREELIST_H_

