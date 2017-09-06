// DlgAdmin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "DlgAdmin.h"
#include "afxdialogex.h"
#include "PassDlg.h"
#include "libzkfpDemoDlg.h"
#include "LogWriter.h"


// CDlgAdmin �Ի���

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


// CDlgAdmin ��Ϣ�������


BOOL CDlgAdmin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));//���list����Ϣ��
	plistctrl->InsertColumn(0, _T("����"), LVCFMT_LEFT, 150);
	plistctrl->InsertColumn(1, _T("����"), LVCFMT_LEFT, 150);
	plistctrl->InsertColumn(2, _T("ָ��ID��tid��"), LVCFMT_LEFT, 150);
	plistctrl->InsertColumn(3, _T("�û�Ȩ�޵ȼ�"), LVCFMT_LEFT, 145);

	SetTimer(1, 1000, NULL);//��һ������Ϊһ��Ķ�ʱ��

	//m_iRank = 0;//�ȼ��ؼ���ʼ��ѡ��Ϊ��ͨ�û�

	//��ȡini�ļ���list�ؼ�
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

	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgAdmin::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	if (m_CountTime1 > 0)
	{
		if ("" != m_PointScreen)
		{
			m_CountTime1 = 120;
		}
		m_UINTTime = m_CountTime1;	  //m_UINTTimeΪ����ʾ���ֵı༭������ı���
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
			CString strSetItem = _T("����༭��ʱ");
			strWriter.Format("%12s����%s", pMain->m_strIdtifyName, strSetItem);//ʹ�����log���룬�ٵĵط����ո�
			logWriter.WriteString(strWriter);
		}
			
	}
	CDialogEx::OnTimer(nIDEvent);
	
}


void CDlgAdmin::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_PointScreen.Format(_T("%d,%d"), point.x,point.y);
	SetDlgItemText(IDC_EDIT4, m_PointScreen);
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgAdmin::OnBnClickedBtnReg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����    
	BOOL ifFind = finder.FindFile(_T("d:\\finger\\fingerprint.ini"));
	if (!ifFind)
	{
		CFile file;
		file.Open(_T("d:\\finger\\fingerprint.ini"), CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite );//�򿪵�ģʽ�д������������д��ķ�ʽ
	}

	//������һ��ini�ļ�ȡ��count������һ�ڴ�������ini�У����ӿؼ�ȡ����ȫ��
	int iCount = ::GetPrivateProfileIntA(_T("peopleTempcount"), _T("Count"), 0, "d:\\finger\\fingerTemp.ini");
	CString strCount;
	strCount.Format(_T("%d"), iCount+1);
	::WritePrivateProfileString(_T("peopleTempcount"), "Count", strCount, "d:\\finger\\fingerTemp.ini");//���������´�����������Ҫ
	::WritePrivateProfileString(_T("peoplecount"), "Count", strCount, "d:\\finger\\fingerprint.ini");
	//¼��������id
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();//�õ������ھ��
	auto  pList1 = static_cast<CListCtrl *>(AfxGetMainWnd()->GetDlgItem(IDC_LIST1));
	CString string;
	int istr = GetDlgItemTextA(IDC_EDIT_NAME, string);
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	if (istr > 0)
	{
		CString key, strName;
		key.Format(_T("peoplelist%d"), pMain->m_Tid-1);
		pList1->InsertItem(m_icount, string);//���뵽������list
		plistctrl->InsertItem(m_icount, string);//���뵽������list
		::WritePrivateProfileString(key, "Name", string, "d:\\finger\\fingerprint.ini");//���浽ini
		strName = string;
		GetDlgItemTextA(IDC_EDIT_ID, string);
		plistctrl->SetItemText(m_icount, 1, string);
		pList1->SetItemText(m_icount, 1, string);
		::WritePrivateProfileString(key, "ID", string, "d:\\finger\\fingerprint.ini");
		string.Format(_T("%d"), pMain->m_Tid);
		plistctrl->SetItemText(m_icount, 2, string);
		pList1->SetItemText(m_icount, 2, string);
		::WritePrivateProfileString(key, "tID", string, "d:\\finger\\fingerprint.ini");
		//���һ�˺��������+1
		CString strCountPeople;
		GetDlgItemTextA(IDC_EDIT_PEOP, strCountPeople);
		m_iCountPeople = _ttoi(strCountPeople);
		strCountPeople.Format("%d", m_iCountPeople+1);
		SetDlgItemTextA(IDC_EDIT_PEOP, strCountPeople);
		UpdateData(TRUE);
		if (m_iRank == 0)
		{
			string = "��ͨ�û�";
		}
		if (m_iRank == 1)
		{
			string = "������Ա";
		}
		plistctrl->SetItemText(m_icount, 3, string);
		::WritePrivateProfileString(key, "rank", string, "d:\\finger\\fingerprint.ini");
		if (IDOK == MessageBox(_T("�ѱ���������Ϣ������Ҫ����ʾ�������ָ��¼��"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION))
		{
			::SendMessageA(pMain->m_hWnd, WM_COMMAND, IDC_BTN_REG, BN_CLICKED);//�������Ի���ĵ����Ϣ
			//�õ�¼���������¼����־
			CLogWriter logWriter;
			CString strWriter;
			strWriter.Format("%12s����%s����%s", pMain->m_strIdtifyName, "���һ����Ա", strName);
			logWriter.WriteString(strWriter);
			SetDlgItemTextA(IDC_EDIT_ID, "");//¼������edit�ؼ�
			SetDlgItemTextA(IDC_EDIT_NAME, "");
		}
	}
	else
	{
		MessageBoxA(_T("������Ա��������"), _T("ע��"));
		return;
	}
	

}


