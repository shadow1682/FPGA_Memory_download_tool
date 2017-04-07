// FPGA_Memory_SetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FPGA_Memory_Set.h"
#include "FPGA_Memory_SetDlg.h"
#include "Transformers.h"
#include "USB_Driver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ThreadFun(LPVOID pthread);
DWORD WINAPI ThreadFun2(LPVOID pthread);
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFPGA_Memory_SetDlg �Ի���




CFPGA_Memory_SetDlg::CFPGA_Memory_SetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFPGA_Memory_SetDlg::IDD, pParent)
	, m_Blen(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFPGA_Memory_SetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEN, m_Blen);
	DDX_Control(pDX, IDC_COMBO_FORM, m_combox_form);
	DDX_Control(pDX, IDC_RICHEDIT_FPGASEND, m_ctrl_fpgasend);
	DDX_Control(pDX, IDC_RICHEDIT_FPGAREAD, m_ctrl_fpgaread);
	DDX_Control(pDX, IDC_STATIC_FPGA_SEND, m_ctrl_static_fpga_send);
	DDX_Control(pDX, IDC_STATIC_FPGA_READ, m_ctrl_static_fpga_read);
	DDX_Control(pDX, IDC_COMBO_RAM_SELENC, m_combox_ram_selenc);
	DDX_Control(pDX, IDC_CHECK_RAM_ENCENABLE, m_ctrl_check_RamEncEn);
	DDX_Control(pDX, IDC_RADIO_PROM, m_ctrl_radio_RamProm);
}

BEGIN_MESSAGE_MAP(CFPGA_Memory_SetDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RAM, &CFPGA_Memory_SetDlg::OnBnClickedButtonRam)
	ON_BN_CLICKED(IDC_RADIO_PROM, &CFPGA_Memory_SetDlg::OnBnClickedRadioProm)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFPGA_Memory_SetDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RADIO_ATMC, &CFPGA_Memory_SetDlg::OnBnClickedRadioAtmc)
	ON_BN_CLICKED(IDC_RADIO_TROM, &CFPGA_Memory_SetDlg::OnBnClickedRadioTrom)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CFPGA_Memory_SetDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_SAVER, &CFPGA_Memory_SetDlg::OnBnClickedButtonSaver)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ENC, &CFPGA_Memory_SetDlg::OnBnClickedButtonSaveEnc)
	ON_BN_CLICKED(IDC_BUTTON_CLR_SENDEDIT, &CFPGA_Memory_SetDlg::OnBnClickedButtonClrSendedit)
	ON_BN_CLICKED(IDC_BUTTON_CLR_READEDIT, &CFPGA_Memory_SetDlg::OnBnClickedButtonClrReadedit)
	ON_CBN_SELCHANGE(IDC_COMBO_FORM, &CFPGA_Memory_SetDlg::OnCbnSelchangeComboForm)
	ON_CBN_SELCHANGE(IDC_COMBO_RAM_SELENC, &CFPGA_Memory_SetDlg::OnCbnSelchangeComboRamSelenc)
END_MESSAGE_MAP()


// CFPGA_Memory_SetDlg ��Ϣ�������


