// HYTEST_INIT.cpp : 实现文件
//

#include "stdafx.h"
#include "FPGA_Memory_Set.h"
#include "HYTEST_INIT.h"

// CHYTEST_INIT 对话框

IMPLEMENT_DYNAMIC(CHYTEST_INIT, CDialog)

CHYTEST_INIT::CHYTEST_INIT(CWnd* pParent /*=NULL*/)
	: CDialog(CHYTEST_INIT::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CHYTEST_INIT::~CHYTEST_INIT()
{
}

void CHYTEST_INIT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_INIT, m_tab_init);
}


BEGIN_MESSAGE_MAP(CHYTEST_INIT, CDialog)
	ON_MESSAGE(WM_DEVICECHANGE,OnDeviceChange)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_INIT, &CHYTEST_INIT::OnTcnSelchangeTabInit)
END_MESSAGE_MAP()


// CHYTEST_INIT 消息处理程序

#define FPGA_MEMSET_VER_DEVOPEN "FPGA Memory Set Tool V6.3.2*Device Connect"
#define FPGA_MEMSET_VER_DEVCLOSE "FPGA Memory Set Tool V6.3.2*Device Disconnect"

static const GUID GUID_DEVINTERFACE_LIST[] = 
{
	// GUID_DEVINTERFACE_USB_DEVICE
	{ 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

	// GUID_DEVINTERFACE_DISK
	//{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

	// GUID_DEVINTERFACE_HID, 
	//{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },

	// GUID_NDIS_LAN_CLASS
	//{ 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }

};

BOOL CHYTEST_INIT::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	HICON m_hicon;
	m_hicon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hicon,TRUE);


	m_tab_init.InsertItem(0,_T("RAM"));
	//m_tab_init.InsertItem(1,_T("FLASH"));

	CRect rect,rt;

	m_tab_init.GetWindowRect(rect);
	ScreenToClient(rect);
	m_tab_init.GetItemRect(0,rt);
	rect.top += rt.Height()+10;
	rect.DeflateRect(2,2);

	//m_tab_init.SetItemSize(CSize(4,30));
	//m_tab_init.SetMinTabWidth(300);


	//m_dlg_flash.Create(IDD_DIALOG_FLASH,this);
	m_dlg_fpga_memoey.Create(IDD_FPGA_MEMORY_SET_DIALOG,this);
	m_dlg_fpga_memoey.MoveWindow(rect);
	//m_dlg_flash.MoveWindow(rect);

	m_dlg_fpga_memoey.ShowWindow(SW_SHOW);
	m_dlg_fpga_memoey.SetFocus();

	/*对USB进行注册，为以后插拔消息做准备*/

	if(USBScanDev(1) == 1 && USBOpenDev(0) == 0)
	{
		this->SetWindowText(FPGA_MEMSET_VER_DEVOPEN);
		m_dlg_fpga_memoey.Func_Enable(TRUE);
	}
	else 
	{
		this->SetWindowText(FPGA_MEMSET_VER_DEVCLOSE);
		Func_Enable(FALSE);
	}

	//注册插拔事件
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for(int i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) 
	{
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if( !hDevNotify ) 
		{
			return FALSE;
		}
	}

	/************************************/


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CHYTEST_INIT::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case DBT_DEVICEARRIVAL:
		{
			if(USBScanDev(1) == 1 && USBOpenDev(0) == 0)
			{
				m_dlg_fpga_memoey.Func_Enable(TRUE);
				this->SetWindowText(FPGA_MEMSET_VER_DEVOPEN);
				break;
			}
		}
	case DBT_DEVICEREMOVECOMPLETE:
		{
			if(USBScanDev(1) == 0 || USBOpenDev(0) == 1)
			{
				DWORD code;
				if(GetExitCodeThread(m_dlg_fpga_memoey.hThread1,&code) || code == STILL_ACTIVE)
				{
					TerminateThread(m_dlg_fpga_memoey.hThread1,0);
					CloseHandle(m_dlg_fpga_memoey.hThread1);
				}
				m_dlg_fpga_memoey.Func_Enable(TRUE);
				USBCloseDev(0);
				Func_Enable(FALSE);
				this->SetWindowText(FPGA_MEMSET_VER_DEVCLOSE);
				break;
			}
		}
	}
	return LRESULT();
}

void CHYTEST_INIT::OnTcnSelchangeTabInit(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_tab_init.GetCurSel();
	CWnd* ps[] = {&m_dlg_fpga_memoey,&m_dlg_flash};
	int i = 0;
	while(i<sizeof(ps)/sizeof(ps[0]))
	{
		ps[i]->ShowWindow(i==nSel?SW_SHOW:SW_HIDE);
		ps[i]->UpdateData();
		i++;
	}
	ps[nSel]->SetFocus();
	*pResult = 0;
}


void CHYTEST_INIT::Func_Enable(bool bEnable)
{
	::EnableWindow(::GetDlgItem(m_dlg_fpga_memoey.GetSafeHwnd(),IDC_BUTTON_RAM),bEnable);
	::EnableWindow(::GetDlgItem(m_dlg_fpga_memoey.GetSafeHwnd(),IDC_BUTTON_READ),bEnable);
}
