
#ifndef __MD5_INCLUDED__
#define __MD5_INCLUDED__


//MD5ժҪֵ�ṹ��
typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} MD5VAL;

//�����ַ�����MD5ֵ(����ָ���������ɺ�������)
MD5VAL md5(char * str, unsigned int size = 0);
//MD5�ļ�ժҪ
MD5VAL md5File(FILE * fpin);

//ֱ�Ӽ����ļ�MD5
CStringA GetFileMd5(CString fname);

//�ַ���md5����
void GetStrMd5(char * input, char * output);

#endif