BOOL CFPGA_Memory_SetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	m_ctrl_fpgasend.ReplaceSel(_T("\r\nhex��д����ʹ��˵����\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\n1�����Ƚ�FPGAͨ��USB������PC��\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\n2���ڡ�Settings����������Ҫд��Ŀ����ͣ�ͬʱ����ROMKEY�Լ���Ҫ��д���ļ����͡�\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\nRCF�ļ����������ܺ����ɵĸ�ʽΪ.rcf���ļ���\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\nHEX��׼��ʽ�ļ���δ�����ܵĸ�ʽΪ.hex���ļ�����������м��ܲ��������������ص�RAM�洢���С�\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\nHEX�Զ����ʽ�ļ����Ǳ�׼hex��ʽ��.hex�ļ���û�е�ַת���У�Ҳδ�����ܡ�\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\n3��������Download����ť��ѡ����Ҫд����ĵ������ؼ��ɡ�\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("\r\nע�⣺1.����ֵ�ַ���ô���ȴ�����Ϣ���뽫FPGA��ϵ������ϵ�ſɼ���������\r\n"));
	m_ctrl_fpgasend.ReplaceSel(_T("      2.Debug����Ϊ�����������ã����������ļ�����Ҫ�õ���\r\n"));

	//richedit�༭���л�����
	m_ctrl_fpgaread.SetTargetDevice(NULL,0);
	m_ctrl_fpgasend.SetTargetDevice(NULL,0);

	//richedit�༭���������
	CHARFORMAT2 cf_read;
	m_ctrl_fpgasend.GetDefaultCharFormat(cf_read);
	cf_read.dwMask = CFM_FACE;
	strcpy_s(cf_read.szFaceName,_T("����"));
	m_ctrl_fpgasend.SetDefaultCharFormat(cf_read);
	m_ctrl_fpgaread.SetDefaultCharFormat(cf_read);

	((CButton*)GetDlgItem(IDC_RADIO_PROM))->SetCheck(TRUE);//����Promѡ��
	check_PROM = TRUE;
	check_TROM = FALSE;
	check_ATMC = FALSE;

	SetDlgItemText(IDC_EDIT_ROMKEY,"4859188B");//����Ĭ��KEY

	m_combox_form.SetCurSel(1);//Hex��׼���ܸ�ʽ

	m_ctrl_check_RamEncEn.SetCheck(true);

	//GetDlgItem(IDC_RADIO_ATMC)->EnableWindow(false);

	SetDlgItemText(IDC_RADIO_PROM, "Prom:0x00000-0x27FFF(160K)");
	SetDlgItemText(IDC_RADIO_ATMC, "Alglib:0x28000-0x2FFFF(32K)");
	SetDlgItemText(IDC_RADIO_TROM, "Trom:0x40000-0x47FFF(32K)");

	m_combox_ram_selenc.AddString(_T("HY188B_201512"));
	m_combox_ram_selenc.AddString(_T("HY328Av1.0_201604"));
	m_combox_ram_selenc.AddString(_T("HY328Av2.0_201609"));
	m_combox_ram_selenc.AddString(_T("HY128Av1.0_201609"));
	m_combox_ram_selenc.AddString(_T("HY328A_SPW_201703"));
	m_combox_ram_selenc.SetCurSel(4);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFPGA_Memory_SetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFPGA_Memory_SetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFPGA_Memory_SetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int m_persent=0;
CString persent = _T(""),STR2,str_ADD_ALL,STR_ENC = _T("");

#define EP1_OUT_SIZE 64
#define EP1_IN_SIZE 64
#define SEND_SIZE 1024
#define LEN_BUF_STR 128
#define ADD_LEN_PROM_ATMC 192 * 1024
#define ADD_LEN_PROM 160*1024
#define ADD_LEN_ATMC 32*1024
#define ADD_LEN_TROM 32*1024
#define ADD_LEN 288*1024
#define ADD_LEN_FLASH 200*1024
#define ENC_LEN 32*1024
#define ENC_LEN_64K 64*1024

//��Ŀ����ģʽ
#define MODE_HY188B_201512 0
#define MODE_HY328Av1_0_201604 1
#define MODE_HY328Av2_0_201609 2
#define MODE_HY128Av1_0_201609 3
#define MODE_HY328A_SPW_201703 4

//�ļ���ʽ
#define FILE_RCF_ENC 0
#define FILE_HEX_STAND_NOENC 1
#define FILE_HEX_DEFINED_NOENC 2

unsigned int sbox(unsigned int in)
{
	if(in == 0x00) return 0x0C;
	else if(in == 0x01)return 0x05;
	else if(in == 0x02)return 0x06;
	else if(in == 0x03)return 0x0B;
	else if(in == 0x04)return 0x09;
	else if(in == 0x05)return 0x00;
	else if(in == 0x06)return 0x0A;
	else if(in == 0x07)return 0x0D;
	else if(in == 0x08)return 0x03;
	else if(in == 0x09)return 0x0E;
	else if(in == 0x0A)return 0x0F;
	else if(in == 0x0B)return 0x08;
	else if(in == 0x0C)return 0x04;
	else if(in == 0x0D)return 0x07;
	else if(in == 0x0E)return 0x01;
	else if(in == 0x0F)return 0x02;
	else return 0;
}

bool Hex_188B_Enc(unsigned char* ROMKEY_UC, int Input_len,unsigned char* Input_Data, unsigned char* Enc_Data)//188B����
{
	//���ܲ�����ʼ

	unsigned int ROMKEY = (ROMKEY_UC[0] << 24) + (ROMKEY_UC[1] << 16) + (ROMKEY_UC[2] << 8) + ROMKEY_UC[3];

	unsigned char Enc_Data_One[ENC_LEN] = {0};

	unsigned int ad_tp = 0;

	int Enc_num = 0;

	for (int block_i = 0; block_i < Input_len/32768; block_i++)
	{
		memcpy(Enc_Data_One, &Input_Data[block_i*ENC_LEN], ENC_LEN);//��ԭ���ݷ�Ϊn��32K�����򣬴��뵥����������

		unsigned int Enc_word_data[8192] = {0};

		for (unsigned int Addr = 0; Addr < 8192;Addr++)//32K�������Ϊ8192��word��4byte��
		{
			unsigned int initial_hex_int = 0,Addr_log = 0,Ext_key = 0,before_s = 0,enc_hex = 0,ad_phy =0;
			unsigned char initial_hex[4] = { 0 }, before_s_uc[32] = {0},enc_hex_uc[32] = {0},ad_tp_uc[16] = {0};

			memcpy(initial_hex, &Enc_Data_One[Addr*4], 4);//32KB����ת��Ϊ1WORDһ��

			//initial_hex����(���ݽ��з�ת)����תǰΪ:initial_hex_int = ((initial_hex[0] << 24) + (initial_hex[1] << 16) + (initial_hex[2] << 8) + initial_hex[3]);

			initial_hex_int = ((initial_hex[3] << 24) + (initial_hex[2] << 16) + (initial_hex[1] << 8) + initial_hex[0]);

			Addr_log = (Addr << 16) + Addr;//addr_log

			Ext_key = initial_hex_int ^ (ROMKEY ^ Addr_log);

			before_s = (sbox((Ext_key&0xf0000000) >> 28) << 28)+
				       (sbox((Ext_key&0x0f000000) >> 24) << 24)+
					   (sbox((Ext_key&0x00f00000) >> 20) << 20)+
					   (sbox((Ext_key&0x000f0000) >> 16) << 16)+
					   (sbox((Ext_key&0x0000f000) >> 12) << 12)+
					   (sbox((Ext_key&0x00000f00) >> 8) << 8)+
					   (sbox((Ext_key&0x000000f0) >> 4) << 4)+
					   (sbox(Ext_key&0x0000000f));

			int before_s_bit = 0x01;

			for(int before_s_i = 0;before_s_i < 32;before_s_i++)
			{

				before_s_uc[before_s_i] = ((before_s & before_s_bit) >> before_s_i);

				before_s_bit *= 2;
			}

			enc_hex_uc[31] = before_s_uc[15];enc_hex_uc[30] = before_s_uc[6];enc_hex_uc[29] = before_s_uc[19];enc_hex_uc[28] = before_s_uc[20];enc_hex_uc[27] = before_s_uc[28];
			enc_hex_uc[26] = before_s_uc[11];enc_hex_uc[25] = before_s_uc[27];enc_hex_uc[24] = before_s_uc[16];enc_hex_uc[23] = before_s_uc[0];enc_hex_uc[22] = before_s_uc[14];
			enc_hex_uc[21] = before_s_uc[22];enc_hex_uc[20] = before_s_uc[25];enc_hex_uc[19] = before_s_uc[4];enc_hex_uc[18] = before_s_uc[17];enc_hex_uc[17] = before_s_uc[30];
			enc_hex_uc[16] = before_s_uc[9];enc_hex_uc[15] = before_s_uc[1];enc_hex_uc[14] = before_s_uc[7];enc_hex_uc[13] = before_s_uc[23];enc_hex_uc[12] = before_s_uc[13];
			enc_hex_uc[11] = before_s_uc[31];enc_hex_uc[10] = before_s_uc[26];enc_hex_uc[9] = before_s_uc[2];enc_hex_uc[8] = before_s_uc[8];enc_hex_uc[7] = before_s_uc[18];
			enc_hex_uc[6] = before_s_uc[12];enc_hex_uc[5] = before_s_uc[29];enc_hex_uc[4] = before_s_uc[5];enc_hex_uc[3] = before_s_uc[21];enc_hex_uc[2] = before_s_uc[10];
			enc_hex_uc[1] = before_s_uc[3];enc_hex_uc[0] = before_s_uc[24];

			//ע�⣺enc_hex��Ҫ�ٴν��з�ת
			enc_hex = (((enc_hex_uc[31]*8+enc_hex_uc[30]*4+enc_hex_uc[29]*2+enc_hex_uc[28]) << 4) + (enc_hex_uc[27]*8+enc_hex_uc[26]*4+enc_hex_uc[25]*2+enc_hex_uc[24])) +   
					  ((((enc_hex_uc[23]*8+enc_hex_uc[22]*4+enc_hex_uc[21]*2+enc_hex_uc[20]) << 4) + (enc_hex_uc[19]*8+enc_hex_uc[18]*4+enc_hex_uc[17]*2+enc_hex_uc[16])) << 8)+
					  ((((enc_hex_uc[15]*8+enc_hex_uc[14]*4+enc_hex_uc[13]*2+enc_hex_uc[12]) << 4) + (enc_hex_uc[11]*8+enc_hex_uc[10]*4+enc_hex_uc[9]*2+enc_hex_uc[8])) << 16)+
					  ((((enc_hex_uc[7]*8+enc_hex_uc[6]*4+enc_hex_uc[5]*2+enc_hex_uc[4]) << 4) + (enc_hex_uc[3]*8+enc_hex_uc[2]*4+enc_hex_uc[1]*2+enc_hex_uc[0])) <<24);

			/*��ַ�������*/
			ad_tp = (Addr_log & 0x00001FFF) ^ ((ROMKEY & 0x00007FFC) >> 2);//Addr_log[12:0] ^ ROMKEY[14:2](ע�⣺Ҫ����2λ����2λɾ��)

			int ad_tp_bit = 0x01;

			for(int ad_tp_i = 0;ad_tp_i < 16;ad_tp_i++)
			{

				ad_tp_uc[ad_tp_i] = (ad_tp & ad_tp_bit) >> ad_tp_i;

				ad_tp_bit *= 2;
			}

			ad_phy = (ad_tp_uc[6] << 12)+
				     ((ad_tp_uc[10]*8+ad_tp_uc[8]*4+ad_tp_uc[5]*2+ad_tp_uc[12]) << 8)+
					 ((ad_tp_uc[9]*8+ad_tp_uc[7]*4+ad_tp_uc[11]*2+ad_tp_uc[2]) << 4)+
					 (ad_tp_uc[0]*8+ad_tp_uc[1]*4+ad_tp_uc[3]*2+ad_tp_uc[4]);

			Enc_word_data[ad_phy] = enc_hex;
		}

		CString Str_enc_dat = _T("");
		char Enc_dat[9] = {0};

		for(int Enc_num_1 = 0;Enc_num_1<8192;Enc_num_1++)
		{
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF000000) >> 24);Enc_num++;
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF0000) >> 16);Enc_num++;
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF00) >> 8);Enc_num++;
			Enc_Data[Enc_num] = (Enc_word_data[Enc_num_1] & 0xFF);Enc_num++;
			
			//to compare RCF,eg.(49 a4 f1 29)-->(29 f1 a4 49)  changed by ghan in 3.30.2017
			Enc_dat[6] = ((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) <= 9?((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28)+'0':(((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28)-10)+'a'; 
			Enc_dat[7] = ((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) <= 9?((Enc_word_data[Enc_num_1] & 0xF000000) >> 24)+'0':(((Enc_word_data[Enc_num_1] & 0xF000000) >> 24)-10)+'a'; 
			Enc_dat[4] = ((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) <= 9?((Enc_word_data[Enc_num_1] & 0xF00000) >> 20)+'0':(((Enc_word_data[Enc_num_1] & 0xF00000) >> 20)-10)+'a'; 
			Enc_dat[5] = ((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) <= 9?((Enc_word_data[Enc_num_1] & 0xF0000) >> 16)+'0':(((Enc_word_data[Enc_num_1] & 0xF0000) >> 16)-10)+'a'; 
			Enc_dat[2] = ((Enc_word_data[Enc_num_1] & 0xF000) >> 12) <= 9?((Enc_word_data[Enc_num_1] & 0xF000) >> 12)+'0':(((Enc_word_data[Enc_num_1] & 0xF000) >> 12)-10)+'a'; 
			Enc_dat[3] = ((Enc_word_data[Enc_num_1] & 0xF00) >> 8) <= 9?((Enc_word_data[Enc_num_1] & 0xF00) >> 8)+'0':(((Enc_word_data[Enc_num_1] & 0xF00) >> 8)-10)+'a'; 
			Enc_dat[0] = ((Enc_word_data[Enc_num_1] & 0xF0) >> 4) <= 9?((Enc_word_data[Enc_num_1] & 0xF0) >> 4)+'0':(((Enc_word_data[Enc_num_1] & 0xF0) >> 4)-10)+'a'; 
			Enc_dat[1] = (Enc_word_data[Enc_num_1] & 0x0F) <= 9?(Enc_word_data[Enc_num_1] & 0x0F)+'0':((Enc_word_data[Enc_num_1] & 0x0F)-10)+'a'; 

			Str_enc_dat.Format(_T("%s"),Enc_dat);

			Str_enc_dat += "\r\n";

			STR_ENC += Str_enc_dat;
		}

		memset(Enc_word_data,0,sizeof(Enc_word_data));
	}

	return true;
}

UINT32 sbox4(UINT32 in,UINT16 KEY)
{
	UINT32 out = 0;
	out = ((in & 0xFFFF0000) >> 16) |  //in[31:16]
		(((((in & 0x7FFF0000) << 1) | ((in & 0x80000000) >> 15)) & (((in & 0x00FF0000) << 8) | ((in & 0xFF000000) >> 8))) ^//(({in[30:16],in[31]}) & ({in[23:16],in[31:24]}))
		(((in & 0x3FFF0000) << 2) | ((in & 0xC0000000) >> 14)) ^ (KEY << 16) ^ ((in & 0x0000FFFF) << 16));
	return out;
}
bool Hex_328A_Enc(unsigned char* ROMKEY_UC, int Input_len, unsigned char* Input_Data, unsigned char* Enc_Data)//328A����
{
	//���ܲ�����ʼ

	unsigned int ROMKEY = (ROMKEY_UC[0] << 24) + (ROMKEY_UC[1] << 16) + (ROMKEY_UC[2] << 8) + ROMKEY_UC[3];

	unsigned char Enc_Data_One[ENC_LEN] = { 0 };

	unsigned int ad_tp = 0;

	int Enc_num = 0;

	for (int block_i = 0; block_i < Input_len / 32768; block_i++)
	{
		memcpy(Enc_Data_One, &Input_Data[block_i*ENC_LEN], ENC_LEN);//��ԭ���ݷ�Ϊn��32K�����򣬴��뵥����������

		unsigned int Enc_word_data[8192] = { 0 };

		for (unsigned int Addr = 0; Addr < 8192; Addr++)//32K�������Ϊ8192��word��4byte��
		{
			unsigned int initial_hex_int = 0, Addr_log = 0, ad_phy = 0;
			unsigned char initial_hex[4] = { 0 }, new_Data40_uc[32] = { 0 }, ad_tp_uc[16] = { 0 };
			unsigned char enc_ck4 = 0, enc_ck3 = 0, enc_ck2 = 0, enc_ck1 = 0;
			UINT32 new_Data40 = 0, enc_hex = 0;
			UINT64 key64 = 0, key32 = 0;
			UINT16 key16 = 0;
			
			memcpy(initial_hex, &Enc_Data_One[Addr * 4], 4);//32KB����ת��Ϊ1WORDһ��

		    //initial_hex����(���ݽ��з�ת)����תǰΪ:initial_hex_int = ((initial_hex[0] << 24) + (initial_hex[1] << 16) + (initial_hex[2] << 8) + initial_hex[3]);

			initial_hex_int = ((initial_hex[3] << 24) + (initial_hex[2] << 16) + (initial_hex[1] << 8) + initial_hex[0]);

			Addr_log = (Addr << 16) + Addr;//addr_log

			key32 = ROMKEY ^ Addr_log;
			key64 = ((key32 & 0x00000000FFFFFFFF )<< 32) | ((~key32) & 0x00000000FFFFFFFF);//�γ�64λ�µ�KEY

			new_Data40 = sbox4(sbox4(sbox4(sbox4(initial_hex_int, (UINT16)(key64 & 0xFFFF)), (UINT16)((key64 & 0xFFFF0000) >> 16)), (UINT16)((key64 & 0xFFFF00000000) >> 32)), (UINT16)((key64 & 0xFFFF000000000000) >> 48));

			int new_Data40_bit = 1;

			for (int new_Data40_i = 0; new_Data40_i < 32; new_Data40_i++)
			{
				new_Data40_uc[new_Data40_i] = ((new_Data40 & new_Data40_bit) >> new_Data40_i);

				new_Data40_bit *= 2;
			}

			//����λ��ΪУ��λ���ڴ˲���Ϊ��������
			//enc_ck4 = new_Data40_uc[31] ^ new_Data40_uc[29] ^ new_Data40_uc[27] ^ new_Data40_uc[25] ^ new_Data40_uc[23] ^ new_Data40_uc[21] ^ new_Data40_uc[19] ^ new_Data40_uc[17];
			//enc_ck3 = new_Data40_uc[30] ^ new_Data40_uc[28] ^ new_Data40_uc[26] ^ new_Data40_uc[24] ^ new_Data40_uc[22] ^ new_Data40_uc[20] ^ new_Data40_uc[18] ^ new_Data40_uc[16];
			//enc_ck2 = new_Data40_uc[15] ^ new_Data40_uc[13] ^ new_Data40_uc[11] ^ new_Data40_uc[9] ^ new_Data40_uc[7] ^ new_Data40_uc[5] ^ new_Data40_uc[3] ^ new_Data40_uc[1];
			//enc_ck1 = new_Data40_uc[14] ^ new_Data40_uc[12] ^ new_Data40_uc[10] ^ new_Data40_uc[8] ^ new_Data40_uc[6] ^ new_Data40_uc[4] ^ new_Data40_uc[2] ^ new_Data40_uc[0];

			
			//ע�⣺enc_hex��Ҫ�ٴν��з�ת
			//enc_hex = ((enc_ck4 & 0x000000001) << 35) | ((enc_ck3 & 0x000000002) << 34) | ((enc_ck2 & 0x000000004) << 33) | ((enc_ck1 & 0x000000008) << 32) | new_Data40;

			enc_hex = (((new_Data40_uc[31] * 8 + new_Data40_uc[30] * 4 + new_Data40_uc[29] * 2 + new_Data40_uc[28]) << 4) + (new_Data40_uc[27] * 8 + new_Data40_uc[26] * 4 + new_Data40_uc[25] * 2 + new_Data40_uc[24])) +
				((((new_Data40_uc[23] * 8 + new_Data40_uc[22] * 4 + new_Data40_uc[21] * 2 + new_Data40_uc[20]) << 4) + (new_Data40_uc[19] * 8 + new_Data40_uc[18] * 4 + new_Data40_uc[17] * 2 + new_Data40_uc[16])) << 8) +
				((((new_Data40_uc[15] * 8 + new_Data40_uc[14] * 4 + new_Data40_uc[13] * 2 + new_Data40_uc[12]) << 4) + (new_Data40_uc[11] * 8 + new_Data40_uc[10] * 4 + new_Data40_uc[9] * 2 + new_Data40_uc[8])) << 16) +
				((((new_Data40_uc[7] * 8 + new_Data40_uc[6] * 4 + new_Data40_uc[5] * 2 + new_Data40_uc[4]) << 4) + (new_Data40_uc[3] * 8 + new_Data40_uc[2] * 4 + new_Data40_uc[1] * 2 + new_Data40_uc[0])) << 24);

			/*��ַ�������*/
			ad_tp = (Addr_log & 0x00001FFF) ^ ((ROMKEY & 0x00007FFC) >> 2);//Addr_log[12:0] ^ ROMKEY[14:2](ע�⣺Ҫ����2λ����2λɾ��)

			int ad_tp_bit = 0x01;

			for (int ad_tp_i = 0; ad_tp_i < 16; ad_tp_i++)
			{

				ad_tp_uc[ad_tp_i] = (ad_tp & ad_tp_bit) >> ad_tp_i;

				ad_tp_bit *= 2;
			}

			ad_phy = (ad_tp_uc[6] << 12) +
				((ad_tp_uc[10] * 8 + ad_tp_uc[8] * 4 + ad_tp_uc[5] * 2 + ad_tp_uc[12]) << 8) +
				((ad_tp_uc[9] * 8 + ad_tp_uc[7] * 4 + ad_tp_uc[11] * 2 + ad_tp_uc[2]) << 4) +
				(ad_tp_uc[0] * 8 + ad_tp_uc[1] * 4 + ad_tp_uc[3] * 2 + ad_tp_uc[4]);

			Enc_word_data[ad_phy] = enc_hex;

		}

		//������ɣ�����������ת��Ϊ�ַ������

		CString Str_enc_dat = _T("");
		char Enc_dat[9] = { 0 };

		for (int Enc_num_1 = 0; Enc_num_1<8192; Enc_num_1++)
		{
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF000000) >> 24); Enc_num++;
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF0000) >> 16); Enc_num++;
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF00) >> 8); Enc_num++;
			Enc_Data[Enc_num] = (Enc_word_data[Enc_num_1] & 0xFF); Enc_num++;

			//to compare RCF,eg.(49 a4 f1 29)-->(29 f1 a4 49)  changed by ghan in 3.30.2017
			Enc_dat[6] = ((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) + '0' : (((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) - 10) + 'a';
			Enc_dat[7] = ((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) + '0' : (((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) - 10) + 'a';
			Enc_dat[4] = ((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) + '0' : (((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) - 10) + 'a';
			Enc_dat[5] = ((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) + '0' : (((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) - 10) + 'a';
			Enc_dat[2] = ((Enc_word_data[Enc_num_1] & 0xF000) >> 12) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF000) >> 12) + '0' : (((Enc_word_data[Enc_num_1] & 0xF000) >> 12) - 10) + 'a';
			Enc_dat[3] = ((Enc_word_data[Enc_num_1] & 0xF00) >> 8) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF00) >> 8) + '0' : (((Enc_word_data[Enc_num_1] & 0xF00) >> 8) - 10) + 'a';
			Enc_dat[0] = ((Enc_word_data[Enc_num_1] & 0xF0) >> 4) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF0) >> 4) + '0' : (((Enc_word_data[Enc_num_1] & 0xF0) >> 4) - 10) + 'a';
			Enc_dat[1] = (Enc_word_data[Enc_num_1] & 0x0F) <= 9 ? (Enc_word_data[Enc_num_1] & 0x0F) + '0' : ((Enc_word_data[Enc_num_1] & 0x0F) - 10) + 'a';

			Str_enc_dat.Format(_T("%s"), Enc_dat);

			Str_enc_dat += "\r\n";

			STR_ENC += Str_enc_dat;
		}

		memset(Enc_word_data, 0, sizeof(Enc_word_data));
	}

	return true;
}

