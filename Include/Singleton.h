//
// singleton.h
//
//  Created on: 2016-05-05
//      Author: zjg
//

#ifndef SINGLETON_H_
#define SINGLETON_H_

template<class T>
class Singleton
{
public:
	static T& Instance()
	{ 
		if(NULL == m_pInstance)
		{
			m_pInstance = new T;
		}
		return *m_pInstance;
	}
	static void UnInstance()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
protected:
	Singleton(){};
private:
	static T* m_pInstance;
 };

template<class T>
T*	Singleton<T>::m_pInstance	=	NULL;

#endif // SINGLETON_H_
