
// MyTask.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyTaskApp: 
// �йش����ʵ�֣������ MyTask.cpp
//

class CMyTaskApp : public CWinApp
{
public:
	CMyTaskApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyTaskApp theApp;