bool Hex_328A_Enc64K(unsigned char* ROMKEY_UC, int Input_len, unsigned char* Input_Data, unsigned char* Enc_Data)//328A_64K����
{
	//���ܲ�����ʼ

	unsigned int ROMKEY = (ROMKEY_UC[0] << 24) + (ROMKEY_UC[1] << 16) + (ROMKEY_UC[2] << 8) + ROMKEY_UC[3];

	unsigned char Enc_Data_One[65536] = { 0 };

	unsigned int ad_tp = 0;

	int Enc_num = 0;

	for (int block_i = 0; block_i < Input_len / 65536; block_i++)
	{
		memcpy(Enc_Data_One, &Input_Data[block_i*65536], 65536);//��ԭ���ݷ�Ϊn��64K�����򣬴��뵥����������

		unsigned int Enc_word_data[16384] = { 0 };

		for (unsigned int Addr = 0; Addr < 16384; Addr++)//64K�������Ϊ16384��word��4byte��
		{
			unsigned int initial_hex_int = 0, Addr_log = 0, ad_phy = 0;
			unsigned char initial_hex[4] = { 0 }, new_Data40_uc[32] = { 0 }, ad_tp_uc[16] = { 0 };
			unsigned char enc_ck4 = 0, enc_ck3 = 0, enc_ck2 = 0, enc_ck1 = 0;
			UINT32 new_Data40 = 0, enc_hex = 0;
			UINT64 key64 = 0, key32 = 0;
			UINT16 key16 = 0;

			memcpy(initial_hex, &Enc_Data_One[Addr * 4], 4);//64KB����ת��Ϊ1WORDһ��

			//initial_hex����(���ݽ��з�ת)����תǰΪ:initial_hex_int = ((initial_hex[0] << 24) + (initial_hex[1] << 16) + (initial_hex[2] << 8) + initial_hex[3]);

			initial_hex_int = ((initial_hex[3] << 24) + (initial_hex[2] << 16) + (initial_hex[1] << 8) + initial_hex[0]);

			Addr_log = (Addr << 16) + Addr;//addr_log

			key32 = ROMKEY ^ Addr_log;
			key64 = ((key32 & 0x00000000FFFFFFFF) << 32) | ((~key32) & 0x00000000FFFFFFFF);//�γ�64λ�µ�KEY

			new_Data40 = sbox4(sbox4(sbox4(sbox4(initial_hex_int, (UINT16)(key64 & 0xFFFF)), (UINT16)((key64 & 0xFFFF0000) >> 16)), (UINT16)((key64 & 0xFFFF00000000) >> 32)), (UINT16)((key64 & 0xFFFF000000000000) >> 48));

			int new_Data40_bit = 1;

			for (int new_Data40_i = 0; new_Data40_i < 32; new_Data40_i++)
			{
				new_Data40_uc[new_Data40_i] = ((new_Data40 & new_Data40_bit) >> new_Data40_i);

				new_Data40_bit *= 2;
			}
			//����λ��ΪУ��λ���ڴ˲���Ϊ��������
			//enc_ck4 = new_Data40_uc[31] ^ new_Data40_uc[29] ^ new_Data40_uc[27] ^ new_Data40_uc[25] ^ new_Data40_uc[23] ^ new_Data40_uc[21] ^ new_Data40_uc[19] ^ new_Data40_uc[17];
			//enc_ck3 = new_Data40_uc[30] ^ new_Data40_uc[28] ^ new_Data40_uc[26] ^ new_Data40_uc[24] ^ new_Data40_uc[22] ^ new_Data40_uc[20] ^ new_Data40_uc[18] ^ new_Data40_uc[16];
			//enc_ck2 = new_Data40_uc[15] ^ new_Data40_uc[13] ^ new_Data40_uc[11] ^ new_Data40_uc[9] ^ new_Data40_uc[7] ^ new_Data40_uc[5] ^ new_Data40_uc[3] ^ new_Data40_uc[1];
			//enc_ck1 = new_Data40_uc[14] ^ new_Data40_uc[12] ^ new_Data40_uc[10] ^ new_Data40_uc[8] ^ new_Data40_uc[6] ^ new_Data40_uc[4] ^ new_Data40_uc[2] ^ new_Data40_uc[0];
			
			//ע�⣺enc_hex��Ҫ�ٴν��з�ת
			//enc_hex = ((enc_ck4 & 0x000000001) << 35) | ((enc_ck3 & 0x000000002) << 34) | ((enc_ck2 & 0x000000004) << 33) | ((enc_ck1 & 0x000000008) << 32) | new_Data40;

			enc_hex = (((new_Data40_uc[31] * 8 + new_Data40_uc[30] * 4 + new_Data40_uc[29] * 2 + new_Data40_uc[28]) << 4) + (new_Data40_uc[27] * 8 + new_Data40_uc[26] * 4 + new_Data40_uc[25] * 2 + new_Data40_uc[24])) +
				((((new_Data40_uc[23] * 8 + new_Data40_uc[22] * 4 + new_Data40_uc[21] * 2 + new_Data40_uc[20]) << 4) + (new_Data40_uc[19] * 8 + new_Data40_uc[18] * 4 + new_Data40_uc[17] * 2 + new_Data40_uc[16])) << 8) +
				((((new_Data40_uc[15] * 8 + new_Data40_uc[14] * 4 + new_Data40_uc[13] * 2 + new_Data40_uc[12]) << 4) + (new_Data40_uc[11] * 8 + new_Data40_uc[10] * 4 + new_Data40_uc[9] * 2 + new_Data40_uc[8])) << 16) +
				((((new_Data40_uc[7] * 8 + new_Data40_uc[6] * 4 + new_Data40_uc[5] * 2 + new_Data40_uc[4]) << 4) + (new_Data40_uc[3] * 8 + new_Data40_uc[2] * 4 + new_Data40_uc[1] * 2 + new_Data40_uc[0])) << 24);

			/*��ַ�������*/
			ad_tp = (Addr_log & 0x00001FFF) ^ ((ROMKEY & 0x00007FFC) >> 2);//Addr_log[12:0] ^ ROMKEY[14:2](ע�⣺Ҫ����2λ����2λɾ��)

			int ad_tp_bit = 0x01;

			for (int ad_tp_i = 0; ad_tp_i < 16; ad_tp_i++)
			{

				ad_tp_uc[ad_tp_i] = (ad_tp & ad_tp_bit) >> ad_tp_i;

				ad_tp_bit *= 2;
			}

			ad_tp_uc[13] = (Addr_log & 0x00002000) >> 13;//add 328A spw encypt by ghan in 2017.1.10
		
			ad_phy = (ad_tp_uc[13] << 13) + (ad_tp_uc[6] << 12) +
				((ad_tp_uc[10] * 8 + ad_tp_uc[8] * 4 + ad_tp_uc[5] * 2 + ad_tp_uc[12]) << 8) +
				((ad_tp_uc[9] * 8 + ad_tp_uc[7] * 4 + ad_tp_uc[11] * 2 + ad_tp_uc[2]) << 4) +
				(ad_tp_uc[0] * 8 + ad_tp_uc[1] * 4 + ad_tp_uc[3] * 2 + ad_tp_uc[4]);

			Enc_word_data[ad_phy] = enc_hex;

		}

		//������ɣ�����������ת��Ϊ�ַ������

		CString Str_enc_dat = _T("");
		char Enc_dat[9] = { 0 };

		for (int Enc_num_1 = 0; Enc_num_1<16384; Enc_num_1++)
		{
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF000000) >> 24); Enc_num++;
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF0000) >> 16); Enc_num++;
			Enc_Data[Enc_num] = ((Enc_word_data[Enc_num_1] & 0xFF00) >> 8); Enc_num++;
			Enc_Data[Enc_num] = (Enc_word_data[Enc_num_1] & 0xFF); Enc_num++;

			//to compare RCF,eg.(49 a4 f1 29)-->(29 f1 a4 49)  changed by ghan in 3.30.2017
			Enc_dat[6] = ((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) + '0' : (((Enc_word_data[Enc_num_1] & 0xF0000000) >> 28) - 10) + 'a';
			Enc_dat[7] = ((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) + '0' : (((Enc_word_data[Enc_num_1] & 0xF000000) >> 24) - 10) + 'a';
			Enc_dat[4] = ((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) + '0' : (((Enc_word_data[Enc_num_1] & 0xF00000) >> 20) - 10) + 'a';
			Enc_dat[5] = ((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) + '0' : (((Enc_word_data[Enc_num_1] & 0xF0000) >> 16) - 10) + 'a';
			Enc_dat[2] = ((Enc_word_data[Enc_num_1] & 0xF000) >> 12) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF000) >> 12) + '0' : (((Enc_word_data[Enc_num_1] & 0xF000) >> 12) - 10) + 'a';
			Enc_dat[3] = ((Enc_word_data[Enc_num_1] & 0xF00) >> 8) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF00) >> 8) + '0' : (((Enc_word_data[Enc_num_1] & 0xF00) >> 8) - 10) + 'a';
			Enc_dat[0] = ((Enc_word_data[Enc_num_1] & 0xF0) >> 4) <= 9 ? ((Enc_word_data[Enc_num_1] & 0xF0) >> 4) + '0' : (((Enc_word_data[Enc_num_1] & 0xF0) >> 4) - 10) + 'a';
			Enc_dat[1] = (Enc_word_data[Enc_num_1] & 0x0F) <= 9 ? (Enc_word_data[Enc_num_1] & 0x0F) + '0' : ((Enc_word_data[Enc_num_1] & 0x0F) - 10) + 'a';

			Str_enc_dat.Format(_T("%s"), Enc_dat);

			Str_enc_dat += "\r\n";

			STR_ENC += Str_enc_dat;
		}

		memset(Enc_word_data, 0, sizeof(Enc_word_data));
	}

	return true;
}

