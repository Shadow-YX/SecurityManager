// CAddVirus.cpp: 实现文件
//
#include"stdafx.h"

#include "MyTask.h"
#include "CAddVirus.h"
#include "afxdialogex.h"
#include"MyHead.h"
#include "MD5_FILE.h"
#include <fstream>
using namespace std;

// CAddVirus 对话框

IMPLEMENT_DYNAMIC(CAddVirus, CDialogEx)

CAddVirus::CAddVirus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_ADDVIRUS, pParent)
{

}

CAddVirus::~CAddVirus()
{
}

void CAddVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddVirus, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddVirus::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddVirus 消息处理程序


void CAddVirus::OnBnClickedOk()
{
	CString nFileName;
	CString nFilePath;
	CString nFileMd5;

	GetDlgItemText(IDC_MFCEDITBROWSE1, nFilePath); //获取选中文件路径
	//AfxMessageBox(nFilePath);  //病毒验证路径

	nFileMd5 = GetFileMd5(nFilePath);//获取上传病毒的md5
	//nFileName = nFilePath.Mid(nFilePath.ReverseFind('\\ ') + 1);//获取上传病毒的文件名
	nFileName = nFilePath.Right(nFilePath.GetLength() - nFilePath.ReverseFind('\\') - 1);//获取上传病毒的文件名

	const char* FileName;
	char temp[260] ;
	for (int i = 0; i < 260; i++) temp[i] = -2; //初始化缓冲区
	
	::wsprintfA(temp, "%ls", (LPCTSTR)nFileName); 
	FileName = temp;   //格式转换
	int SizeName = strlen(FileName);

	const char* FileMd5;
	char temp2[260];
	for (int i = 0; i < 260; i++) temp2[i] = -2;

	::wsprintfA(temp2, "%ls", (LPCTSTR)nFileMd5);
	FileMd5 = temp2;
	int SizeMd5 = strlen(FileMd5);

	ofstream f;
	f.open("VirusLib.dat", ios::app | ios::binary); //打开病毒库末尾追加信息

	f.write((char*)FileName, 260);//把病毒名称写入病毒库
	f.write((char*)FileMd5,260);//把病毒md5写入病毒库

	f.close();

	AfxMessageBox(_T("病毒已经成功上传至本地病毒库!!"));  //验证路径

	/*
	char FileName[256] = {NULL};
	strcpy(FileName, (LPSTR)(LPCTSTR)nFileName);
	//strncpy(FileName, (LPCTSTR)nFileName, sizeof(FileName));

	char FileMd5[256] = { NULL };
	strcpy(FileMd5, (LPSTR)(LPCTSTR)nFileMd5);
	FILE* fp = NULL;
	char* p = NULL;
	p=FileName;
	if ((fp = fopen("VirusLib.dat", "a+")) == NULL)		    	//打开这个二进制文件，a+方式
	{
		printf("打开文件失败！\n");
		exit(EXIT_SUCCESS);
	}
	fwrite(p, sizeof(FileName), 1, fp);													   	//把有效数据插入数组
	fclose(fp);
	fp = NULL;
	delete fp;


	FILE* fp1 = NULL;
	char* p1 = NULL;
	p1 = FileMd5;
	if ((fp1 = fopen("VirusLib.dat", "a+")) == NULL)		    	//打开这个二进制文件，a+方式
	{
		printf("打开文件失败！\n");
		exit(EXIT_SUCCESS);
	}
	fwrite(p1, sizeof(FileMd5), 1, fp1);													   	//把有效数据插入数组
	fclose(fp1);
	fp1 = NULL;
	delete fp1;

	*/
	/*
	ofstream f;
	f.open("VirusLib.dat", ios::app | ios::binary);

	f.write((char*)FileName, SizeName+1);//把病毒名称写入病毒库
	f.write((char*)FileMd5, SizeMd5+1);//把病毒md5写入病毒库

	f.close();
	8=*/
	/*
	//强转转换
	const char* cstr;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)nFilePath);
	cstr = temp;
	FILE* pFile = nullptr;
		pFile=fopen(cstr, "r");

	//DWORD nCount = m_List_VirusLib.GetItemCount();
	//PVIRUSINFO nVirusInfo = new VIRUSINFO[nCount];

	DWORD nSize = GetFileSize((PHANDLE)pFile, 0);
	DWORD nReadSize = 0;

	nFileMd5 = GetFileMd5(nFilePath);//获取上传病毒的md5

	//fwrite(nVIRUSINFO, sizeof(VIRUSINFO), nCount, pFile);
	fclose(pFile);

	CDialogEx::OnOK();
	*/
}
