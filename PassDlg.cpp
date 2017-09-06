// PassDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "PassDlg.h"
#include "afxdialogex.h"
#include "libzkfpDemoDlg.h"
#include "LogWriter.h"


// CPassDlg �Ի���

IMPLEMENT_DYNAMIC(CPassDlg, CDialogEx)

CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassDlg::IDD, pParent)
	, m_strRankPass(_T(""))
{

}

CPassDlg::~CPassDlg()
{
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDITname, &CPassDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDITpass, &CPassDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDCANCEL, &CPassDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPassDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK_FINGER, &CPassDlg::OnBnClickedFinger)
END_MESSAGE_MAP()


// CPassDlg ��Ϣ�������


void CPassDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPassDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPassDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CPassDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	auto *pBOk = static_cast<CPassDlg*>(GetDlgItem(IDOK));
	CString strname;
	GetDlgItemTextA(IDC_EDITname, strname);
	CString strpass;
	GetDlgItemTextA(IDC_EDITpass, strpass);
	if ("q" == strname & "1" == strpass)
	{
		ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
		pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("��ӭ������Ա��¼�ɹ���"));
		CDialogEx::OnOK();
	}
	else
	{
		AfxMessageBox(_T("�û������������"));

	}
}


void CPassDlg::OnBnClickedFinger()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	CString strMach;
	pMain->GetDlgItemTextA(IDC_EDIT_RESULT, strMach);
	if ("ʶ��ɹ���������Ա"== strMach)
	{
		pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("��ӭ������Ա��¼�ɹ���"));
		CLogWriter logWriter;
		CString strWriter;
		CString strSetItem = _T("��¼�������");
		strWriter.Format("%12s����%s", pMain->m_strIdtifyName, strSetItem); //ʹ�����log���룬�ٵĵط����ո�
		logWriter.WriteString(strWriter);
		CDialog::OnOK();
	}
	if ("ʶ��ɹ�����ͨ�û�" == strMach)
	{
		pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("��ӭ���û���¼�ɹ���"));
		CLogWriter logWriter;
		CString strWriter;
		CString strSetItem = _T("��¼�û�����");
		strWriter.Format("%12s����%s", pMain->m_strIdtifyName, strSetItem);//ʹ�����log���룬�ٵĵط����ո�
		logWriter.WriteString(strWriter);
		CDialog::OnOK();
	}
}