// DlgAdmin.cpp : 实现文件
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "DlgAdmin.h"
#include "afxdialogex.h"
#include "PassDlg.h"
#include "libzkfpDemoDlg.h"
#include "LogWriter.h"


// CDlgAdmin 对话框

IMPLEMENT_DYNAMIC(CDlgAdmin, CDialogEx)

CDlgAdmin::CDlgAdmin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAdmin::IDD, pParent)
	, m_UINTTime(0)
	, m_strCursorPos(_T(""))
	, m_iCountPeople(0)
	, m_iRank(0)
{

}

CDlgAdmin::~CDlgAdmin()
{
}

void CDlgAdmin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIME, m_UINTTime);
	DDV_MinMaxUInt(pDX, m_UINTTime, 0, 500);
	DDX_Text(pDX, IDC_EDIT_PEOP, m_iCountPeople);
	DDX_Radio(pDX, IDC_RADIO_User, m_iRank);
}


BEGIN_MESSAGE_MAP(CDlgAdmin, CDialogEx)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_REG, &CDlgAdmin::OnBnClickedBtnReg)
	ON_BN_CLICKED(IDC_BIN_CNG, &CDlgAdmin::OnBnClickedBinCng)
	ON_BN_CLICKED(IDOK, &CDlgAdmin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAdmin::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgAdmin::OnBnClickedBtnDel)
END_MESSAGE_MAP()


// CDlgAdmin 消息处理程序


