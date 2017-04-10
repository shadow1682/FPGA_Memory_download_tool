// FPGA_Memory_SetDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CFPGA_Memory_SetDlg 对话框
class CFPGA_Memory_SetDlg : public CDialog
{
// 构造
public:
	CFPGA_Memory_SetDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FPGA_MEMORY_SET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:

	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	HANDLE hThread1;
	HANDLE hThread2;

	bool check_PROM,check_TROM,check_ATMC;

	afx_msg void OnBnClickedButtonRam();
	int m_Blen;
	afx_msg void OnBnClickedRadioProm();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedRadioAtmc();
	afx_msg void OnBnClickedRadioTrom();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonSaver();

	CButton m_check_enc;
	afx_msg void OnBnClickedButtonSaveEnc();
	CComboBox m_combox_form;
	CRichEditCtrl m_ctrl_fpgasend;
	CRichEditCtrl m_ctrl_fpgalog;
	afx_msg void OnBnClickedButtonClrSendedit();
	CStatic m_ctrl_static_fpga_send;
	void Func_Enable(bool bEnable);
	BOOL FileOpen(int iFileType, int iTrasAdd, CString fileTitle);
	CComboBox m_combox_ram_selenc;
	afx_msg void OnCbnSelchangeComboForm();
	CButton m_ctrl_check_RamEncEn;
	CButton m_ctrl_ram_check_prom;
	CButton m_ctrl_ram_check_alglib;
	CButton m_ctrl_ram_check_trom;
	CButton m_ctrl_ram_check_4K;
	afx_msg void OnBnClickedButtonRamOpenfile();
	afx_msg void OnBnClickedCheckRamProm();
	afx_msg void OnBnClickedCheckRamAlglib();
};