void CDlgAdmin::OnBnClickedBinCng()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	auto  pList1 = static_cast<CListCtrl *>(AfxGetMainWnd()->GetDlgItem(IDC_LIST1));
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	int imark = plistctrl->GetSelectionMark();
	int m_icount = plistctrl->GetItemCount();
	if (imark <0)
	{
		AfxMessageBox(_T("��ѡ��һ������!"), MB_ICONINFORMATION);//!������ͼ�ꣻ
		return;
	}

	{
		if (IDYES == AfxMessageBox(_T("ȷ��ɾ��ѡ����������"), MB_YESNO))
		{
			ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
			CString str = plistctrl->GetItemText(imark, 2);//�õ�Ҫɾ����id
			CString key, strName;
			int iId = _ttoi(str);
			key.Format(_T("peoplelist%d"), iId-1);//��ȡָ���е�id   �õ�Ҫɾ���Ľ�
			::GetPrivateProfileString(key, "Name", NULL, strName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
			::WritePrivateProfileString(key, NULL, NULL, "d:\\finger\\fingerprint.ini");//ɾ��ini��Ӧ�ڵ�����
			::SendMessageA(pMain->m_hWnd, WM_COMMAND, IDC_BTN_DBCLEAR, BN_CLICKED);//��������ɾ������Ϣ
			plistctrl->DeleteItem(imark);//ɾ��������������
			pList1->DeleteItem(imark);//ɾ��������������
			//****************��¼��log   T8.30
			CLogWriter logWriter;
			CString strWriter;
			strWriter.Format("%12s����%s����%s ", pMain->m_strIdtifyName, "ɾ��һ����Ա", strName);
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strCount;
	GetDlgItemTextA(IDC_EDIT_PEOP, strCount);
	::WritePrivateProfileString(_T("peopleTempcount"), "Count", strCount, "d:\\finger\\fingerTemp.ini");
	::WritePrivateProfileString(_T("peoplecount"), "Count", strCount, "d:\\finger\\fingerprint.ini");
	KillTimer(1);
	CDialogEx::OnOK();
}





void CDlgAdmin::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	plistctrl->DeleteItem(m_icount);//ɾ��������������
	KillTimer(1);
	CDialogEx::OnCancel();
}


void CDlgAdmin::OnBnClickedBtnDel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
			AfxMessageBox(_T("��ѡ��Ҫ�޸ĵ�һ������!"), MB_ICONINFORMATION);//!������ͼ�ꣻ
			return;
		}
		else
		{
			if (IDYES == AfxMessageBox(_T("ȷ���޸�ѡ����������"), MB_YESNO))
			{
				CString str = plistctrl->GetItemText(imark, 2);//�õ�Ҫɾ����id
				CString key, string;
				int iId = _ttoi(str);
				key.Format(_T("peoplelist%d"), iId - 1);//��ȡָ���е�id  �õ�Ҫ�޸ĵĽ�
				int istr = GetDlgItemTextA(IDC_EDIT_NAME, string);
				::WritePrivateProfileString(key, "Name", string, "d:\\finger\\fingerprint.ini");//���ǵ�ini��Ӧ�Ľ�
				GetDlgItemTextA(IDC_EDIT_ID, string);
				::WritePrivateProfileString(key, "ID", string, "d:\\finger\\fingerprint.ini");
				UpdateData(TRUE);
				if (m_iRank == 0)
				{
					string = "��ͨ�û�";
				}
				if (m_iRank == 1)
				{
					string = "������Ա";
				}
				::WritePrivateProfileString(key, "rank", string, "d:\\finger\\fingerprint.ini");
				//������������list
				plistctrl->DeleteItem(imark);//ɾ��������������
				pList1->DeleteItem(imark);//ɾ��������������
				plistctrl->InsertItem(imark, strName);
				pList1->InsertItem(imark, strName);
				plistctrl->SetItemText(imark, 1, strId);
				pList1->SetItemText(imark, 1, strId);
				plistctrl->SetItemText(imark, 3, string);
				::GetPrivateProfileString(key, "tID", NULL, string.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				plistctrl->SetItemText(imark, 2, string);
				pList1->SetItemText(imark, 2, string);
				pList1->SetDlgItemText(IDC_EDIT_RESULT, _T("�޸�ѡ�������ݳɹ���"));
				//��ɺ��¼��־
				CLogWriter logWriter;
				CString strWriter;
				strWriter.Format("%12s����%s����%s ", pMain->m_strIdtifyName, "�޸�һ����Ա", strName);
				logWriter.WriteString(strWriter);
				SetDlgItemTextA(IDC_EDIT_ID, "");//¼������edit�ؼ�
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
		MessageBoxA(_T("�������޸ĺ��Ա�������͹��ţ�"), _T("�޸�ע��"));
		return;
	}
}
