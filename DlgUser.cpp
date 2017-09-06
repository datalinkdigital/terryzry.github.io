// DlgUser.cpp : 实现文件
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "DlgUser.h"
#include "afxdialogex.h"
#include "libzkfpDemoDlg.h"
#include "LogWriter.h"


// CDlgUser 对话框

IMPLEMENT_DYNAMIC(CDlgUser, CDialogEx)

CDlgUser::CDlgUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgUser::IDD, pParent)
	, m_PointScreen(_T(""))
	, m_UINTTime(0)
{

	m_CountTime1 = 60;
}

CDlgUser::~CDlgUser()
{
}

void CDlgUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIME, m_UINTTime);
}


BEGIN_MESSAGE_MAP(CDlgUser, CDialogEx)
	
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUT_RegFinger, &CDlgUser::OnBnClickedButRegfinger)
	ON_BN_CLICKED(IDOK, &CDlgUser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUser::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgUser 消息处理程序


BOOL CDlgUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1, 1000, NULL);//加一个周期为一秒的定时器

	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	SetDlgItemText(IDC_EDIT_NAME, pMain->m_strIdtifyName);
	SetDlgItemText(IDC_EDIT_ID, pMain->m_strIdtifyId);
	SetDlgItemText(IDC_EDIT_TID, pMain->m_strIdtifyTid);
	SetDlgItemText(IDC_EDIT_RANK, pMain->m_strIdtifyRank);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}



void CDlgUser::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和
	m_PointScreen.Format(_T("%d,%d"), point.x, point.y);
	CDialogEx::OnMouseMove(nFlags, point);
}



void CDlgUser::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	if (m_CountTime1 > 0)
	{
		if ("" != m_PointScreen)
		{
			m_CountTime1 = 60;
		}  
		CString strTime;
		strTime.Format("%d", m_CountTime1);
		SetDlgItemTextA(IDC_EDIT_TIME, strTime);
		m_CountTime1--;
		m_PointScreen = "";
		if (m_CountTime1 == 0)
		{
			SendMessage(WM_CLOSE, 0, 0);
			//完成后记录日志
			CLogWriter logWriter;
			CString strWriter;
			CString strSetItem = _T("用户编辑超时");
			strWriter.Format("%12s——%s", pMain->m_strIdtifyName, strSetItem);//使输出到log对齐，少的地方补空格
			logWriter.WriteString(strWriter);
		}	   
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgUser::OnBnClickedButRegfinger()
{
	// TODO:  在此添加控件通知处理程序代码
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	CString key, strSeTemp="", strCbTemp="";
	int iTid = atoi(pMain->m_strIdtifyTid);
	pMain->m_Tid = iTid;
	key.Format(_T("peopleTemplist%d"), iTid-1);
	WritePrivateProfileStringA(key, "Temp", strSeTemp, "d:\\finger\\fingerTemp.ini");//把模板都清空；
	WritePrivateProfileStringA(key, "Tempsize", strCbTemp, "d:\\finger\\fingerTemp.ini");
	::SendMessageA(pMain->m_hWnd, WM_MYMSGCLEAR, 0, 0);//自定义的清除缓冲区指纹的消息
	::SendMessageA(pMain->m_hWnd, WM_COMMAND, IDC_BTN_REG, BN_CLICKED);//发送主对话框录入的点击信息
	//完成后记录日志
	CLogWriter logWriter;
	CString strWriter;
	CString strSetItem = _T("用户重录指纹");
	strWriter.Format("%12s——%s", pMain->m_strIdtifyName, strSetItem);//使输出到log对齐，少的地方补空格
	logWriter.WriteString(strWriter);
}


void CDlgUser::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	CString strName;
	int i_strNaLen = GetDlgItemTextA(IDC_EDIT_NAME, strName);
	CString strId;
	int i_strIdLen = GetDlgItemTextA(IDC_EDIT_ID, strId);
	if (i_strIdLen > 0 && i_strIdLen > 0)
	{
		if (IDYES == AfxMessageBox(_T("是否保存修改的信息？"), MB_YESNO))
		{
			CString key, string;
			key.Format(_T("peoplelist%d"), pMain->m_iIdentfyTid-1 );//获取指定行的id  得到要修改的节
			::WritePrivateProfileString(key, "Name", strName, "d:\\finger\\fingerprint.ini");
			::WritePrivateProfileString(key, "ID", strId, "d:\\finger\\fingerprint.ini");
			pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("修改本人数据成功！下次启动设备生效"));
			//记录到日志
			CLogWriter logWriter;
			CString strWriter;
			CString strSetItem = _T("用户修改数据");
			strWriter.Format("%12s——%s", pMain->m_strIdtifyName, strSetItem);//使输出到log对齐，少的地方补空格
			logWriter.WriteString(strWriter);
		}

	}

	KillTimer(1);
	CDialogEx::OnOK();
}


void CDlgUser::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	KillTimer(1);//
	CDialogEx::OnCancel();
}
