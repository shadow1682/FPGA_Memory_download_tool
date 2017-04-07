// FPGAFLASH.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FPGA_Memory_Set.h"
#include "FPGAFLASH.h"
#include "Transformers.h"
#include "USB_Driver.h"

DWORD WINAPI ThreadFun_FlashWrite(LPVOID pthread);
DWORD WINAPI ThreadFun_FlashRead(LPVOID pthread);
// CFPGAFLASH �Ի���

IMPLEMENT_DYNAMIC(CFPGAFLASH, CDialog)

CFPGAFLASH::CFPGAFLASH(CWnd* pParent /*=NULL*/)
	: CDialog(CFPGAFLASH::IDD, pParent)
{

}

CFPGAFLASH::~CFPGAFLASH()
{
}

void CFPGAFLASH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFPGAFLASH, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FLASH, &CFPGAFLASH::OnBnClickedButtonFlash)
	ON_BN_CLICKED(IDC_BUTTON_FLASH_SAVEWRITE, &CFPGAFLASH::OnBnClickedButtonFlashSavewrite)
END_MESSAGE_MAP()

BOOL CFPGAFLASH::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// CFPGAFLASH ��Ϣ�������

int m_persent_flash=0;
CString persent_flash = _T(""),str_ADD_ALL_FLASH = _T("");

#define EP1_OUT_SIZE 64
#define EP1_IN_SIZE 64
#define SEND_SIZE 1024
#define LEN_BUF_STR 128
#define ADD_LEN_FLASH 200*1024

void CFPGAFLASH::OnBnClickedButtonFlash()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	DWORD ThreadID;
	DWORD code;

	UpdateData(FALSE);

	m_persent_flash = 0;

	SetDlgItemText(IDC_STATIC_PEST_FLASH, "%0");

	Func_Enable_flash(FALSE);

	if (!GetExitCodeThread(hThread_FlashWrite, &code) || (code != STILL_ACTIVE))
	{
		hThread_FlashWrite = CreateThread(NULL, 0, ThreadFun_FlashWrite, this, 0, &ThreadID);//��������ʼ�߳�
	}
}

