// DlgUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "DlgUser.h"
#include "afxdialogex.h"
#include "libzkfpDemoDlg.h"
#include "LogWriter.h"


// CDlgUser �Ի���

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


// CDlgUser ��Ϣ�������


BOOL CDlgUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1, 1000, NULL);//��һ������Ϊһ��Ķ�ʱ��

	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	SetDlgItemText(IDC_EDIT_NAME, pMain->m_strIdtifyName);
	SetDlgItemText(IDC_EDIT_ID, pMain->m_strIdtifyId);
	SetDlgItemText(IDC_EDIT_TID, pMain->m_strIdtifyTid);
	SetDlgItemText(IDC_EDIT_RANK, pMain->m_strIdtifyRank);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}



void CDlgUser::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������
	m_PointScreen.Format(_T("%d,%d"), point.x, point.y);
	CDialogEx::OnMouseMove(nFlags, point);
}



void CDlgUser::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			//��ɺ��¼��־
			CLogWriter logWriter;
			CString strWriter;
			CString strSetItem = _T("�û��༭��ʱ");
			strWriter.Format("%12s����%s", pMain->m_strIdtifyName, strSetItem);//ʹ�����log���룬�ٵĵط����ո�
			logWriter.WriteString(strWriter);
		}	   
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgUser::OnBnClickedButRegfinger()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	CString key, strSeTemp="", strCbTemp="";
	int iTid = atoi(pMain->m_strIdtifyTid);
	pMain->m_Tid = iTid;
	key.Format(_T("peopleTemplist%d"), iTid-1);
	WritePrivateProfileStringA(key, "Temp", strSeTemp, "d:\\finger\\fingerTemp.ini");//��ģ�嶼��գ�
	WritePrivateProfileStringA(key, "Tempsize", strCbTemp, "d:\\finger\\fingerTemp.ini");
	::SendMessageA(pMain->m_hWnd, WM_MYMSGCLEAR, 0, 0);//�Զ�������������ָ�Ƶ���Ϣ
	::SendMessageA(pMain->m_hWnd, WM_COMMAND, IDC_BTN_REG, BN_CLICKED);//�������Ի���¼��ĵ����Ϣ
	//��ɺ��¼��־
	CLogWriter logWriter;
	CString strWriter;
	CString strSetItem = _T("�û���¼ָ��");
	strWriter.Format("%12s����%s", pMain->m_strIdtifyName, strSetItem);//ʹ�����log���룬�ٵĵط����ո�
	logWriter.WriteString(strWriter);
}


void CDlgUser::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ClibzkfpDemoDlg *pMain = (ClibzkfpDemoDlg *)AfxGetMainWnd();
	CString strName;
	int i_strNaLen = GetDlgItemTextA(IDC_EDIT_NAME, strName);
	CString strId;
	int i_strIdLen = GetDlgItemTextA(IDC_EDIT_ID, strId);
	if (i_strIdLen > 0 && i_strIdLen > 0)
	{
		if (IDYES == AfxMessageBox(_T("�Ƿ񱣴��޸ĵ���Ϣ��"), MB_YESNO))
		{
			CString key, string;
			key.Format(_T("peoplelist%d"), pMain->m_iIdentfyTid-1 );//��ȡָ���е�id  �õ�Ҫ�޸ĵĽ�
			::WritePrivateProfileString(key, "Name", strName, "d:\\finger\\fingerprint.ini");
			::WritePrivateProfileString(key, "ID", strId, "d:\\finger\\fingerprint.ini");
			pMain->SetDlgItemText(IDC_EDIT_RESULT, _T("�޸ı������ݳɹ����´������豸��Ч"));
			//��¼����־
			CLogWriter logWriter;
			CString strWriter;
			CString strSetItem = _T("�û��޸�����");
			strWriter.Format("%12s����%s", pMain->m_strIdtifyName, strSetItem);//ʹ�����log���룬�ٵĵط����ո�
			logWriter.WriteString(strWriter);
		}

	}

	KillTimer(1);
	CDialogEx::OnOK();
}


void CDlgUser::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);//
	CDialogEx::OnCancel();
}
