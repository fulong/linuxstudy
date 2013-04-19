/*******************************************************
*	��������ҵĲ��͡�JRTPLIB@Conference DIY��Ƶ����ϵͳ��
*	��д��һ���׶���ʵ�顣
*	���ߣ��븻�� tinnal
*	�汾��1.00
*	�޸ģ�2013-04-19
*	�޸��ˣ�  Ī����
*********************************************************/

#ifndef _WAV_H_
#define _WAV_H_

#include "types.h"
#include "stdlib.h"
#pragma pack(1)

struct RIFF_HEADER
{
	U8		szRiffID[4];  // 'R','I','F','F'
	U32		dwRiffSize;	  //������˸��ֽ��ļ����ܴ�С
	U8		szRiffFormat[4]; // 'W','A','V','E'
};

struct WAVE_FORMAT
{
	U16		wFormatTag;//����ķ�ʽ��1--pcm     6--a-raw
	U16		wChannels;//������Ŀ
	U32		dwSamplesPerSec;//����Ƶ��
	U32		dwAvgBytesPerSec;//ÿ������Ҫ���ֽ�����������
	U16		wBlockAlign;//���ݵ�����ˮƽ�������ݶ����ֽ���
	U16		wBitsPerSample;//ÿһλ������λ��
	U16		pack;		//������Ϣ
};
struct FMT_BLOCK
{
	U8		szFmtID[4]; // 'f','m','t',' '
	U32		dwFmtSize;  //�����ǩ�Ĵ�С
	struct	WAVE_FORMAT wavFormat;
};

struct FACT_BLOCK
{
	U8		szFactID[4]; // 'f','a','c','t'
	U32		dwFactSize;
};

struct DATA_BLOCK
{
	U8		szDataID[4]; // 'd','a','t','a'
	U32		dwDataSize;//���ļ�ͷ������Ĵ��٣������ݴ�С
};

extern FILE* openWav();
#endif