// FPGA_Memory_Set.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFPGA_Memory_SetApp:
// �йش����ʵ�֣������ FPGA_Memory_Set.cpp
//

class CFPGA_Memory_SetApp : public CWinApp
{
public:
	CFPGA_Memory_SetApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFPGA_Memory_SetApp theApp;