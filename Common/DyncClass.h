#ifndef __DYNAMIC_CLASS_H__
#define __DYNAMIC_CLASS_H__
//////////////////////////////////////////////////////////////////////////

/*
	C++不是动态语言，所以没法从语言机制上实现类的动态创建，
但这样的需求却有可能存在，一个类似的例子便是MFC中CWnd类的Create方法，其第
一个参数为Window Class的名字，这就允许用户通过class的名字来创建相应的窗口。
要想实现这一点，必须有一个“管理中心”，用于登记类的名字，并且通过名字能够
调用对应的创建方法来创建相应的实例。结合类工厂的设计思想，这里我们让一套继
承体系中的基类作为“管理中心”，由它来维护所有派生类的必要信息，包括类名和
工厂函数，这二者必须建立起映射关系，map是不错的选择。定义了一个派生类后,它
就自动向基类进行注册

实现原理:
    基类里面用map管理所有需要动态创建 【类名-创建对象的函数指针】
    基类声明中需要添加 DECLEAR_DYNCRT_BASE 宏, cpp实现中要添加 IMPLEMENT_DYNCRT_BASE 宏    
    派生类声明中需要添加 DECLEAR_DYNCRT_CLASS 宏，cpp实现中要添加 IMPLEMENT_DYNCRT_CLASS 宏
    派生类宏中创建了一个全局对象向基类map中注册【类名-创建对象的函数指针】
*/

#include <string>
#include <map>
using namespace std;

// 声明具有动态创建的基类
#define DECLEAR_DYNCRT_BASE(CBase) \
public: \
	typedef CBase *(*Generator)(); /* 声明函数指针*/ \
	static CBase *Create(const string &class_name) /* 工厂函数 */ \
	{ \
		CMapClass::iterator iter = m_mapClass.find(class_name); \
		if (m_mapClass.end() != iter) \
		{ \
			return ((*iter).second)(); \
		} \
		return NULL; \
	} \
protected: \
	static void Register(const string &class_name, Generator class_gen) /* 注册函数 */ \
	{ \
		m_mapClass.insert(map<string, Generator>::value_type(class_name, class_gen)); \
	} \
	typedef std::map<string, Generator> CMapClass; \
	static CMapClass m_mapClass; /* 存储子类信息 */

// 用于实现基类
#define IMPLEMENT_DYNCRT_BASE(CBase)  \
	std::map<string, CBase::Generator> CBase::m_mapClass;


// 用于声明一个能够被动态创建的类(用一个全局对象进行注册)
#define DECLEAR_DYNCRT_CLASS(CDerived, CBase) \
public: \
	struct CDerived##Register /* 辅助类，用于注册 */ \
	{ \
		CDerived##Register() \
		{ \
			static bool bRegistered = false; /* 注册子类，虽然map能保证唯一，但仍只注册一次 */ \
			if(!bRegistered) \
			{ \
				CBase::Register(#CDerived, CDerived::Create); /* 注册子类信息 */ \
				bRegistered = true; \
			} \
		} \
	} ; \
	static CBase *Create() /* 工厂函数 */ \
	{ \
		return new CDerived; \
	} \
	static struct CDerived##Register m_t##CDerived##Register;

// 用于实现一个能被动态创建的类
#define IMPLEMENT_DYNCRT_CLASS(CDerived) \
	static CDerived::CDerived##Register m_t##CDerived##Register;

//////////////////////////////////////////////////////////////////////////
#endif // !defined(__DYNAMIC_CLASS_H__)