unsigned char buffer_PROM[ADD_LEN] = { 0 }, buffer_ATMC[ADD_LEN] = { 0 }, buffer_TROM[ADD_LEN] = { 0 };
unsigned char gPromAlglibHexStd[ADD_LEN] = { 0 },gPromAlglibHexFre[ADD_LEN] = { 0 }, gPromAlglibRcf[ADD_LEN] = { 0 }, gHexStdEnc[ADD_LEN_PROM_ATMC] = { 0 };

void CFPGA_Memory_SetDlg::OnBnClickedButtonRam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*�����λ������״̬*/

	if(((CButton*)GetDlgItem(IDC_RADIO_PROM))->GetCheck())
	{
		check_PROM = TRUE;
		check_TROM = FALSE;
		check_ATMC = FALSE;
	}
	else if(((CButton*)GetDlgItem(IDC_RADIO_ATMC))->GetCheck())
	{
		check_PROM = FALSE;
		check_TROM = FALSE;
		check_ATMC = TRUE;
	}
	else
	{
		check_PROM = FALSE;
		check_TROM = TRUE;
		check_ATMC = FALSE;
	}

	memset(buffer_PROM, 0, ADD_LEN);
	memset(buffer_ATMC, 0, ADD_LEN);
	memset(buffer_TROM, 0, ADD_LEN);

	DWORD ThreadID;
	DWORD code;

	UpdateData(FALSE);

	STR_ENC = _T("");

	m_persent = 0;

	Func_Enable(FALSE);

	SetDlgItemText(IDC_Persent, "%0");

	if (!GetExitCodeThread(hThread1, &code) || (code != STILL_ACTIVE))
	{
		hThread1 = CreateThread(NULL, 0, ThreadFun, this, 0, &ThreadID);//��������ʼ�߳�

	}
}