BOOL CDlgAdmin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));//添加list的信息栏
	plistctrl->InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 150);
	plistctrl->InsertColumn(1, _T("工号"), LVCFMT_LEFT, 150);
	plistctrl->InsertColumn(2, _T("指纹ID（tid）"), LVCFMT_LEFT, 150);
	plistctrl->InsertColumn(3, _T("用户权限等级"), LVCFMT_LEFT, 145);

	SetTimer(1, 1000, NULL);//加一个周期为一秒的定时器

	//m_iRank = 0;//等级控件初始化选中为普通用户

	//读取ini文件到list控件
	int iInsertLine=0;
	int count = ::GetPrivateProfileIntA(_T("peoplecount"), _T("Count"), 0, "d:\\finger\\fingerprint.ini");
	CString strCount;
	strCount.Format("%d", count);
	SetDlgItemTextA(IDC_EDIT_PEOP, strCount);
	for (int i = 0; i < count; i++)
	{
		CString key;
		CString product;
		CString strName, strId, strTId, strRank;
		key.Format(_T("peoplelist%d"), i);
		::GetPrivateProfileString(key, "Name", NULL, strName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		::GetPrivateProfileString(key, "ID", NULL, strId.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		::GetPrivateProfileString(key, "tID", NULL, strTId.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		::GetPrivateProfileString(key, "rank", NULL, strRank.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		if ("" != strName)
		{
			plistctrl->InsertItem(iInsertLine, strName);
			plistctrl->SetItemText(iInsertLine, 1, strId);
			plistctrl->SetItemText(iInsertLine, 2, strTId);
			plistctrl->SetItemText(iInsertLine, 3, strRank);
			iInsertLine++;
		}
		if ("" == strName)
		{
			
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgAdmin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	if (m_CountTime1 > 0)
	{
		if ("" != m_PointScreen)
		{
			m_CountTime1 = 120;
		}
		m_UINTTime = m_CountTime1;	  //m_UINTTime为与显示数字的编辑框关联的变量
		CString strTime;
		strTime.Format("%d", m_CountTime1);
		SetDlgItemTextA(IDC_EDIT_TIME, strTime);
		m_CountTime1--;
		m_PointScreen = "";
		if (m_CountTime1 == 0)
		{
			SendMessage(WM_CLOSE, 0, 0);
			CLogWriter logWriter;
			CString strWriter;
			CString strSetItem = _T("管理编辑超时");
			strWriter.Format("%12s——%s", pMain->m_strIdtifyName, strSetItem);//使输出到log对齐，少的地方补空格
			logWriter.WriteString(strWriter);
		}
			
	}
	CDialogEx::OnTimer(nIDEvent);
	
}


void CDlgAdmin::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_PointScreen.Format(_T("%d,%d"), point.x,point.y);
	SetDlgItemText(IDC_EDIT4, m_PointScreen);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgAdmin::OnBnClickedBtnReg()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用    
	BOOL ifFind = finder.FindFile(_T("d:\\finger\\fingerprint.ini"));
	if (!ifFind)
	{
		CFile file;
		file.Open(_T("d:\\finger\\fingerprint.ini"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite );//打开的模式有创建但不清空且写入的方式
	}

	//从任意一个ini文件取出count人数加一在存入两个ini中（不从控件取更安全）
	int iCount = ::GetPrivateProfileIntA(_T("peopleTempcount"), _T("Count"), 0, "d:\\finger\\fingerTemp.ini");
	CString strCount;
	strCount.Format(_T("%d"), iCount+1);
	::WritePrivateProfileString(_T("peopleTempcount"), "Count", strCount, "d:\\finger\\fingerTemp.ini");//保存总数下次载入数据需要
	::WritePrivateProfileString(_T("peoplecount"), "Count", strCount, "d:\\finger\\fingerprint.ini");
	//录入姓名和id
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();//得到主窗口句柄
	auto  pList1 = static_cast<CListCtrl *>(AfxGetMainWnd()->GetDlgItem(IDC_LIST1));
	CString string;
	int istr = GetDlgItemTextA(IDC_EDIT_NAME, string);
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	if (istr > 0)
	{
		CString key, strName;
		key.Format(_T("peoplelist%d"), pMain->m_Tid-1);
		pList1->InsertItem(m_icount, string);//输入到主窗口list
		plistctrl->InsertItem(m_icount, string);//输入到副窗口list
		::WritePrivateProfileString(key, "Name", string, "d:\\finger\\fingerprint.ini");//保存到ini
		strName = string;
		GetDlgItemTextA(IDC_EDIT_ID, string);
		plistctrl->SetItemText(m_icount, 1, string);
		pList1->SetItemText(m_icount, 1, string);
		::WritePrivateProfileString(key, "ID", string, "d:\\finger\\fingerprint.ini");
		string.Format(_T("%d"), pMain->m_Tid);
		plistctrl->SetItemText(m_icount, 2, string);
		pList1->SetItemText(m_icount, 2, string);
		::WritePrivateProfileString(key, "tID", string, "d:\\finger\\fingerprint.ini");
		//添加一人后把总人数+1
		CString strCountPeople;
		GetDlgItemTextA(IDC_EDIT_PEOP, strCountPeople);
		m_iCountPeople = _ttoi(strCountPeople);
		strCountPeople.Format("%d", m_iCountPeople+1);
		SetDlgItemTextA(IDC_EDIT_PEOP, strCountPeople);
		UpdateData(TRUE);
		if (m_iRank == 0)
		{
			string = "普通用户";
		}
		if (m_iRank == 1)
		{
			string = "管理人员";
		}
		plistctrl->SetItemText(m_icount, 3, string);
		::WritePrivateProfileString(key, "rank", string, "d:\\finger\\fingerprint.ini");
		if (IDOK == MessageBox(_T("已保存人物信息，还需要按提示完成三次指纹录入"), _T("信息"), MB_OK | MB_ICONINFORMATION))
		{
			::SendMessageA(pMain->m_hWnd, WM_COMMAND, IDC_BTN_REG, BN_CLICKED);//发送主对话框的点击信息
			//得到录入的人名记录到日志
			CLogWriter logWriter;
			CString strWriter;
			strWriter.Format("%12s——%s——%s", pMain->m_strIdtifyName, "添加一名成员", strName);
			logWriter.WriteString(strWriter);
			SetDlgItemTextA(IDC_EDIT_ID, "");//录入后清空edit控件
			SetDlgItemTextA(IDC_EDIT_NAME, "");
		}
	}
	else
	{
		MessageBoxA(_T("请填入员工姓名！"), _T("注意"));
		return;
	}
	

}


void CDlgAdmin::OnBnClickedBinCng()
{
	// TODO:  在此添加控件通知处理程序代码
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	auto  pList1 = static_cast<CListCtrl *>(AfxGetMainWnd()->GetDlgItem(IDC_LIST1));
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	int imark = plistctrl->GetSelectionMark();
	int m_icount = plistctrl->GetItemCount();
	if (imark <0)
	{
		AfxMessageBox(_T("先选择一行数据!"), MB_ICONINFORMATION);//!的提醒图标；
		return;
	}

	{
		if (IDYES == AfxMessageBox(_T("确定删除选中行数据吗？"), MB_YESNO))
		{
			ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
			CString str = plistctrl->GetItemText(imark, 2);//得到要删除的id
			CString key, strName;
			int iId = _ttoi(str);
			key.Format(_T("peoplelist%d"), iId-1);//获取指定行的id   得到要删除的节
			::GetPrivateProfileString(key, "Name", NULL, strName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
			::WritePrivateProfileString(key, NULL, NULL, "d:\\finger\\fingerprint.ini");//删除ini对应节的数据
			::SendMessageA(pMain->m_hWnd, WM_COMMAND, IDC_BTN_DBCLEAR, BN_CLICKED);//给主窗口删除发消息
			plistctrl->DeleteItem(imark);//删除副窗口行数据
			pList1->DeleteItem(imark);//删除主窗口行数据
			//****************记录到log   T8.30
			CLogWriter logWriter;
			CString strWriter;
			strWriter.Format("%12s——%s——%s ", pMain->m_strIdtifyName, "删除一名成员", strName);
			logWriter.WriteString(strWriter);
		}
		else
		{
			return;
		}
	}


}


void CDlgAdmin::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strCount;
	GetDlgItemTextA(IDC_EDIT_PEOP, strCount);
	::WritePrivateProfileString(_T("peopleTempcount"), "Count", strCount, "d:\\finger\\fingerTemp.ini");
	::WritePrivateProfileString(_T("peoplecount"), "Count", strCount, "d:\\finger\\fingerprint.ini");
	KillTimer(1);
	CDialogEx::OnOK();
}





void CDlgAdmin::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	plistctrl->DeleteItem(m_icount);//删除副窗口行数据
	KillTimer(1);
	CDialogEx::OnCancel();
}


void CDlgAdmin::OnBnClickedBtnDel()
{
	// TODO:  在此添加控件通知处理程序代码
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	auto  pList1 = static_cast<CListCtrl *>(AfxGetMainWnd()->GetDlgItem(IDC_LIST1));
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	int imark = plistctrl->GetSelectionMark();
	int m_icount = plistctrl->GetItemCount();
	CString strName;
	int i_strNaLen = GetDlgItemTextA(IDC_EDIT_NAME, strName);
	CString strId;
	int i_strIdLen = GetDlgItemTextA(IDC_EDIT_ID, strId);
	if (i_strIdLen > 0 && i_strIdLen > 0)
	{
		if (imark <0)
		{
			AfxMessageBox(_T("先选择要修改的一行数据!"), MB_ICONINFORMATION);//!的提醒图标；
			return;
		}
		else
		{
			if (IDYES == AfxMessageBox(_T("确定修改选中行数据吗？"), MB_YESNO))
			{
				CString str = plistctrl->GetItemText(imark, 2);//得到要删除的id
				CString key, string;
				int iId = _ttoi(str);
				key.Format(_T("peoplelist%d"), iId - 1);//获取指定行的id  得到要修改的节
				int istr = GetDlgItemTextA(IDC_EDIT_NAME, string);
				::WritePrivateProfileString(key, "Name", string, "d:\\finger\\fingerprint.ini");//覆盖到ini对应的节
				GetDlgItemTextA(IDC_EDIT_ID, string);
				::WritePrivateProfileString(key, "ID", string, "d:\\finger\\fingerprint.ini");
				UpdateData(TRUE);
				if (m_iRank == 0)
				{
					string = "普通用户";
				}
				if (m_iRank == 1)
				{
					string = "管理人员";
				}
				::WritePrivateProfileString(key, "rank", string, "d:\\finger\\fingerprint.ini");
				//清除后再输出到list
				plistctrl->DeleteItem(imark);//删除副窗口行数据
				pList1->DeleteItem(imark);//删除主窗口行数据
				plistctrl->InsertItem(imark, strName);
				pList1->InsertItem(imark, strName);
				plistctrl->SetItemText(imark, 1, strId);
				pList1->SetItemText(imark, 1, strId);
				plistctrl->SetItemText(imark, 3, string);
				::GetPrivateProfileString(key, "tID", NULL, string.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				plistctrl->SetItemText(imark, 2, string);
				pList1->SetItemText(imark, 2, string);
				pList1->SetDlgItemText(IDC_EDIT_RESULT, _T("修改选中项数据成功！"));
				//完成后记录日志
				CLogWriter logWriter;
				CString strWriter;
				strWriter.Format("%12s——%s——%s ", pMain->m_strIdtifyName, "修改一名成员", strName);
				logWriter.WriteString(strWriter);
				SetDlgItemTextA(IDC_EDIT_ID, "");//录入后清空edit控件
				SetDlgItemTextA(IDC_EDIT_NAME, "");
			}
			else
			{
				return;
			}
	    }
	}
	else
	{
		MessageBoxA(_T("请填入修改后的员工姓名和工号！"), _T("修改注意"));
		return;
	}
}
