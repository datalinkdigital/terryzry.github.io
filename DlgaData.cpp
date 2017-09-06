// DlgaData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "DlgaData.h"
#include "afxdialogex.h"
#include "WinBase.h"
#include "base64.h"


// CDlgaData �Ի���

IMPLEMENT_DYNAMIC(CDlgaData, CDialogEx)

CDlgaData::CDlgaData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgaData::IDD, pParent)
{

}

CDlgaData::~CDlgaData()
{
}

void CDlgaData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgaData, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgaData::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUT_SAVE, &CDlgaData::OnBnClickedButSave)
END_MESSAGE_MAP()


// CDlgaData ��Ϣ�������


void CDlgaData::OnBnClickedButton1()
{
	BOOL isOpen = TRUE;     //�Ƿ��(����Ϊ����)  
	CString defaultDir = "d:\\finger";   //Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = "";         //Ĭ�ϴ򿪵��ļ���  
	CString filter = "�ļ� (*.uf; )|*.uf;";   //�ļ����ǵ�����  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = "E:\\FileTest\\test.uf";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath ;
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
		CStdioFile stdfile;
		CString strText, strLine, strOut , strNow;
		stdfile.Open(filePath, CFile::modeReadWrite);
		while (stdfile.ReadString(strLine))
		{
			std::string decoded = base64_decode(static_cast<std::string>(strLine));
			strText = decoded.c_str();
			strOut += strText + "\n";
		}
		GetDlgItem(IDC_RICHEDIT21)->SetWindowText(strOut);
	}
	SetDlgItemTextA(IDC_EDIT_PATHNAME, filePath);
}


void CDlgaData::OnBnClickedButSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strEditTxt, strPathName;
	GetDlgItemTextA(IDC_RICHEDIT21, strEditTxt);
	std::string strStl;
	strStl = strEditTxt.GetBuffer(0);
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(strStl.c_str()), strStl.length());
	strEditTxt = encoded.c_str();
	GetDlgItemTextA(IDC_EDIT_PATHNAME, strPathName);
	CStdioFile stdfile;
	stdfile.Open(strPathName , CFile::modeReadWrite);
	stdfile.Write(strEditTxt, strlen(strEditTxt));
}
