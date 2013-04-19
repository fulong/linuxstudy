/*******************************************************
*	这是配合我的博客《JRTPLIB@Conference DIY视频会议系统》
*	而写的一个阶段性实验。
*	作者：冯富秋 tinnal
*	版本：1.00
*	修改：2013-04-19
*	修改人：  莫福龙
*********************************************************/

#ifndef _WAV_H_
#define _WAV_H_

#include "types.h"
#include "stdlib.h"
#pragma pack(1)

struct RIFF_HEADER
{
	U8		szRiffID[4];  // 'R','I','F','F'
	U32		dwRiffSize;	  //除了这八个字节文件的总大小
	U8		szRiffFormat[4]; // 'W','A','V','E'
};

struct WAVE_FORMAT
{
	U16		wFormatTag;//编码的方式。1--pcm     6--a-raw
	U16		wChannels;//声道数目
	U32		dwSamplesPerSec;//采样频率
	U32		dwAvgBytesPerSec;//每秒所需要的字节数，数据量
	U16		wBlockAlign;//数据的量化水平，即数据对齐字节数
	U16		wBitsPerSample;//每一位采样的位数
	U16		pack;		//附加信息
};
struct FMT_BLOCK
{
	U8		szFmtID[4]; // 'f','m','t',' '
	U32		dwFmtSize;  //这个标签的大小
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
	U32		dwDataSize;//除文件头的以外的大少，即数据大小
};

extern FILE* openWav();
#endif