DWORD WINAPI ThreadFun_FlashWrite(LPVOID pArg)						//�߳���ں���
{
	CFPGAFLASH* cMyDlg = (CFPGAFLASH*)pArg;

	str_ADD_ALL_FLASH = _T("");

	unsigned char buffer[ADD_LEN_FLASH] = {0},trans_buffer[1024] = {0},send_buffer[SEND_SIZE] = {0},recv_buffer[1024] = {0};
	char memory[64] = {0};
	char str_buffer[1024] = {0};
	int tras_buf1 = 0,tras_buf2 = 0,tras_add = 0,tras_buf0 = 0;

	int m_2 = 0,m_1 = 0,m_all = 0;

	CString str=_T("");

	cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"���ļ�...");

	/*����Ҫ���͵��ĵ�*/

	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER|OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST,"All Files (*.*)|*.*|Text Files (*.txt)|*.txt|Hex Files(*.hex)|*.hex||");

	if (dlg.DoModal() != IDOK)
	{
		cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"�ĵ�δ��...");
		cMyDlg->Func_Enable_flash(TRUE);

		return false;
	}

	//����ļ�·��
	CString strPathName = dlg.GetPathName();
	CStdioFile file;

	//���ļ�
	if (!file.Open(strPathName, CFile::modeRead))
	{
		cMyDlg->MessageBox("�ļ���ʧ��","ERROR��",MB_ICONEXCLAMATION);
		cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"�ļ���ʧ��...");
		cMyDlg->Func_Enable_flash(TRUE);
		return false;
	}

	cMyDlg->SetDlgItemText(IDC_STATIC_PEST_FLASH,"%0");

	m_persent_flash = 0;

	//cMyDlg->Invalidate();
	/*����FLASH����*/

	memory[0] = 0x0F;

	USBBulkWriteData(0,EP1_OUT,memory,EP1_OUT_SIZE,500);

	USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,500);

	if(recv_buffer[3] != 0x10) 
	{
		cMyDlg->MessageBox("��FLASH����ʧ��!���FPGA��λ�������ϵ磡","ERROR��", MB_ICONEXCLAMATION);
		cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"����FLASHʧ��...");
		cMyDlg->Func_Enable_flash(TRUE);
		return false;
	}
	memset(recv_buffer,0,1024);

	CString strText = _T(""),str_over;

	//��ȡHEX�ļ������ݵ�BYTE������
	while (file.ReadString(strText))
	{
		if(strText[8] != '0')
		{
			if(strText[8] == '1') str_over = strText+'\r'+'\n';
			if(strText[8] == '4')
			{
				if(strText[12] == '0') tras_buf0 = 0;
				else if(strText[12] == '1') tras_buf0 = 1;
				else if(strText[12] == '2') tras_buf0 = 2;
				else if(strText[12] == '3') tras_buf0 = 3;
				else continue;
			}
			continue;//���ݴ����У�����
		}

		strText.Remove(':');

		transCStringbuffer(strText,trans_buffer);

		tras_buf1 = trans_buffer[1];
		tras_buf2 = trans_buffer[2];
		tras_add = (tras_buf0<<16) | (tras_buf1<<8)|tras_buf2;

		for(m_1 = 0;m_1<trans_buffer[0];m_1++)//�����ݴ���buffer��
		{
			buffer[tras_add+m_1] = trans_buffer[m_1+4];
		}
	}
	file.Close();

	/*����ת��*/

	CString str_data = _T(""),str_num = _T(""),str_check = _T(""),str_ADD = _T("");

	WORD wordnum = 0x0000;

	str_ADD_ALL_FLASH += ":020000040000FA\r\n";

	for(int h = 0;h<ADD_LEN_FLASH;h+=16)
	{
		unsigned char trs_buffer[16] = {0},trs_num[2] = {0},trs_len = 0x10;

		//��ַ����
		str_num.Format("%04X",wordnum);//ת����ַΪCString����

		wordnum+=16;//ÿ��10h������

		switch(h)
		{
		case 65536:
			str_ADD_ALL_FLASH += ":020000040001F9\r\n";
			break;
		case 131072:
			str_ADD_ALL_FLASH += ":020000040002F8\r\n";
			break;
		case 196608:
			str_ADD_ALL_FLASH += ":020000040003F7\r\n";
			break;
		}

		//���ݼ���
		memcpy(&trs_buffer[0],&buffer[h],16);
		str_data = transResult(trs_buffer,16);

		str_data.Remove(' ');

		str_data = ":10"+str_num+"00"+str_data;

		//У��ͼ���

		transCStringbuffer(str_num,trs_num);

		UCHAR uccheck[1] = {0};

		for(int icheck = 0;icheck<15;icheck++)
			trs_buffer[icheck+1] += trs_buffer[icheck];

		uccheck[0] = 0x01+~(trs_buffer[15]+0x10+trs_num[0]+trs_num[1]);

		str_check = transResult(uccheck,1);

		str_check.Remove(' ');

		//��������ƴ��

		str_ADD = str_data+str_check+"\r\n";

		//��������

		str_ADD_ALL_FLASH += str_ADD;

	}

	str_ADD_ALL_FLASH += str_over;

	/*��������*/

	//���ַ���ת��Ϊchar�Ա�ͨ��USB����

	static unsigned char buffer_all[576090] = {0};

	strcpy((char*)buffer_all,str_ADD_ALL_FLASH);

	while( m_all<(12800*45+13+4*17))
	{
		if(m_all<17 || (m_all>=4096*45+17 && m_all<4096*45+2*17) || (m_all>=2*4096*45+2*17 && m_all<2*4096*45+3*17) || (m_all>=3*4096*45+3*17 && m_all<3*4096*45+4*17)) //���͵�ַת����
		{
			memcpy(send_buffer,&buffer_all[m_all],17);

			USBBulkWriteData(0,EP1_OUT,(char*)send_buffer,64,500);

			USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,500);

			if(recv_buffer[3] != 0x10)
			{
				cMyDlg->MessageBox("дFLASHʧ�ܣ������豸������λ�����������ϵ�!","ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable_flash(TRUE);
				return false;
			}

			m_all += 17;
			memset(send_buffer,0,1024);
			memset(recv_buffer,0,1024);
			continue;
		}
		else if(m_all>=3*4096*45+4*17 && m_all<3*4096*45+4*17+512*45)//������������
		{
			for(int m_3 = 0;m_3<22;m_3++)
			{
				for(m_2 = 0;m_2<SEND_SIZE;m_2++)//1024������һ��
				{
					send_buffer[m_2] = buffer_all[m_all];
					m_all++;
				}
				USBBulkWriteData(0,EP1_OUT,(char*)send_buffer,1024,500);

				USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,500);

				if(recv_buffer[3] != 0x10)
				{
					cMyDlg->MessageBox("дFLASHʧ�ܣ������豸������λ�����������ϵ�","ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->Func_Enable_flash(TRUE);
					return false;
				}
				memset(send_buffer,0,1024);
				memset(recv_buffer,0,1024);
			}

			memcpy(send_buffer,&buffer_all[m_all],512);

			USBBulkWriteData(0,EP1_OUT,(char*)send_buffer,512,500);

			USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,500);

			if(recv_buffer[3] != 0x10)
			{
				cMyDlg->MessageBox("дFLASHʧ�ܣ������豸������λ�����������ϵ�","ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable_flash(TRUE);
				return false;
			}
			m_all+=512;
			memset(send_buffer,0,1024);
			memset(recv_buffer,0,1024);
			continue;

		}
		else if(m_all>=3*4096*45+4*17+512*45 && m_all<3*4096*45+4*17+512*45+13)//���ͽ�����
		{
			memcpy(send_buffer,&buffer_all[m_all],13);

			USBBulkWriteData(0,EP1_OUT,(char*)send_buffer,64,500);

			memset(send_buffer,0,1024);
			memset(recv_buffer,0,1024);
			m_all += 13;
			break;
		}
		else /*�������ݵ�ַ*/
		{
			for(m_2 = 0;m_2<SEND_SIZE;m_2++)//1024������һ��
			{
				send_buffer[m_2] = buffer_all[m_all];
				m_all++;
			}

			USBBulkWriteData(0,EP1_OUT,(char*)send_buffer,1024,500);

			USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,500);

			if(recv_buffer[3] != 0x10)
			{
				cMyDlg->MessageBox("дFLASHʧ�ܣ������豸������λ�����������ϵ�","ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable_flash(TRUE);
				return false;
			}
			memset(send_buffer,0,1024);
			memset(recv_buffer,0,1024);
		}
		CString strrun;
		strrun.Format("%d",m_all/1024);
		cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"дFLASH��...�ѷ���"+strrun+"K(��540K)");

		m_persent_flash++;
		persent_flash.Format("%.0f%%", 100.0*(float)(m_persent_flash-0)/(float)(540-0));//��ʾ���ȱ�
		cMyDlg->GetDlgItem(IDC_STATIC_PEST_FLASH)->SetWindowText(persent_flash);

	}

	memset(send_buffer,0,1024);
	memset(buffer_all,0,576090);
	memset(buffer,0,204800);

	USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,500);

	if(recv_buffer[3] == 0x10)
	{
		cMyDlg->MessageBox("дFLASH�ɹ���","FLASH����",MB_ICONINFORMATION);

		cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"дFLASH���...");
		memset(recv_buffer,0,1024);
	}
	else
	{
		cMyDlg->MessageBox("дFLASHʧ�ܣ������豸������λ�����������ϵ�","ERROR��", MB_ICONEXCLAMATION);
		cMyDlg->SetDlgItemText(IDC_EDIT_STATUES_FLASH,"дFLASHʧ�ܣ������豸������λ�����������ϵ�");
		cMyDlg->Func_Enable_flash(TRUE);
		memset(recv_buffer,0,1024);
	}

	cMyDlg->Func_Enable_flash(TRUE);

	return true;

}
void CFPGAFLASH::OnBnClickedButtonFlashSavewrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(str_ADD_ALL_FLASH == _T(""))
		{
			MessageBox("δ���յ�����", "ERROR��", MB_ICONEXCLAMATION);
			return;
		}

		CString strFilter,fileName,strText;
		strFilter="Text Files(*.txt)|*.txt|Hex Files(*.hex)|*.hex||";
		CFileDialog dlg(FALSE,"txt", NULL, OFN_EXPLORER|OFN_HIDEREADONLY|
			OFN_ENABLESIZING|OFN_FILEMUSTEXIST,strFilter);
		if(dlg.DoModal() == IDOK )								//��ʾ�����ļ��Ի���
		{
			fileName+=dlg.GetPathName();
			CFile savefile(fileName,CFile::modeCreate|CFile::modeWrite);		//����CFile����
			savefile.Write(str_ADD_ALL_FLASH,str_ADD_ALL_FLASH.GetLength());							//д�ļ�����|
			savefile.Close();										//�ر��ļ�
		}
}

void CFPGAFLASH::Func_Enable_flash(bool bEnable)
{
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_BUTTON_FLASH),bEnable);
}
