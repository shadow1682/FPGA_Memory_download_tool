#pragma once
#include "afxcmn.h"
#include "FPGA_Memory_SetDlg.h"
#include "FPGAFLASH.h"
#include "dbt.h"
#include "USB_Driver.h"

// CHYTEST_INIT �Ի���

class CHYTEST_INIT : public CDialog
{
	DECLARE_DYNAMIC(CHYTEST_INIT)

public:
	CHYTEST_INIT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHYTEST_INIT();

// �Ի�������
	enum { IDD = IDD_DIALOG_INIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:

	CFPGA_Memory_SetDlg m_dlg_fpga_memoey;
	CFPGAFLASH m_dlg_flash;

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab_init;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabInit(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);
	void Func_Enable(bool bEnable);
};
