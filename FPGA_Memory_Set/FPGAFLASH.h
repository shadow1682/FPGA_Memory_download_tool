#pragma once


// CFPGAFLASH �Ի���

class CFPGAFLASH : public CDialog
{
	DECLARE_DYNAMIC(CFPGAFLASH)

public:
	CFPGAFLASH(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFPGAFLASH();

// �Ի�������
	enum { IDD = IDD_DIALOG_FLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	HANDLE hThread_FlashWrite;
	HANDLE hThread_FlashRead;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFlash();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFlashSavewrite();
	void Func_Enable_flash(bool bEnable);
};
