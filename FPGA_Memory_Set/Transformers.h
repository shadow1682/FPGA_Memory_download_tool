// Transformers.h : Transformers DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTransformersApp
// 有关此类实现的信息，请参阅 Transformers.cpp
//

class CTransformersApp : public CWinApp
{
public:
	CTransformersApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/****************************************************************
函数功能：10进制---->16进制
类型转化：unsigned int(Dec)->CString(Hex)
参数取值范围：
          HEX：0x00000000-0xFFFFFFFF
		  DEC：0-4294967295
*****************************************************************/
CString __stdcall DecimalToHex(int iDecimal);//0-4294967295



/****************************************************************
函数功能：2进制---->10进制
类型转化：CString(Bin)->CString(Dec)
参数取值范围：
          BIN：0-111111111111111111111111111111111
		  DEC：0-4294967295
*****************************************************************/
CString __stdcall BinToDec(CString strBin);



/****************************************************************
函数功能：2进制---->16进制
类型转化：CString(Bin)->CString(Hex)
参数取值范围：
          BIN：0-111111111111111111111111111111111
		  HEX：0-4294967295
*****************************************************************/
CString __stdcall BinToHex(CString strBin);



/****************************************************************
函数功能：16进制---->10进制
类型转化：CString(Hex)->CString(Dec)
参数取值范围：
          HEX：0x00000000-0xFFFFFFFF
		  DEC：0-4294967295
*****************************************************************/
CString __stdcall HexToDec(CString strHex);



/****************************************************************
函数功能：16进制---->2进制
类型转化：CString(Hex)->CString(Bin)
参数取值范围：
          HEX：0x00000000-0xFFFFFFFF
		  BIN：0-111111111111111111111111111111111
*****************************************************************/
CString __stdcall HexToBin(CString strHex);



/****************************************************************
函数功能：10进制---->2进制
类型转化：CString(Dec)->CString(Bin)
参数取值范围：
          BIN：0-111111111111111111111111111111111
		  DEC：0-4294967295
*****************************************************************/
CString __stdcall DecToBin(CString strDec);



int __stdcall hexToint(unsigned char sHex[],int *iInt);
unsigned char* __stdcall intTohex(int iData,unsigned char* sHex);


CString __stdcall transResult(unsigned char* Response_buffer,int buffer_len);//将十六进制数据转换为数据间带空格的CString数据

int __stdcall transCStringbuffer(CString strbuffer,unsigned char *ucbuffer);//将获取的CString数据转换为unsigned char十六进制类型