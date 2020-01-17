#ifndef __DYNAMIC_CLASS_H__
#define __DYNAMIC_CLASS_H__
//////////////////////////////////////////////////////////////////////////

/*
	C++���Ƕ�̬���ԣ�����û�������Ի�����ʵ����Ķ�̬������
������������ȴ�п��ܴ��ڣ�һ�����Ƶ����ӱ���MFC��CWnd���Create���������
һ������ΪWindow Class�����֣���������û�ͨ��class��������������Ӧ�Ĵ��ڡ�
Ҫ��ʵ����һ�㣬������һ�����������ġ������ڵǼ�������֣�����ͨ�������ܹ�
���ö�Ӧ�Ĵ���������������Ӧ��ʵ��������๤�������˼�룬����������һ�׼�
����ϵ�еĻ�����Ϊ���������ġ���������ά������������ı�Ҫ��Ϣ������������
��������������߱��뽨����ӳ���ϵ��map�ǲ����ѡ�񡣶�����һ���������,��
���Զ���������ע��

ʵ��ԭ��:
    ����������map����������Ҫ��̬���� ������-��������ĺ���ָ�롿
    ������������Ҫ��� DECLEAR_DYNCRT_BASE ��, cppʵ����Ҫ��� IMPLEMENT_DYNCRT_BASE ��    
    ��������������Ҫ��� DECLEAR_DYNCRT_CLASS �꣬cppʵ����Ҫ��� IMPLEMENT_DYNCRT_CLASS ��
    ��������д�����һ��ȫ�ֶ��������map��ע�᡾����-��������ĺ���ָ�롿
*/

#include <string>
#include <map>
using namespace std;

// �������ж�̬�����Ļ���
#define DECLEAR_DYNCRT_BASE(CBase) \
public: \
	typedef CBase *(*Generator)(); /* ��������ָ��*/ \
	static CBase *Create(const string &class_name) /* �������� */ \
	{ \
		CMapClass::iterator iter = m_mapClass.find(class_name); \
		if (m_mapClass.end() != iter) \
		{ \
			return ((*iter).second)(); \
		} \
		return NULL; \
	} \
protected: \
	static void Register(const string &class_name, Generator class_gen) /* ע�ắ�� */ \
	{ \
		m_mapClass.insert(map<string, Generator>::value_type(class_name, class_gen)); \
	} \
	typedef std::map<string, Generator> CMapClass; \
	static CMapClass m_mapClass; /* �洢������Ϣ */

// ����ʵ�ֻ���
#define IMPLEMENT_DYNCRT_BASE(CBase)  \
	std::map<string, CBase::Generator> CBase::m_mapClass;


// ��������һ���ܹ�����̬��������(��һ��ȫ�ֶ������ע��)
#define DECLEAR_DYNCRT_CLASS(CDerived, CBase) \
public: \
	struct CDerived##Register /* �����࣬����ע�� */ \
	{ \
		CDerived##Register() \
		{ \
			static bool bRegistered = false; /* ע�����࣬��Ȼmap�ܱ�֤Ψһ������ֻע��һ�� */ \
			if(!bRegistered) \
			{ \
				CBase::Register(#CDerived, CDerived::Create); /* ע��������Ϣ */ \
				bRegistered = true; \
			} \
		} \
	} ; \
	static CBase *Create() /* �������� */ \
	{ \
		return new CDerived; \
	} \
	static struct CDerived##Register m_t##CDerived##Register;

// ����ʵ��һ���ܱ���̬��������
#define IMPLEMENT_DYNCRT_CLASS(CDerived) \
	static CDerived::CDerived##Register m_t##CDerived##Register;

//////////////////////////////////////////////////////////////////////////
#endif // !defined(__DYNAMIC_CLASS_H__)
