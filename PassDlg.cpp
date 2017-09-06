// PassDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "PassDlg.h"
#include "afxdialogex.h"
#include "libzkfpDemoDlg.h"
#include "LogWriter.h"


// CPassDlg 对话框

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


// CPassDlg 消息处理程序


void CPassDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CPassDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CPassDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CPassDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	auto *pBOk = static_cast<CPassDlg*>(GetDlgItem(IDOK));
	CString strname;
	GetDlgItemTextA(IDC_EDITname, strname);
	CString strpass;
	GetDlgItemTextA(IDC_EDITpass, strpass);
	if ("q" == strname & "1" == strpass)
	{
		ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
		pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("欢迎！管理员登录成功！"));
		CDialogEx::OnOK();
	}
	else
	{
		AfxMessageBox(_T("用户名或密码错误"));

	}
}


void CPassDlg::OnBnClickedFinger()
{
	// TODO:  在此添加控件通知处理程序代码
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	CString strMach;
	pMain->GetDlgItemTextA(IDC_EDIT_RESULT, strMach);
	if ("识别成功，管理人员"== strMach)
	{
		pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("欢迎！管理员登录成功！"));
		CLogWriter logWriter;
		CString strWriter;
		CString strSetItem = _T("登录管理界面");
		strWriter.Format("%12s——%s", pMain->m_strIdtifyName, strSetItem); //使输出到log对齐，少的地方补空格
		logWriter.WriteString(strWriter);
		CDialog::OnOK();
	}
	if ("识别成功，普通用户" == strMach)
	{
		pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("欢迎！用户登录成功！"));
		CLogWriter logWriter;
		CString strWriter;
		CString strSetItem = _T("登录用户界面");
		strWriter.Format("%12s——%s", pMain->m_strIdtifyName, strSetItem);//使输出到log对齐，少的地方补空格
		logWriter.WriteString(strWriter);
		CDialog::OnOK();
	}
}