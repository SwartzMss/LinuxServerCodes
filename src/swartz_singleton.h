#ifndef _SINGLETON_H
#define _SINGLETON_H

template<class T>
class singleton
{
public:
	static T* Instance()
	{
		return pInstance;
	}

	static T* pInstance;
private:
	//构造函数隐藏起来
	singleton(){}
	//将拷贝构造隐藏起来
	singleton(const singleton&){}
	//将赋值构造隐藏起来
	singleton& operator=(const singleton&){}

};

template<class T> T* singleton<T>::pInstance = new T;

#endif

