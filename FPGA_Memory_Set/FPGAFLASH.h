#pragma once


// CFPGAFLASH 对话框

class CFPGAFLASH : public CDialog
{
	DECLARE_DYNAMIC(CFPGAFLASH)

public:
	CFPGAFLASH(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFPGAFLASH();

// 对话框数据
	enum { IDD = IDD_DIALOG_FLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	HANDLE hThread_FlashWrite;
	HANDLE hThread_FlashRead;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFlash();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFlashSavewrite();
	void Func_Enable_flash(bool bEnable);
};
