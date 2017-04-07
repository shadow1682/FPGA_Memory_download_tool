// hyrf_dll.h : hyrf_dll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// Chyrf_dllApp
// 有关此类实现的信息，请参阅 hyrf_dll.cpp
//

#ifndef hyrf_EXPORTS
#define hyrf_API __declspec(dllexport)
#else
#define hyrf_API __declspec(dllimport)
#endif


class Chyrf_dllApp : public CWinApp
{
public:
	Chyrf_dllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#ifdef __cplusplus
extern "C" {
#endif

	int __stdcall UsbWrite(unsigned char* pchar,int number);//测试专用
	int __stdcall UsbRead_TEST(unsigned char* pchar,unsigned long W_time);//测试专用

	/***************************************************************功能函数********************************************************************/


	long __stdcall hy_Reader_Open(int port,long baud);
	long __stdcall hy_Reader_Close(long ReaderHandle);

	BOOL __stdcall hy_fuc_14443();//ISO14443
    BOOL __stdcall hy_fuc_15693();//ISO15693

	BOOL __stdcall hy_clearBuffertem();

	BOOL __stdcall hy_SeeData(unsigned char* SeeData_send,unsigned char* SeeData_receive,unsigned char* Seenumber);

	BOOL __stdcall hy_beep(long ReaderHandle,unsigned int _Btime);
	long __stdcall hy_getver(long ReaderHandle,unsigned char* Response);

	/***************************************************************以下为ISO14443命令函数******************************************************/
	//----------------------------------------------Common Command-------------------------------------------------
	long __stdcall hy_card(long ReaderHandle,unsigned char* UID);
	long __stdcall hy_request(long ReaderHandle,unsigned char* Response,unsigned long W_time);
	long __stdcall hy_RequestAll(long ReaderHandle,unsigned char* Response,unsigned long W_time);
	long __stdcall hy_reset(long ReaderHandle,unsigned long W_time);
	long __stdcall hy_anticoll(long ReaderHandle,unsigned char* Response,unsigned long W_time);
	long __stdcall hy_halt(long ReaderHandle,unsigned char* Response,unsigned long W_time);
	long __stdcall hy_select(long ReaderHandle,unsigned char* Response,unsigned long W_time);

	long __stdcall hy_DirectCmd(long ReaderHandle,	                                      
		                         unsigned char Lenth_of_DirectCmd_buffer,
		                         unsigned char* Response,
		                         unsigned char parity_CRC,
		                         unsigned char send_bits,
		                         unsigned char* Data_buffer,
		                         unsigned char receive_bits,
		                         unsigned char receive_bytes,
								 unsigned long W_time);
    //----------------------------------------------M1 Command------------------------------------------------------
	long __stdcall hy_Read(long ReaderHandle,unsigned char* Response,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Write4B(long ReaderHandle,unsigned char* Response,unsigned char* Write4B,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_SetKey(long ReaderHandle,unsigned char* Response,unsigned char Setkey,unsigned char* Keydata,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Authen(long ReaderHandle,unsigned char* Response,unsigned char _Adr,int choice_key,unsigned long W_time);

	long __stdcall hy_Increase(long ReaderHandle,unsigned char* Response,unsigned char* Increase,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Decrease(long ReaderHandle,unsigned char* Response,unsigned char* Decrease,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Restore(long ReaderHandle,unsigned char* Response,unsigned char* Restore,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Transfer(long ReaderHandle,unsigned char* Response,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Write16B(long ReaderHandle,unsigned char* Response,unsigned char* Write16B,unsigned char _Adr,unsigned long W_time);

	long __stdcall hy_Write4To16(long ReaderHandle,unsigned char* Response,unsigned char* Write4To16,unsigned char _Adr,unsigned char* Write4To16_1,unsigned long W_time);

	//----------------------------------------------(14443-4部分)------------------------------------------------------
	long __stdcall hy_pro_command(long ReaderHandle,unsigned char send_len,unsigned int* recv_len,unsigned char* send_buf,unsigned char* recv_buf);
	long __stdcall hy_pro_commandlink(long ReaderHandle,unsigned char send_len,unsigned char* send_buf,unsigned int* recv_len,unsigned char* recv_buf,unsigned char FG);
	long __stdcall hy_pro_halt(long ReaderHandle,unsigned char* recv_buf);
	long __stdcall hy_pro_reset(long ReaderHandle,unsigned char* len,unsigned char* recv_buf);
	long __stdcall hy_pro_commanddirect(long ReaderHandle,unsigned char send_len,unsigned char* send_buf,unsigned char* recv_len,unsigned char* recv_buf,unsigned char waittime);
	

	/***************************************************低频卡(EM4305\5200\4100\T5577)命令控制函数**********************************************/

	long __stdcall hy_em4100_read(unsigned char* Response);//未完成，未能成功接收数据，只能发送数据




	/*************************************************************接触式IC卡命令函数************************************************************/

	long __stdcall hy_SAMreset(long ReaderHandle,unsigned char* recv_len,unsigned char* recv_buf);
	long __stdcall hy_SAMapdudirect(long ReaderHandle,unsigned char send_len,unsigned char* send_buf,unsigned char* recv_len,unsigned char* recv_buf);
	long __stdcall hy_SAMsetSAM(long ReaderHandle,unsigned char SAMID);
	long __stdcall hy_SAMsetETU(long ReaderHandle,unsigned char* Response,unsigned char _etu);

	/*************************************************************以下为ISO15693命令函数********************************************************/
	
	long __stdcall hy_15693_DirectCmd(long ReaderHandle,
		                              long len,
		                              unsigned char* Response,
		                              unsigned char* CRC,
		                              unsigned char* Send_buffer);

	long __stdcall hy_15693_Inventory(long ReaderHandle,
		                              long len,
		                              unsigned char* Response,
		                              unsigned char CRC,
		                              unsigned char flag,
		                              unsigned char* Inventory_data);

	long __stdcall hy_15693_Stay_Quiet(long ReaderHandle,
		                               unsigned char* Response,
		                               unsigned char Stay_Quiet_CRC,
		                               unsigned char Stay_Quiet_flag,
		                               unsigned char* UID);

	long __stdcall hy_15693_Read_Single(long ReaderHandle,
		                                long Lenth_of_Read_Single_buffer,
		                                unsigned char* Response,
		                                unsigned char* Read_Single_CRC,
		                                unsigned char* Read_Single_flag,
		                                unsigned char* Read_Single_data,
		                                unsigned char* Read_Single_block_quantity);

	long __stdcall hy_15693_Reset(long ReaderHandle,unsigned char* Response);

	long __stdcall hy_15693_EOF(long ReaderHandle,unsigned char* Response);

	long __stdcall hy_15693_Write_Single(long ReaderHandle,
		                                 long Lenth_of_Write_Single_buffer,
		                                 unsigned char* Response,
		                                 unsigned char* Write_Single_CRC,
		                                 unsigned char* Write_Single_flag,
		                                 unsigned char* Write_Single_data,
		                                 unsigned char* Write_Single_block_quantity);

	long __stdcall hy_15693_Lock_Block(long ReaderHandle,
		                               long Lenth_of_Lock_Block_buffer,
		                               unsigned char* Response,
		                               unsigned char* Lock_Block_CRC,
		                               unsigned char* Lock_Block_flag,
		                               unsigned char* Lock_Block_data,
		                               unsigned char* Lock_Block_block_quantity);

	long __stdcall hy_15693_Read_Mulblock(long ReaderHandle,
		                                  long Lenth_of_Read_Mulblock_buffer,
		                                  unsigned char* Response,
		                                  unsigned char* Read_Mulblock_CRC,
		                                  unsigned char* Read_Mulblock_flag,
		                                  unsigned char* Read_Mulblock_data,
		                                  unsigned char* Read_Mulblock_block_quantity,
		                                  unsigned char* Read_Mulblock_block_number);

	long __stdcall hy_15693_Select_15693(long ReaderHandle,
		                                 long Lenth_of_Select_15693_buffer,
		                                 unsigned char* Response,
		                                 unsigned char* Select_15693_CRC,
		                                 unsigned char* Select_15693_flag,
		                                 unsigned char* Select_15693_data);

	long __stdcall hy_15693_Reset_to_Ready(long ReaderHandle,
		                                   long Lenth_of_Reset_to_Ready_buffer,
		                                   unsigned char* Response,
		                                   unsigned char* Reset_to_Ready_CRC,
		                                   unsigned char* Reset_to_Ready_flag,
		                                   unsigned char* Reset_to_Ready_data);

	long __stdcall hy_15693_Write_AFI(long ReaderHandle,
		                              long Lenth_of_Write_AFI_buffer,
		                              unsigned char* Response,
		                              unsigned char* Write_AFI_CRC,
		                              unsigned char* Write_AFI_flag,
		                              unsigned char* Write_AFI_data);

	long __stdcall hy_15693_Lock_AFI(long ReaderHandle,
		                             long Lenth_of_Lock_AFI_buffer,
		                             unsigned char* Response,
		                             unsigned char* Lock_AFI_CRC,
		                             unsigned char* Lock_AFI_flag,
		                             unsigned char* Lock_AFI_data);

	long __stdcall hy_15693_Write_DSFID(long ReaderHandle,
		                                long Lenth_of_Write_DSFID_buffer,
		                                unsigned char* Response,
		                                unsigned char* Write_DSFID_CRC,
		                                unsigned char* Write_DSFID_flag,
		                                unsigned char* Write_DSFID_data);

	long __stdcall hy_15693_Lock_DSFID(long ReaderHandle,
		                               long Lenth_of_Lock_DSFID_buffer,
		                               unsigned char* Response,
		                               unsigned char* Lock_DSFID_CRC,
		                               unsigned char* Lock_DSFID_flag,
		                               unsigned char* Lock_DSFID_data);

	long __stdcall hy_15693_Get_SystemInfo(long ReaderHandle,
		                                   long Lenth_of_Get_SystemInfo_buffer,
		                                   unsigned char* Response,
		                                   unsigned char* Get_SystemInfo_CRC,
		                                   unsigned char* Get_SystemInfo_flag,
		                                   unsigned char* Get_SystemInfo_data);

	long __stdcall hy_15693_Get_Mulblock_Security(long ReaderHandle,
		                                          long Lenth_of_Get_Mulblock_Security_buffer,
		                                          unsigned char* Response,
		                                          unsigned char* Get_Mulblock_Security_CRC,
		                                          unsigned char* Get_Mulblock_Security_flag,
		                                          unsigned char* Get_Mulblock_Security_data,
		                                          unsigned char* Get_Mulblock_Security_block_quantity,
		                                          unsigned char* Get_Mulblock_Security_block_number);

	long __stdcall hy_15693_Write_2_Blocks(long ReaderHandle,
		                                   long Lenth_of_Write_2_Blocks_buffer,
		                                   unsigned char* Response,
		                                   unsigned char* Write_2_Blocks_CRC,
		                                   unsigned char* Write_2_Blocks_flag,
		                                   unsigned char* Write_2_Blocks_data,
		                                   unsigned char* Write_2_Blocks_block_number);

	long __stdcall hy_15693_Lock_2_Blocks(long ReaderHandle,
		                                  long Lenth_of_Lock_2_Blocks_buffer,
		                                  unsigned char* Response,
		                                  unsigned char* Lock_2_Blocks_CRC,
		                                  unsigned char* Lock_2_Blocks_flag,
		                                  unsigned char* Lock_2_Blocks_data,
		                                  unsigned char* Lock_2_Blocks_block_number);

	long __stdcall hy_15693_Inventory_Read(long ReaderHandle,
		                                   long Lenth_of_Inventory_Read_buffer,
		                                   unsigned char* Response,
		                                   unsigned char* Inventory_Read_CRC,
		                                   unsigned char* Inventory_Read_flag,
		                                   unsigned char* Inventory_Read_data,
		                                   unsigned char* Inventory_Read_block_quantity,
		                                   unsigned char* Inventory_Read_block_number);

	long __stdcall hy_15693_Fast_Inventory_Read(long ReaderHandle,
		                                        long Lenth_of_Fast_Inventory_Read_buffer,
		                                        unsigned char* Response,
		                                        unsigned char* Fast_Inventory_Read_CRC,
		                                        unsigned char* Fast_Inventory_Read_flag,
		                                        unsigned char* Fast_Inventory_Read_data,
		                                        unsigned char* Fast_Inventory_Read_block_quantity,
		                                        unsigned char* Fast_Inventory_Read_block_number);

	long __stdcall hy_15693_Set_to_Test_State(long ReaderHandle,
		                                      unsigned char* Response,
		                                      unsigned char* Set_to_Test_State_CRC,
		                                      unsigned char* Set_to_Test_State_flag);

	long __stdcall hy_15693_Factory_Lock(long ReaderHandle,
		                                 unsigned char* Response,
		                                 unsigned char* Factory_Lock_CRC,
		                                 unsigned char* Factory_Lock_flag,
		                                 unsigned char* Factory_Lock_block_number);

	long __stdcall hy_15693_Write_all(long ReaderHandle,
		                              unsigned char* Response,
		                              unsigned char* Write_all_CRC,
		                              unsigned char* Write_all_flag,
		                              int choice_00_ff);
	long __stdcall hy_15693_EAS(long ReaderHandle,
		                        long Lenth_of_EAS_buffer,
		                        unsigned char* Response,
		                        unsigned char* EAS_CRC,
		                        unsigned char* EAS_flag,
		                        unsigned char* EAS_data,
		                        int choice);

#ifdef __cplusplus
}
#endif