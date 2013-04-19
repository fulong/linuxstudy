
#include "stdio.h"
#include "string.h"
#include "types.h"
#include "g711.h"
#include "wav.h"

struct RIFF_HEADER	riff_header;
struct FMT_BLOCK	fmt_block;
char   fack_block_buffer[20];		//20 should be enough
struct FACT_BLOCK	fact_block;
struct DATA_BLOCK	data_block;

int main(int argc, char **argv)
{
	FILE *wav_in;
	FILE *wav_out;
	U32 i;
	U8	has_fact_block =0;
	int head_with=58;//文件头的默认大小
	unsigned char pcm_bytes[2];
	short pcm;
	unsigned char a_law;	
	char* output_temp;
	long file_pos;

	if((argc != 3) && (argc != 4) )
	{
		printf("Usage:\n\t%s <intput file> <seconds> [output file]\n", argv[0]);
		exit(-1);
	}
	if(argc == 3)
	{
		argv[3]="a"
	}
	output_temp=argv[3];
	
	wav_in = openWav(argv[1]);
	if(wav_in == NULL)
	{
		printf("Can't open input file %s\n", argv[1]);
		return (-1);
	}

	wav_out = fopen(output_temp, "wb");
	if( wav_out == NULL)
	{
		printf("Can't open output file %s\n",argv[3]);
		fclose(wav_in);
		return(-1);
	}
	
	file_pos = ftell(wav_in);

	//Read RIFF_HEADER
	fread(&riff_header, sizeof(struct RIFF_HEADER), 1, wav_in);
	if(	memcmp(riff_header.szRiffID, "RIFF", 4) != 0 ||
		memcmp(riff_header.szRiffFormat, "WAVE", 4) != 0 )
	{
		printf("No a vaild wave file!\n");
		fclose(wav_in);
		fclose(wav_out);
		return(-1);
	}
	file_pos = ftell(wav_in);

	//Read FMT_BLOCK
	fread(&fmt_block, sizeof(struct FMT_BLOCK), 1, wav_in);
	if(	memcmp(fmt_block.szFmtID, "fmt ", 4) !=0 ||
		fmt_block.dwFmtSize != 18 ||
		fmt_block.wavFormat.wFormatTag != 0x1 ||
		fmt_block.wavFormat.wChannels != 0x1 ||
		fmt_block.wavFormat.dwSamplesPerSec != 8000 ||
		fmt_block.wavFormat.wBitsPerSample != 16)
	{
		printf("Sorry this is only test program,\n"
			"we only support follow format,\n"
			"\t 1. Format:        linear PCM \n"
			"\t 2. Samples Rate:  8000 KHz \n"
			"\t 3. Channels:      one channel \n"
			"\t 4. BitsPerSample: 16 \n");
	if(fmt_block.dwFmtSize != 16)
	{
		fclose(wav_in);
		fclose(wav_out);
		return(-1);	
	}
	head_with-=2;
	}
	
	//Try to read FACT_BLOCK
	file_pos = ftell(wav_in);
	fread(&fact_block, sizeof(struct FACT_BLOCK), 1, wav_in);
	if( memcmp(fact_block.szFactID, "fact", 4) == 0 )
	{	
		has_fact_block =1;
		fread(&fack_block_buffer, fact_block.dwFactSize, 1, wav_in);
	}
	else
	{
		head_with -= 12;
		fseek(wav_in, file_pos, SEEK_SET);
	}
	
	fread(&data_block, sizeof(struct DATA_BLOCK), 1, wav_in);
	if (memcmp(data_block.szDataID, "data", 4) != 0)
	{
		printf("OOh what error?\n");
		fclose(wav_in);
		fclose(wav_out);
		return(-1);
	}
	double time_sum=(data_block.dwDataSize-head_with)/fmt_block.wavFormat.dwAvgBytesPerSec;
	printf("%ld",time_sum);
	exit(0);
	//Change the wave header to write
	riff_header.dwRiffSize					-=	data_block.dwDataSize/2 ;
	
	fmt_block.wavFormat.wFormatTag			=	0x06;
	fmt_block.wavFormat.wChannels			=	0x01;
	fmt_block.wavFormat.dwSamplesPerSec		=	8000;
	fmt_block.wavFormat.dwAvgBytesPerSec	=	8000;
	fmt_block.wavFormat.wBlockAlign			=	0x01;
	fmt_block.wavFormat.wBitsPerSample		=	0x08;

	data_block.dwDataSize					-=	data_block.dwDataSize/2 ;

	//Write wave file header
	fwrite(&riff_header, sizeof(struct RIFF_HEADER), 1, wav_out);
	fwrite(&fmt_block, sizeof(struct FMT_BLOCK), 1, wav_out);
	if(has_fact_block == 1) 
	{
		fwrite(&fact_block, sizeof(struct FACT_BLOCK), 1, wav_out);
		fwrite(&fack_block_buffer, fact_block.dwFactSize, 1, wav_out);
	}
	fwrite(&data_block, sizeof(struct DATA_BLOCK), 1, wav_out);

	//Convert pcm data to a-low data  and  write wav file.
	for(i =0; i< data_block.dwDataSize; i++)
	{
		pcm_bytes[0] = (U8) fgetc(wav_in);
		pcm_bytes[1] = (U8) fgetc(wav_in);
		pcm = *(short *)&pcm_bytes;

		a_law	 = ALawEncode((int)pcm);
//		a_law	 = linear2alaw((int)pcm);
		fputc(a_law, wav_out);
	}
	fclose(wav_in);
	fclose(wav_out);
	
	printf("Finish!\n");
	return 0;
}