DWORD WINAPI ThreadFun(LPVOID pArg)						//�߳���ں���
{
	CFPGA_Memory_SetDlg* cMyDlg = (CFPGA_Memory_SetDlg*)pArg;
	CButton* cMyDlgButton = (CButton*)pArg;

	unsigned char trans_buffer[1024] = { 0 }, send_buffer[SEND_SIZE] = { 0 }, recv_buffer[1024] = { 0 },ROMKEY_UC[4] = {0};

	unsigned char PROM_Enc[ADD_LEN_PROM] = {0},ATMC_Enc[ADD_LEN_ATMC] = {0},TROM_Enc[ADD_LEN_TROM] = {0};

	char memory[64] = { 0 };

	unsigned char data_len[64] = { 0 };

	int tras_buf1_PROM = 0, tras_buf2_PROM = 0, tras_add_PROM = 0, tras_buf0_PROM = 0;
	int tras_buf1_TROM = 0, tras_buf2_TROM = 0, tras_add_TROM = 0, tras_buf0_TROM = 0;
	int tras_buf1_ATMC = 0, tras_buf2_ATMC = 0, tras_add_ATMC = 0, tras_buf0_ATMC = 0;

	int m_2 = 0, m_1_TROM = 0, m_1_PROM = 0, m_1_ATMC = 0, m_all = 0;

	CString str = _T(""),strText = _T("");

	/*��ȡROMKEY*/
	CString STR_ROMKEY;

	cMyDlg->GetDlgItem(IDC_EDIT_ROMKEY)->GetWindowText(STR_ROMKEY);

	if(STR_ROMKEY[0] < '0' || STR_ROMKEY[0] > 'F') goto error1;
	else if(STR_ROMKEY[1] < '0' || STR_ROMKEY[1] > 'F') goto error1;
	else if (STR_ROMKEY[2] < '0' || STR_ROMKEY[2] > 'F')goto error1;
	else if (STR_ROMKEY[3] < '0' || STR_ROMKEY[3] > 'F')goto error1;
	else if (STR_ROMKEY[4] < '0' || STR_ROMKEY[4] > 'F')goto error1;
	else if(STR_ROMKEY[5] < '0' || STR_ROMKEY[5] > 'F') goto error1;
	else if(STR_ROMKEY[6] < '0' || STR_ROMKEY[6] > 'F') goto error1;
	else if(STR_ROMKEY[7] < '0' || STR_ROMKEY[7] > 'F') goto error1;
	else if (strlen(STR_ROMKEY) != 8)
	{
error1: cMyDlg->MessageBox("Format error��Romkey", "ERROR_ROMKEY", MB_ICONEXCLAMATION);
		cMyDlg->Func_Enable(TRUE);
		return false;
	}

	transCStringbuffer(STR_ROMKEY,ROMKEY_UC);

	//����WORD��ʽת��
	if(cMyDlg->m_combox_form.GetCurSel() == FILE_RCF_ENC)//RCF�ļ�
	{
		if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
		{
			if (cMyDlg->check_PROM == TRUE)//PROM&ALGLIB�ֳ�3��64K�ļ�����
			{
				//�����һ���ļ�
				cMyDlg->SetDlgItemText(IDC_STATUES, "Loading the first '64K rcf'file...");
				CFileDialog dlgRcf1(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Rcf Files(*.rcf)|*.rcf|All Files (*.*)|*.*||");
				if (dlgRcf1.DoModal() != IDOK)
				{
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				//����ļ�·��
				CString strPathNameRcf1 = dlgRcf1.GetPathName();
				CStdioFile fileRcf1;
				//���ļ�
				if (!fileRcf1.Open(strPathNameRcf1, CFile::modeRead))
				{
					cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				while (fileRcf1.ReadString(strText))
				{
					strText = strText.Right(strText.GetLength());
					transCStringbuffer(strText, trans_buffer);

					for (m_1_PROM = 4; m_1_PROM > 0; m_1_PROM--)//�����ݴ���buffer��
					{
						memcpy_s(&gPromAlglibRcf[tras_add_PROM],ADD_LEN, &trans_buffer[m_1_PROM - 1], 1);//Modified by ghan in 2017/01/14
						tras_add_PROM++;
					}
				}
				fileRcf1.Close();//�ر��ļ�

				cMyDlg->MessageBox("Completed!\r\nPlease load the second Rcf file.", "Rcf Download", MB_ICONINFORMATION);

				//����ڶ����ļ�

				tras_add_PROM = 64*1024;

				cMyDlg->SetDlgItemText(IDC_STATUES, "Loading the second '64K rcf'file...");
				CFileDialog dlgRcf2(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Rcf Files(*.rcf)|*.rcf|All Files (*.*)|*.*||");
				if (dlgRcf2.DoModal() != IDOK)
				{
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				//����ļ�·��
				CString strPathNameRcf2 = dlgRcf2.GetPathName();
				CStdioFile fileRcf2;
				//���ļ�
				if (!fileRcf2.Open(strPathNameRcf2, CFile::modeRead))
				{
					cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				while (fileRcf2.ReadString(strText))
				{
					strText = strText.Right(strText.GetLength());

					transCStringbuffer(strText, trans_buffer);

					for (m_1_PROM = 4; m_1_PROM > 0; m_1_PROM--)//�����ݴ���buffer��
					{
						memcpy_s(&gPromAlglibRcf[tras_add_PROM], ADD_LEN, &trans_buffer[m_1_PROM - 1], 1);
						tras_add_PROM++;
					}
				}
				fileRcf2.Close();//�ر��ļ�

				cMyDlg->MessageBox("Completed!\r\nPlease load the third Rcf file.", "Rcf Download", MB_ICONINFORMATION);

				//����������ļ�

				tras_add_PROM = 128*1024;

				cMyDlg->SetDlgItemText(IDC_STATUES, "Loading the third '64K rcf'file...");
				CFileDialog dlgRcf3(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Rcf Files(*.rcf)|*.rcf|All Files (*.*)|*.*||");
				if (dlgRcf3.DoModal() != IDOK)
				{
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				//����ļ�·��
				CString strPathNameRcf3 = dlgRcf3.GetPathName();
				CStdioFile fileRcf3;
				//���ļ�
				if (!fileRcf3.Open(strPathNameRcf3, CFile::modeRead))
				{
					cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				while (fileRcf3.ReadString(strText))
				{
					strText = strText.Right(strText.GetLength());

					transCStringbuffer(strText, trans_buffer);

					for (m_1_PROM = 4; m_1_PROM > 0; m_1_PROM--)//�����ݴ���buffer��
					{
						memcpy_s(&gPromAlglibRcf[tras_add_PROM], ADD_LEN, &trans_buffer[m_1_PROM - 1], 1);
						tras_add_PROM++;
					}
				}
				fileRcf3.Close();//�ر��ļ�

				memcpy_s(&buffer_PROM[0], ADD_LEN,&gPromAlglibRcf[0], ADD_LEN_PROM);

				//cMyDlg->GetDlgItem(IDC_RADIO_ATMC)->EnableWindow(true);

				data_len[0] = 0x00;
				data_len[1] = 0x00;
				data_len[2] = 0x00;
			}
			else if (cMyDlg->check_ATMC == TRUE)//�㷨��
			{
				memcpy_s(&buffer_ATMC[0x28000], ADD_LEN, &gPromAlglibRcf[0x28000], ADD_LEN_ATMC);

				data_len[0] = 0x02;
				data_len[1] = 0x80;
				data_len[2] = 0x00;
			}
		}
		else
		{
			if (cMyDlg->check_PROM == TRUE)//PROM
			{
				cMyDlg->SetDlgItemText(IDC_STATUES, "Loading file...");

				CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Rcf Files(*.rcf)|*.rcf|All Files (*.*)|*.*||");

				if (dlg.DoModal() != IDOK)
				{
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				//����ļ�·��
				CString strPathName = dlg.GetPathName();
				CStdioFile file;

				//���ļ�
				if (!file.Open(strPathName, CFile::modeRead))
				{
					cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				while (file.ReadString(strText))
				{
					strText = strText.Right(strText.GetLength() - 1);

					transCStringbuffer(strText, trans_buffer);

					for (m_1_PROM = 4; m_1_PROM > 0; m_1_PROM--)//�����ݴ���buffer��
					{
						buffer_PROM[tras_add_PROM] = trans_buffer[m_1_PROM - 1];

						tras_add_PROM++;
					}
				}
				data_len[0] = 0x00;
				data_len[1] = 0x00;
				data_len[2] = 0x00;

				file.Close();//�ر��ļ�
			}
			else if (cMyDlg->check_ATMC == TRUE)//�㷨��
			{
				cMyDlg->SetDlgItemText(IDC_STATUES, "Loading file...");

				CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Rcf Files(*.rcf)|*.rcf|All Files (*.*)|*.*||");

				if (dlg.DoModal() != IDOK)
				{
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				//����ļ�·��
				CString strPathName = dlg.GetPathName();
				CStdioFile file;

				//���ļ�
				if (!file.Open(strPathName, CFile::modeRead))
				{
					cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}
				while (file.ReadString(strText))
				{
					strText = strText.Right(strText.GetLength() - 1);

					transCStringbuffer(strText, trans_buffer);

					for (m_1_ATMC = 4; m_1_ATMC > 0; m_1_ATMC--)//�����ݴ���buffer��
					{
						buffer_ATMC[tras_add_ATMC + 0x28000] = trans_buffer[m_1_ATMC - 1];

						tras_add_ATMC++;
					}
				}
				data_len[0] = 0x02;
				data_len[1] = 0x80;
				data_len[2] = 0x00;

				file.Close();//�ر��ļ�
			}
		}
		if (cMyDlg->check_TROM == TRUE)//TROM
		{
			cMyDlg->SetDlgItemText(IDC_STATUES, "Loading file...");

			CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Rcf Files(*.rcf)|*.rcf|All Files (*.*)|*.*||");

			if (dlg.DoModal() != IDOK)
			{
				cMyDlg->Func_Enable(TRUE);
				return false;
			}

			//����ļ�·��
			CString strPathName = dlg.GetPathName();
			CStdioFile file;

			//���ļ�
			if (!file.Open(strPathName, CFile::modeRead))
			{
				cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable(TRUE);
				return false;
			}
			while (file.ReadString(strText))
			{
				if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
				{
					strText = strText.Right(strText.GetLength());
				}
				else
				{
					strText = strText.Right(strText.GetLength() - 1);
				}

				transCStringbuffer(strText, trans_buffer);//������ת��

				for (m_1_TROM = 4; m_1_TROM > 0; m_1_TROM--)//�����ݴ���buffer��
				{
					buffer_TROM[tras_add_TROM + 0x40000] = trans_buffer[m_1_TROM - 1];

					tras_add_TROM++;
				}
			}
			data_len[0] = 0x04;
			data_len[1] = 0x00;
			data_len[2] = 0x00;

			file.Close();//�ر��ļ�
		}
	}
	else if(cMyDlg->m_combox_form.GetCurSel() == FILE_HEX_STAND_NOENC)//����HEX��׼��ʽת��
	{
		if (cMyDlg->check_PROM == TRUE)//PROM
		{
			cMyDlg->SetDlgItemText(IDC_STATUES, "Loading file...");

			CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Hex Files(*.hex)|*.hex|All Files (*.*)|*.*||");

			//CFileDialog dlg(TRUE, "�����ļ�", "*.*", OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_ALLOWMULTISELECT, "Hex Files(*.hex)|*.hex|All Files (*.*)|*.*||");

			if (dlg.DoModal() != IDOK)
			{
				cMyDlg->Func_Enable(TRUE);
				return false;
			}

			//����ļ�·��
			CString strPathName = dlg.GetPathName();
			CStdioFile file;

			//���ļ�
			if (!file.Open(strPathName, CFile::modeRead))
			{
				cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable(TRUE);
				return false;
			}
			while (file.ReadString(strText))
			{
				if (strText[0] != ':')//�����Ϣ������
				{
					cMyDlg->MessageBox("PROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText, "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "PROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				strText.Remove(':');

				if (strText[7] != '0')
				{
					if (strText[7] == '4')//������չ��ַ��
					{
						if (strText[11] == '0')
						{
							tras_buf0_PROM = 0;
							continue;
						}
						else if (strText[11] == '1')
						{
							tras_buf0_PROM = 1;
							continue;
						}
						else if (strText[11] == '2')
						{
							tras_buf0_PROM = 2;
							continue;
						}
						else
						{
							cMyDlg->MessageBox("PROM�ĵ��д��ڴ���������չ��ַ��������Ϣ��:" + strText, "ERROR��", MB_ICONEXCLAMATION);
							cMyDlg->SetDlgItemText(IDC_STATUES, "������Ϣ��:" + strText);
							cMyDlg->Func_Enable(TRUE);
							return false;
						}
					}
					else if (strText[7] == '1')//�ļ�������־
					{
						break;
					}
					else continue;//���ݴ����У�����       **********�˴���־λ�����Ժ������չ***********
				}

				transCStringbuffer(strText, trans_buffer);

				tras_buf1_PROM = trans_buffer[1];
				tras_buf2_PROM = trans_buffer[2];
				tras_add_PROM = (tras_buf0_PROM << 16) | (tras_buf1_PROM << 8) | tras_buf2_PROM;

				if(tras_add_PROM > 0x27FFF)
				{
					cMyDlg->MessageBox("�����PROM�ĵ���", "ERROR!", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "�����PROM�ĵ�");
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				for (m_1_PROM = 0; m_1_PROM < trans_buffer[0]; m_1_PROM++)//�����ݴ���buffer��
				{
					buffer_PROM[tras_add_PROM + m_1_PROM] = trans_buffer[m_1_PROM + 4];
				}

			}
			data_len[0] = 0x00;
			data_len[1] = 0x00;
			data_len[2] = 0x00;

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)//328A SPW 192K���ݴ���
			{
				memcpy_s(&gPromAlglibHexStd[0], ADD_LEN, &buffer_PROM[0], ADD_LEN_PROM);
				cMyDlg->MessageBox("pcos�ļ�������ɣ������һ���㷨���ļ�", "FPGA Memory Set", MB_ICONINFORMATION);
				goto Step2;
			}

			//����ת����ɣ����м���

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY188B_201512 || cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av1_0_201604)
			{
				Hex_188B_Enc(ROMKEY_UC, ADD_LEN_PROM, buffer_PROM, PROM_Enc);//188B
			}
			else if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av2_0_201609 || 
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY128Av1_0_201609)
			{
				Hex_328A_Enc(ROMKEY_UC, ADD_LEN_PROM, buffer_PROM, PROM_Enc);//328A
			}

			file.Close();//�ر��ļ�
		}
		if (cMyDlg->check_ATMC == TRUE)//�㷨��
		{
			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)goto Step3;

Step2:		cMyDlg->SetDlgItemText(IDC_STATUES, "���ĵ�...");

			CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Hex Files(*.hex)|*.hex|All Files (*.*)|*.*||");

			if (dlg.DoModal() != IDOK)
			{
				cMyDlg->Func_Enable(TRUE);
				return false;
			}

			//����ļ�·��
			CString strPathName = dlg.GetPathName();
			CStdioFile file;

			//���ļ�
			if (!file.Open(strPathName, CFile::modeRead))
			{
				cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable(TRUE);
				return false;
			}
			while (file.ReadString(strText))
			{
				if (strText[0] != ':')//�����Ϣ������
				{
					cMyDlg->MessageBox("�㷨���ĵ��д��ڴ������ݣ�������ϢΪ:" + strText, "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "�㷨���ĵ��д��ڴ������ݣ�������ϢΪ:" + strText);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				strText.Remove(':');

				if (strText[7] != '0')
				{
					if (strText[7] == '4')
					{
						if (strText[11] == '2')
						{
							tras_buf0_ATMC = 2;
							continue;
						}
						else
						{
							cMyDlg->MessageBox("�㷨���ĵ��д��ڴ���������չ��ַ��������Ϣ��:" + strText, "ERROR��", MB_ICONEXCLAMATION);
							cMyDlg->SetDlgItemText(IDC_STATUES, "������Ϣ:" + strText);
							cMyDlg->Func_Enable(TRUE);
							return false;
						}
					}
					else if (strText[7] == '1')break;//�ļ�������־
					else continue;//���ݴ����У�����       **********�˴���־λ�����Ժ������չ***********
				}

				transCStringbuffer(strText, trans_buffer);//������ת��

				tras_buf1_ATMC = trans_buffer[1];
				tras_buf2_ATMC = trans_buffer[2];
				tras_add_ATMC = (tras_buf0_ATMC << 16) | (tras_buf1_ATMC << 8) | tras_buf2_ATMC;

				if((tras_add_ATMC < 0x28000) || (tras_add_ATMC > 0x2FFFF))
				{
					cMyDlg->MessageBox("������㷨���ĵ���", "ERROR!", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "������㷨���ĵ�");
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				for (m_1_ATMC = 0; m_1_ATMC < trans_buffer[0]; m_1_ATMC++)//�����ݴ���buffer��
				{
					buffer_ATMC[tras_add_ATMC + m_1_ATMC] = trans_buffer[m_1_ATMC + 4];
				}
			}
			data_len[0] = 0x02;
			data_len[1] = 0x80;
			data_len[2] = 0x00;

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)//328A SPW 192K���ݴ���
			{
				memcpy_s(&gPromAlglibHexStd[0x28000], ADD_LEN, &buffer_ATMC[0x28000], ADD_LEN_ATMC);
			}

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY188B_201512 || cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av1_0_201604)
			{
				Hex_188B_Enc(ROMKEY_UC, ADD_LEN_ATMC, &buffer_ATMC[0x28000], ATMC_Enc);//188B���㷨
			}
			else if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av2_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY128Av1_0_201609)
			{
				Hex_328A_Enc(ROMKEY_UC, ADD_LEN_ATMC, &buffer_ATMC[0x28000], ATMC_Enc);//�����㷨
			}
			file.Close();//�ر��ļ�
		}

		if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)//328A SPW 192K���ݴ���
		{
			Hex_328A_Enc64K(ROMKEY_UC, ADD_LEN_PROM_ATMC, &gPromAlglibHexStd[0], gHexStdEnc);

			if (cMyDlg->check_PROM == TRUE)
			{
				data_len[0] = 0x00;
				data_len[1] = 0x00;
				data_len[2] = 0x00;
				memcpy_s(&PROM_Enc[0], ADD_LEN_PROM, &gHexStdEnc[0], ADD_LEN_PROM);
			}
Step3: 		if (cMyDlg->check_ATMC == TRUE)
			{
				data_len[0] = 0x02;
				data_len[1] = 0x80;
				data_len[2] = 0x00;
				memcpy_s(&ATMC_Enc[0], ADD_LEN_ATMC, &gHexStdEnc[0x28000], ADD_LEN_ATMC);
			}
		}

		if (cMyDlg->check_TROM == TRUE)//TROM
		{
			cMyDlg->SetDlgItemText(IDC_STATUES, "���ĵ�...");

			CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Hex Files(*.hex)|*.hex|All Files (*.*)|*.*||");

			if (dlg.DoModal() != IDOK)
			{
				cMyDlg->Func_Enable(TRUE);
				return false;
			}

			//����ļ�·��
			CString strPathName = dlg.GetPathName();
			CStdioFile file;

			//���ļ�
			if (!file.Open(strPathName, CFile::modeRead))
			{
				cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
				cMyDlg->Func_Enable(TRUE);
				return false;
			}
			while (file.ReadString(strText))
			{
				if (strText[0] != ':')//�����Ϣ������
				{
					cMyDlg->MessageBox("TROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText, "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "TROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				strText.Remove(':');

				if (strText[7] != '0')
				{
					if (strText[7] == '4')
					{
						if (strText[11] == '4')
						{
							tras_buf0_TROM = 4;
							continue;
						}
						else
						{
							cMyDlg->MessageBox("TROM�ĵ��д��ڴ���������չ��ַ��������Ϣ��:" + strText, "ERROR��", MB_ICONEXCLAMATION);
							cMyDlg->SetDlgItemText(IDC_STATUES, "������Ϣ:" + strText);
							cMyDlg->Func_Enable(TRUE);
							return false;
						}
					}
					else if (strText[7] == '1') break;//�ļ�������־
					else continue;//���ݴ����У�����       **********�˴���־λ�����Ժ������չ***********
				}

				transCStringbuffer(strText, trans_buffer);//������ת��

				tras_buf1_TROM = trans_buffer[1];
				tras_buf2_TROM = trans_buffer[2];
				tras_add_TROM = (tras_buf0_TROM << 16) | (tras_buf1_TROM << 8) | tras_buf2_TROM;

				if((tras_add_TROM < 0x40000) || (tras_add_TROM > 0x47FFF))
				{
					cMyDlg->MessageBox("�����TROM�ĵ���","ERROR!", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "�����TROM�ĵ�");
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				for (m_1_TROM = 0; m_1_TROM < trans_buffer[0]; m_1_TROM++)//�����ݴ���buffer��
				{
					buffer_TROM[tras_add_TROM + m_1_TROM] = trans_buffer[m_1_TROM + 4];
				}
			}
			data_len[0] = 0x04;
			data_len[1] = 0x00;
			data_len[2] = 0x00;

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY188B_201512 || cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av1_0_201604)
			{
				Hex_188B_Enc(ROMKEY_UC, ADD_LEN_TROM, &buffer_TROM[0x40000], TROM_Enc);
			}
			else if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av2_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY128Av1_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
			{
				Hex_328A_Enc(ROMKEY_UC, ADD_LEN_TROM, &buffer_TROM[0x40000], TROM_Enc);
			}
			file.Close();//�ر��ļ�
		}
	}
	else//�Զ���HEX�ļ���ʽ
	{
		cMyDlg->SetDlgItemText(IDC_STATUES, "���ĵ�...");

		CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_FILEMUSTEXIST, "Hex Files(*.hex)|*.hex|All Files (*.*)|*.*||");

		if (dlg.DoModal() != IDOK)
		{
			cMyDlg->Func_Enable(TRUE);
			return false;
		}

		//����ļ�·��
		CString strPathName = dlg.GetPathName();
		CStdioFile file;

		//���ļ�
		if (!file.Open(strPathName, CFile::modeRead))
		{
			cMyDlg->MessageBox("Load file error.", "ERROR��", MB_ICONEXCLAMATION);
			cMyDlg->Func_Enable(TRUE);
			return false;
		}

		if (cMyDlg->check_PROM == TRUE)//PROM
		{
			while (file.ReadString(strText))
			{
				if (strText[0] != ':')//�����Ϣ������
				{
					cMyDlg->MessageBox("PROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText, "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "PROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				strText.Remove(':');

				if (strText[7] != '0')
				{
					if (strText[7] == '4')//������չ��ַ��
					{
						if (strText[11] == '0')
						{
							tras_buf0_PROM = 0;
							continue;
						}
						else if (strText[11] == '1')
						{
							tras_buf0_PROM = 1;
							continue;
						}
						else if (strText[11] == '2')
						{
							tras_buf0_PROM = 2;
							continue;
						}
						else
						{
							cMyDlg->MessageBox("PROM�ĵ��д��ڴ���������չ��ַ��������Ϣ��:" + strText, "ERROR��", MB_ICONEXCLAMATION);
							cMyDlg->SetDlgItemText(IDC_STATUES, "������Ϣ��:" + strText);
							cMyDlg->Func_Enable(TRUE);
							return false;
						}
					}
					else if (strText[7] == '1')//�ļ�������־
					{
						break;
					}
					else continue;//���ݴ����У�����       **********�˴���־λ�����Ժ������չ***********
				}

				transCStringbuffer(strText, trans_buffer);

				tras_buf1_PROM = trans_buffer[1];
				tras_buf2_PROM = trans_buffer[2];
				tras_add_PROM = (tras_buf0_PROM << 16) | (tras_buf1_PROM << 8) | tras_buf2_PROM;

				if(tras_add_PROM > 0x27FFF)
				{
					cMyDlg->MessageBox("�����PROM�ĵ���","ERROR!", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "�����PROM�ĵ�");
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				for (m_1_PROM = 0; m_1_PROM < trans_buffer[0]; m_1_PROM++)//�����ݴ���buffer��
				{
					buffer_PROM[tras_add_PROM + m_1_PROM] = trans_buffer[m_1_PROM + 4];
				}

			}
			data_len[0] = 0x00;
			data_len[1] = 0x00;
			data_len[2] = 0x00;

			//����ת����ɣ����м���
			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY188B_201512 || cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av1_0_201604)
			{
				Hex_188B_Enc(ROMKEY_UC, ADD_LEN_PROM, buffer_PROM, PROM_Enc);//188B
			}
			else if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av2_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY128Av1_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
			{
				Hex_328A_Enc(ROMKEY_UC, ADD_LEN_PROM, buffer_PROM, PROM_Enc);//328A
			}

			file.Close();//�ر��ļ�
		}
		else if (cMyDlg->check_ATMC == TRUE)//�㷨��
		{
			while (file.ReadString(strText))
			{
				if (strText[0] != ':')//�����Ϣ������
				{
					cMyDlg->MessageBox("�㷨���ĵ��д��ڴ������ݣ�������ϢΪ:" + strText, "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "�㷨���ĵ��д��ڴ������ݣ�������ϢΪ:" + strText);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				strText.Remove(':');

				if (strText[7] != '0')
				{
					if (strText[7] == '4')
					{
						if (strText[11] == '2')
						{
							continue;
						}
						else
						{
							cMyDlg->MessageBox("�㷨���ĵ��д��ڴ���������չ��ַ��������Ϣ��:" + strText, "ERROR��", MB_ICONEXCLAMATION);
							cMyDlg->SetDlgItemText(IDC_STATUES, "������Ϣ:" + strText);
							cMyDlg->Func_Enable(TRUE);
							return false;
						}
					}
					else if (strText[7] == '1')break;//�ļ�������־
					else continue;//���ݴ����У�����       **********�˴���־λ�����Ժ������չ***********
				}

				transCStringbuffer(strText, trans_buffer);//������ת��

				tras_buf0_ATMC = 2;
				tras_buf1_ATMC = trans_buffer[1]+0x80;
				tras_buf2_ATMC = trans_buffer[2];
				tras_add_ATMC = (tras_buf0_ATMC << 16) | (tras_buf1_ATMC << 8) | tras_buf2_ATMC;

				if((tras_add_ATMC < 0x28000) || (tras_add_ATMC > 0x2FFFF))
				{
					cMyDlg->MessageBox("������㷨���ĵ���","ERROR!", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "������㷨���ĵ�");
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				for (m_1_ATMC = 0; m_1_ATMC < trans_buffer[0]; m_1_ATMC++)//�����ݴ���buffer��
				{
					buffer_ATMC[tras_add_ATMC + m_1_ATMC] = trans_buffer[m_1_ATMC + 4];
				}
			}
			data_len[0] = 0x02;
			data_len[1] = 0x80;
			data_len[2] = 0x00;

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY188B_201512 || cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av1_0_201604)
			{
				Hex_188B_Enc(ROMKEY_UC, ADD_LEN_ATMC, &buffer_ATMC[0x28000], ATMC_Enc);
			}
			else if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av2_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY128Av1_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
			{
				Hex_328A_Enc(ROMKEY_UC, ADD_LEN_ATMC, &buffer_ATMC[0x28000], ATMC_Enc);
			}

			file.Close();//�ر��ļ�

		}
		else//TROM
		{
			while (file.ReadString(strText))
			{
				if (strText[0] != ':')//�����Ϣ������
				{
					cMyDlg->MessageBox("TROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText, "ERROR��", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "TROM�ĵ��д��ڴ������ݣ�������ϢΪ:" + strText);
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				strText.Remove(':');

				if (strText[7] != '0')
				{
					if (strText[7] == '4')
					{
						if (strText[11] == '4')
						{
							continue;
						}
						else
						{
							cMyDlg->MessageBox("TROM�ĵ��д��ڴ���������չ��ַ��������Ϣ��:" + strText, "ERROR��", MB_ICONEXCLAMATION);
							cMyDlg->SetDlgItemText(IDC_STATUES, "������Ϣ:" + strText);
							cMyDlg->Func_Enable(TRUE);
							return false;
						}
					}
					else if (strText[7] == '1') break;//�ļ�������־
					else continue;//���ݴ����У�����       **********�˴���־λ�����Ժ������չ***********
				}

				transCStringbuffer(strText, trans_buffer);//������ת��

				tras_buf0_TROM = 4;
				tras_buf1_TROM = trans_buffer[1];
				tras_buf2_TROM = trans_buffer[2];
				tras_add_TROM = (tras_buf0_TROM << 16) | (tras_buf1_TROM << 8) | tras_buf2_TROM;

				if((tras_add_TROM < 0x40000) || (tras_add_TROM > 0x47FFF))
				{
					cMyDlg->MessageBox("�����TROM�ĵ���","ERROR!", MB_ICONEXCLAMATION);
					cMyDlg->SetDlgItemText(IDC_STATUES, "�����TROM�ĵ�");
					cMyDlg->Func_Enable(TRUE);
					return false;
				}

				for (m_1_TROM = 0; m_1_TROM < trans_buffer[0]; m_1_TROM++)//�����ݴ���buffer��
				{
					buffer_TROM[tras_add_TROM + m_1_TROM] = trans_buffer[m_1_TROM + 4];
				}
			}
			data_len[0] = 0x04;
			data_len[1] = 0x00;
			data_len[2] = 0x00;

			if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY188B_201512 || cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av1_0_201604)
			{
				Hex_188B_Enc(ROMKEY_UC, ADD_LEN_TROM, &buffer_TROM[0x40000], TROM_Enc);
			}
			else if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328Av2_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY128Av1_0_201609 ||
					 cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
			{
				Hex_328A_Enc(ROMKEY_UC, ADD_LEN_TROM, &buffer_TROM[0x40000], TROM_Enc);
			}

			file.Close();//�ر��ļ�
		}
	}

	//���͵�ַ

	USBBulkWriteData(0, EP1_OUT, (char*)data_len, EP1_OUT_SIZE, 500);

	if (cMyDlg->check_PROM == TRUE) USBBulkReadData(0, EP1_IN, (char*)recv_buffer, EP1_IN_SIZE, 500);
	else if(cMyDlg->check_ATMC == TRUE) USBBulkReadData(0, EP1_IN, (char*)recv_buffer, EP1_IN_SIZE, 2000);
	else USBBulkReadData(0, EP1_IN, (char*)recv_buffer, EP1_IN_SIZE, 4000);


	if (recv_buffer[3] != 0x10)
	{
		cMyDlg->MessageBox("��ַ����ʧ�ܣ������豸�����豸���������ϵ�", "ERROR��", MB_ICONEXCLAMATION);
		cMyDlg->SetDlgItemText(IDC_STATUES, "��ַ����ʧ�ܣ������豸�����豸���������ϵ�");

		cMyDlg->Func_Enable(TRUE);
		return false;
	}
	memset(recv_buffer, 0, 1024);

	//�����ʼ��

	cMyDlg->SetDlgItemText(IDC_Persent,"%0");

	m_persent = 0;

	//��ת�������ݱ���Ϊ�ַ�����ʽ

	CString STR1 = _T("");

	STR2 = _T("");

	unsigned char trs_buffer[LEN_BUF_STR] = { 0 };

	if (cMyDlg->check_PROM == TRUE)
	{
		for (int h = 0; h < ADD_LEN_PROM; h += LEN_BUF_STR)
		{
			if ((cMyDlg->m_ctrl_check_RamEncEn.GetCheck()) && (cMyDlg->m_combox_form.GetCurSel() != FILE_RCF_ENC))//��������
			{
				memcpy(&trs_buffer[0], &PROM_Enc[h], LEN_BUF_STR);
			}
			else memcpy(&trs_buffer[0], &buffer_PROM[h], LEN_BUF_STR);
			STR1 = transResult(trs_buffer, LEN_BUF_STR);
			STR2 += STR1;

			memset(trs_buffer, 0, LEN_BUF_STR);
		}
	}
	else if (cMyDlg->check_ATMC == TRUE)
	{
		for (int h = 0; h<ADD_LEN_ATMC; h += LEN_BUF_STR)
		{
			if ((cMyDlg->m_ctrl_check_RamEncEn.GetCheck()) && (cMyDlg->m_combox_form.GetCurSel() != FILE_RCF_ENC))
			{
				memcpy(&trs_buffer[0], &ATMC_Enc[h], LEN_BUF_STR);
			}
			else
			{
				memcpy(&trs_buffer[0], &buffer_ATMC[h+0x28000], LEN_BUF_STR);
			}
			STR1 = transResult(trs_buffer, LEN_BUF_STR);
			STR2 += STR1;

			memset(trs_buffer,0,LEN_BUF_STR);
		}
	}
	else
	{
		for (int h = 0; h<ADD_LEN_TROM; h += LEN_BUF_STR)
		{
			if ((cMyDlg->m_ctrl_check_RamEncEn.GetCheck()) && (cMyDlg->m_combox_form.GetCurSel() != FILE_RCF_ENC))
			{
				memcpy(&trs_buffer[0], &TROM_Enc[h], LEN_BUF_STR);
			}
			else
			{
				memcpy(&trs_buffer[0], &buffer_TROM[h+0x40000], LEN_BUF_STR);
			}
			STR1 = transResult(trs_buffer, LEN_BUF_STR);
			STR2 += STR1;

			memset(trs_buffer,0,LEN_BUF_STR);
		}
	}

	CString ram_strrun;

	if (cMyDlg->check_PROM == TRUE)
	{
		while (m_all<ADD_LEN_PROM)//��������
		{
			for (m_2 = 0; m_2<SEND_SIZE; m_2++)//64������һ��
			{
				if ((cMyDlg->m_ctrl_check_RamEncEn.GetCheck()) && (cMyDlg->m_combox_form.GetCurSel() != FILE_RCF_ENC)) send_buffer[m_2] = PROM_Enc[m_all];
				else send_buffer[m_2] = buffer_PROM[m_all];
				m_all++;
			}

			USBBulkWriteData(0, EP1_OUT, (char*)send_buffer, SEND_SIZE, 500);

			for (int m = 0; m<50; m++)
			{
				USBBulkReadData(0, EP1_IN, (char*)recv_buffer, EP1_IN_SIZE, 500);

				if (recv_buffer[3] != 0x10)
				{
					::Sleep(1);
					continue;
				}
				else break;
			}

			memset(send_buffer, 0, SEND_SIZE);

			ram_strrun.Format("%d", m_all / 1024);
			cMyDlg->SetDlgItemText(IDC_STATUES, "����������...�ѷ���" + ram_strrun + "K(��160K)");

			m_persent++;
			persent.Format("%.0f%%", 100.0*(float)(m_persent - 0) / (float)(160 - 0));//��ʾ���ȱ�
			cMyDlg->GetDlgItem(IDC_Persent)->SetWindowText(persent);
		}
	}
	else if(cMyDlg->check_ATMC == TRUE)
	{
		while (m_all<ADD_LEN_ATMC)//��������
		{

			for (m_2 = 0; m_2<SEND_SIZE; m_2++)//64������һ��
			{
				if ((cMyDlg->m_ctrl_check_RamEncEn.GetCheck()) && (cMyDlg->m_combox_form.GetCurSel() != FILE_RCF_ENC)) send_buffer[m_2] = ATMC_Enc[m_all];
				else send_buffer[m_2] = buffer_ATMC[m_all+0x28000];
				m_all++;
			}

			USBBulkWriteData(0, EP1_OUT, (char*)send_buffer, SEND_SIZE, 500);

			for (int m = 0; m<50; m++)
			{
				USBBulkReadData(0, EP1_IN, (char*)recv_buffer, EP1_IN_SIZE, 500);

				if (recv_buffer[3] != 0x10)
				{
					::Sleep(1);
					continue;
				}
				else break;
			}

			memset(send_buffer, 0, SEND_SIZE);

			ram_strrun.Format("%d", m_all / 1024);
			cMyDlg->SetDlgItemText(IDC_STATUES, "����������...�ѷ���" + ram_strrun + "K(��32K)");

			m_persent++;
			persent.Format("%.0f%%", 100.0*(float)(m_persent - 0) / (float)(32 - 0));//��ʾ���ȱ�
			cMyDlg->GetDlgItem(IDC_Persent)->SetWindowText(persent);
		}
	}
	else
	{
		while (m_all<ADD_LEN_TROM)//��������
		{

			for (m_2 = 0; m_2<SEND_SIZE; m_2++)//64������һ��
			{
				if ((cMyDlg->m_ctrl_check_RamEncEn.GetCheck()) && (cMyDlg->m_combox_form.GetCurSel() != FILE_RCF_ENC)) send_buffer[m_2] = TROM_Enc[m_all];
				else send_buffer[m_2] = buffer_TROM[m_all+0x40000];
				m_all++;
			}

			USBBulkWriteData(0, EP1_OUT, (char*)send_buffer, SEND_SIZE, 500);

			for (int m = 0; m<50; m++)
			{
				USBBulkReadData(0, EP1_IN, (char*)recv_buffer, EP1_IN_SIZE, 500);

				if (recv_buffer[3] != 0x10)
				{
					::Sleep(1);
					continue;
				}
				else break;
			}

			memset(send_buffer, 0, SEND_SIZE);

			ram_strrun.Format("%d", m_all / 1024);
			cMyDlg->SetDlgItemText(IDC_STATUES, "����������...�ѷ���" + ram_strrun + "K(��32K)");

			m_persent++;
			persent.Format("%.0f%%", 100.0*(float)(m_persent - 0) / (float)(32 - 0));//��ʾ���ȱ�
			cMyDlg->GetDlgItem(IDC_Persent)->SetWindowText(persent);
		}
	}

	cMyDlg->SetDlgItemText(IDC_STATUES,"У���ѷ�������...");
	long check_t = 0;

	while(check_t<1000)
	{

		USBBulkReadData(0,EP1_IN,(char*)recv_buffer,EP1_IN_SIZE,check_t);

		if(recv_buffer[3] == 0x10)
		{
			cMyDlg->MessageBox("дRAM�ɹ���","RAM����",MB_ICONINFORMATION);

			cMyDlg->SetDlgItemText(IDC_STATUES,"дRAM���...");
			memset(recv_buffer,0,1024);
			break;
		}
		else if(recv_buffer[3] == 0xFF)
		{
			cMyDlg->MessageBox("дRAMʧ��!�����豸�����豸���������ϵ�","ERROR��", MB_ICONEXCLAMATION);
			cMyDlg->SetDlgItemText(IDC_STATUES,"���ݶ�ȡУ��ʧ�ܣ������豸�����豸���������ϵ�");
			memset(recv_buffer,0,1024);

			cMyDlg->Func_Enable(TRUE);
			return false;
		}
		else continue;

		check_t += 10; 
	}

	//����������ʾ

	cMyDlg->m_ctrl_fpgasend.SetWindowText(_T(""));//��ʾ��������
	cMyDlg->m_ctrl_fpgasend.ReplaceSel(STR2);

	CString str_len_data;//��ط������ݳ���
	str_len_data.Format(_T("%1d"),cMyDlg->m_ctrl_fpgasend.GetTextLength()/3);
	cMyDlg->m_ctrl_static_fpga_send.SetWindowText("Send:"+str_len_data+"Byte");

	memset(PROM_Enc,0, ADD_LEN_PROM);
	memset(ATMC_Enc,0,ADD_LEN_ATMC);
	memset(TROM_Enc,0,ADD_LEN_TROM);

	cMyDlg->Func_Enable(TRUE);

	//spw201703 auto download alglib changed by ghan 3.30.2017
	//if (cMyDlg->m_combox_ram_selenc.GetCurSel() == MODE_HY328A_SPW_201703)
	//{
		//if ((cMyDlg->m_combox_form.GetCurSel() == FILE_RCF_ENC) && (cMyDlg->check_PROM == TRUE))//RCF file
		//{
			//CButton* m_radio_atmc = (CButton*)GetDlgItem(IDC_RADIO_ATMC);
			//m_radio_atmc.SetCheck(TRUE);
		//	cMyDlg->OnBnClickedButtonRam();
		//}
		//else if ((cMyDlg->m_combox_form.GetCurSel() == FILE_HEX_STAND_NOENC) && (cMyDlg->check_PROM == TRUE))//standard file
		//{
			//cMyDlg->CButton*GetDlgItem(IDC_RADIO_ATMC)->SetCheck(TRUE);
		//	cMyDlg->OnBnClickedButtonRam();
		//}
	//}

	return true;
}

void CFPGA_Memory_SetDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);   //���ڻ���ı����е�����;

	if(STR2 == _T(""))
	{
		MessageBox("δ���յ�����","ERROR��", MB_ICONEXCLAMATION);
		return;
	}

	CString strFilter,fileName,strText;
	strFilter="Hex Files(*.hex)|*.hex|Text Files(*.txt)|*.txt||";
	CFileDialog dlg(FALSE,"txt", NULL, OFN_EXPLORER|OFN_HIDEREADONLY|
		OFN_ENABLESIZING|OFN_FILEMUSTEXIST,strFilter);
	if(dlg.DoModal() == IDOK )								//��ʾ�����ļ��Ի���
	{
		fileName+=dlg.GetPathName();
		CFile savefile(fileName,CFile::modeCreate|CFile::modeWrite);		//����CFile����
		savefile.Write(STR2,STR2.GetLength());							//д�ļ�����
		savefile.Close();										//�ر��ļ�
	}
}

void CFPGA_Memory_SetDlg::OnBnClickedRadioProm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	check_PROM = TRUE;
	check_TROM = FALSE;
	check_ATMC = FALSE;	
}
void CFPGA_Memory_SetDlg::OnBnClickedRadioAtmc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	check_PROM = FALSE;
	check_TROM = FALSE;
	check_ATMC = TRUE;
}

void CFPGA_Memory_SetDlg::OnBnClickedRadioTrom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	check_PROM = FALSE;
	check_TROM = TRUE;
	check_ATMC = FALSE;
}

CString s_Readdata;
unsigned char memory[64] = {0};

void CFPGA_Memory_SetDlg::OnBnClickedButtonRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	DWORD ThreadID;
	DWORD code;
	
	UpdateData(TRUE);

	if (check_PROM == TRUE)
	{
		memory[0] = 0x0A;
		if (m_Blen > 0xA0 || m_Blen < 1)
		{
			MessageBox("��ȡPROM���ݴ�С������Χ��1~160K��", "ERROR!", MB_ICONEXCLAMATION);
			return;
		}
	}
	else if(check_ATMC == TRUE)
	{
		memory[0] = 0x0B;
		if (m_Blen > 0x20 || m_Blen < 1)
		{
			MessageBox("��ȡ�㷨�����ݴ�С������Χ��1~32K��", "ERROR!", MB_ICONEXCLAMATION);
			return;
		}
	}
	else
	{
		memory[0] = 0x0C;
		if (m_Blen > 0x20 || m_Blen < 1)
		{
			MessageBox("��ȡTROM���ݴ�С������Χ��1~32K��", "ERROR!", MB_ICONEXCLAMATION);
			return;
		}
	}

	
	memory[1] = (m_Blen & 0xFF00) >> 8;
	memory[2] = (m_Blen & 0x00FF);

	Func_Enable(FALSE);

	if(!GetExitCodeThread(hThread2,&code)||(code!=STILL_ACTIVE))
	{
		hThread2=CreateThread(NULL,0,ThreadFun2,this,0,&ThreadID);//��������ʼ�߳�
	}
}

DWORD WINAPI ThreadFun2(LPVOID pArg2)						//�߳���ں���
{
	CFPGA_Memory_SetDlg* cMyDlg2 = (CFPGA_Memory_SetDlg*)pArg2;

	s_Readdata = _T("");

	unsigned char recv_buffer_PROM[ADD_LEN_PROM] = {0}, recv_buffer_ATMC[ADD_LEN_ATMC] = { 0 }, recv_buffer_TROM[ADD_LEN_TROM] = { 0 },buffer[128] = {0};

	int m_1 = 0;
	CString str_m_1,str_trs = _T("");

	cMyDlg2->SetDlgItemText(IDC_STATUES,"��ȡ������...");

	USBBulkWriteData(0,EP1_OUT,(char*)memory,EP1_OUT_SIZE,0);

	cMyDlg2->Func_Enable(FALSE);

	if (cMyDlg2->check_PROM == TRUE)
	{
		switch (cMyDlg2->m_Blen / 50)
		{
		case 0:USBBulkReadData(0, EP1_IN, (char*)recv_buffer_PROM,cMyDlg2->m_Blen * 1024,500); break;//���ݳ���<50
		case 1://���ݳ��� >= 50
		{
			USBBulkReadData(0, EP1_IN, (char*)recv_buffer_PROM,50 * 1024,500);
			USBBulkReadData(0, EP1_IN, (char*)&recv_buffer_PROM[50 * 1024],(cMyDlg2->m_Blen - 50) * 1024,500);
			break;
		}
		case 2://���ݳ��� >= 100
		{
			USBBulkReadData(0, EP1_IN, (char*)recv_buffer_PROM, 50 * 1024,700);
			USBBulkReadData(0, EP1_IN, (char*)&recv_buffer_PROM[50 * 1024],50 * 1024,700);
			USBBulkReadData(0, EP1_IN, (char*)&recv_buffer_PROM[100 * 1024],(cMyDlg2->m_Blen - 100) * 1024,700);
			break;
		}
		case 3://���ݳ��� >= 150
		{
			USBBulkReadData(0, EP1_IN, (char*)recv_buffer_PROM, 50 * 1024,700);
			USBBulkReadData(0, EP1_IN, (char*)&recv_buffer_PROM[50 * 1024], 50 * 1024,700);
			USBBulkReadData(0, EP1_IN, (char*)&recv_buffer_PROM[100 * 1024], 50 * 1024,700);
			USBBulkReadData(0, EP1_IN, (char*)&recv_buffer_PROM[150 * 1024],(cMyDlg2->m_Blen - 150) * 1024,700);
			break;
		}
		}

		while (m_1<(cMyDlg2->m_Blen) * 1024)
		{
			str_trs = transResult(&recv_buffer_PROM[m_1], 64);

			s_Readdata += str_trs;

			m_1 += 64;
		}

		memset(recv_buffer_PROM, 0, ADD_LEN_PROM);
	}
	else if (cMyDlg2->check_ATMC == TRUE)
	{
		USBBulkReadData(0, EP1_IN, (char*)recv_buffer_ATMC, cMyDlg2->m_Blen * 1024, 2000);

		while (m_1<(cMyDlg2->m_Blen) * 1024)
		{
			str_trs = transResult(&recv_buffer_ATMC[m_1], 64);

			s_Readdata += str_trs;

			m_1 += 64;
		}

		memset(recv_buffer_ATMC, 0, ADD_LEN_ATMC);
	}
	else
	{
		USBBulkReadData(0, EP1_IN, (char*)recv_buffer_TROM, cMyDlg2->m_Blen * 1024, 4000);

		while (m_1<(cMyDlg2->m_Blen) * 1024)
		{
			str_trs = transResult(&recv_buffer_TROM[m_1], 64);

			s_Readdata += str_trs;

			m_1 += 64;
		}

		memset(recv_buffer_TROM, 0, ADD_LEN_TROM);
	}

	cMyDlg2->m_ctrl_fpgaread.SetWindowText(_T(""));

	cMyDlg2->m_ctrl_fpgaread.ReplaceSel(s_Readdata);

	str_m_1.Format(_T("%1d"),cMyDlg2->m_ctrl_fpgaread.GetTextLength()/3);

	cMyDlg2->m_ctrl_static_fpga_read.SetWindowText("Read:"+str_m_1+"Byte");

	str_m_1.Format("%d", cMyDlg2->m_Blen);

	cMyDlg2->MessageBox("�Ѷ�ȡ"+str_m_1+"K����","RAM����",MB_ICONINFORMATION);
	cMyDlg2->SetDlgItemText(IDC_STATUES,"��ȡ������ɣ�");

	cMyDlg2->Func_Enable(TRUE);

	return true;
		
}

void CFPGA_Memory_SetDlg::OnBnClickedButtonSaver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);   //���ڻ���ı����е�����;

	if(s_Readdata == _T("")) 
	{
		MessageBox("δ���յ����ݣ�","ERROR��", MB_ICONEXCLAMATION);
		return;
	}

	CString strFilter,fileName,strText;
	strFilter="Hex Files(*.hex)|*.hex|Text Files(*.txt)|*.txt||";
	CFileDialog dlg(FALSE,"txt", NULL, OFN_EXPLORER|OFN_HIDEREADONLY|
		OFN_ENABLESIZING|OFN_FILEMUSTEXIST,strFilter);
	if(dlg.DoModal() == IDOK )								//��ʾ�����ļ��Ի���
	{
		fileName+=dlg.GetPathName();
		CFile savefile(fileName,CFile::modeCreate|CFile::modeWrite);		//����CFile����
		savefile.Write(s_Readdata,s_Readdata.GetLength());							//д�ļ�����|
		savefile.Close();										//�ر��ļ�
	}
}

void CFPGA_Memory_SetDlg::OnBnClickedButtonSaveEnc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);   //���ڻ���ı����е�����;

	if(STR_ENC == _T(""))
	{
		MessageBox("��������Ϊ��","ERROR��", MB_ICONEXCLAMATION);
		return;
	}

	CString strFilter,fileName,strText;
	strFilter="Hex Files(*.hex)|*.hex|Text Files(*.txt)|*.txt||";
	CFileDialog dlg(FALSE,"txt", NULL, OFN_EXPLORER|OFN_HIDEREADONLY|
		OFN_ENABLESIZING|OFN_FILEMUSTEXIST,strFilter);
	if(dlg.DoModal() == IDOK )								//��ʾ�����ļ��Ի���
	{
		fileName+=dlg.GetPathName();
		CFile savefile(fileName,CFile::modeCreate|CFile::modeWrite);		//����CFile����
		savefile.Write(STR_ENC,STR_ENC.GetLength());							//д�ļ�����
		savefile.Close();										//�ر��ļ�
	}
}

void CFPGA_Memory_SetDlg::OnBnClickedButtonClrSendedit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrl_fpgasend.SetWindowText(_T(""));

	if (check_PROM == TRUE) memset(buffer_PROM, 0, ADD_LEN_PROM);
	else if (check_ATMC == TRUE) memset(buffer_ATMC, 0, ADD_LEN_ATMC);
	else memset(buffer_TROM, 0, ADD_LEN_TROM);
		
	m_ctrl_static_fpga_send.SetWindowText("Send:0Byte");
	STR2 = _T("");
}

void CFPGA_Memory_SetDlg::OnBnClickedButtonClrReadedit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrl_fpgaread.SetWindowText(_T(""));
	m_ctrl_static_fpga_read.SetWindowText("Read:0Byte");

	s_Readdata = _T("");
}

void CFPGA_Memory_SetDlg::Func_Enable(bool bEnable)
{
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_RADIO_PROM),bEnable);
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_RADIO_ATMC),bEnable);
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_RADIO_TROM),bEnable);
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_COMBO_FORM),bEnable);
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_BUTTON_SAVE_ENC),bEnable);
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_BUTTON_RAM),bEnable);
	::EnableWindow(::GetDlgItem(this->GetSafeHwnd(),IDC_BUTTON_READ),bEnable);

}


void CFPGA_Memory_SetDlg::OnCbnSelchangeComboForm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_combox_form.GetCurSel() == FILE_RCF_ENC)
	{
		m_ctrl_check_RamEncEn.SetCheck(false);
		m_ctrl_check_RamEncEn.EnableWindow(false);
	}
	else
	{
		m_ctrl_check_RamEncEn.SetCheck(true);
		m_ctrl_check_RamEncEn.EnableWindow(true);
	}
}


void CFPGA_Memory_SetDlg::OnCbnSelchangeComboRamSelenc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
