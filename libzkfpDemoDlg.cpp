
// libzkfpDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libzkfpDemo.h"
#include "libzkfpDemoDlg.h"
#include "PassDlg.h"
#include "DlgAdmin.h"
#include <winuser.h>
#include "LogWriter.h"
#include "DlgUser.h"
#include "DlgaData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

RGBQUAD g_GrayTable[256] = {0x0};



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// ClibzkfpDemoDlg �Ի���




ClibzkfpDemoDlg::ClibzkfpDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ClibzkfpDemoDlg::IDD, pParent)
	, m_ipeople(0)
	, m_strIdtifyRank(_T(""))
	, m_strIdtifyTid(_T(""))
	, m_iIdentfyTid(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDBCache = NULL;
	m_hDevice = NULL;
	m_bStopThread = FALSE;
	m_hThreadWork = NULL;
	m_Tid = 1;
	m_bRegister = FALSE;
	m_pImgBuf = NULL;
}

void ClibzkfpDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, m_ipeople);
}

BEGIN_MESSAGE_MAP(ClibzkfpDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_REG, &ClibzkfpDemoDlg::OnBnClickedBtnReg)
	ON_BN_CLICKED(IDC_BTN_DBCLEAR, &ClibzkfpDemoDlg::OnBnClickedBtnDbclear)
	ON_BN_CLICKED(IDC_BTN_VERIFY, &ClibzkfpDemoDlg::OnBnClickedBtnVerify)
	ON_BN_CLICKED(IDC_BTN_IDENTIFY, &ClibzkfpDemoDlg::OnBnClickedBtnIdentify)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_REG_BY_IMG, &ClibzkfpDemoDlg::OnBnClickedBtnRegByImg)
	ON_BN_CLICKED(IDC_BTN_IDENTIFY_BY_IMG, &ClibzkfpDemoDlg::OnBnClickedBtnIdentifyByImg)
	
	ON_BN_CLICKED(IDC_BUT_ADMIN, &ClibzkfpDemoDlg::OnBnClickedButAdmin)
	ON_BN_CLICKED(IDC_MFC_BTN_CONN, &ClibzkfpDemoDlg::OnBnClickedMfcBtnConn)
	ON_BN_CLICKED(IDC_MFC_BTN_DISCONN, &ClibzkfpDemoDlg::OnBnClickedMfcBtnDisConn)
	ON_MESSAGE(WM_MYMSGCLEAR, &ClibzkfpDemoDlg::OnMymsgclear)
	ON_BN_CLICKED(IDC_BUT_LOGIN, &ClibzkfpDemoDlg::OnBnClickedButLogin)
END_MESSAGE_MAP()


// ClibzkfpDemoDlg ��Ϣ�������

BOOL ClibzkfpDemoDlg::OnInitDialog()
{
	//************************************************************
	CDialog::OnInitDialog();
	//��ʼ��list�б�
	auto *plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	plistctrl->InsertColumn(0, _T("����"), LVCFMT_LEFT, 230);
	plistctrl->InsertColumn(1, _T("����"), LVCFMT_LEFT, 230);
	plistctrl->InsertColumn(2, _T("ָ��ID��tid��"), LVCFMT_LEFT, 212);
	//��ȡini�ļ����ݵ�list�ؼ�
	int count = ::GetPrivateProfileIntA(_T("peoplecount"), _T("Count"), 0, "d:\\finger\\fingerprint.ini");
	CString strCount;
	int iInsertLine = 0;
	strCount.Format("%d", count);
	SetDlgItemTextA(IDC_EDIT4, strCount);
	for (int i = 0; i < count; i++)
	{
		CString key;
		CString product;
		CString strName, strId, strTId;
		key.Format(_T("peoplelist%d"), i);
		::GetPrivateProfileString(key, "Name", NULL, strName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		::GetPrivateProfileString(key, "ID", NULL, strId.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		::GetPrivateProfileString(key, "tID", NULL, strTId.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
		if ("" != strName)
		{
			plistctrl->InsertItem(iInsertLine, strName);
			plistctrl->SetItemText(iInsertLine, 1, strId);
			plistctrl->SetItemText(iInsertLine, 2, strTId);
			iInsertLine++;
		}
		if ("" == strName)
		{

		}
	}
	CEdit *editPlace = (CEdit *)GetDlgItem(IDC_EDIT_RESULT);
	m_editFont.CreatePointFont(130, "����");
	editPlace->SetFont(&m_editFont); // ����������
	// �����������õĴ�ť��ɫ�Ȳ��� ������
	auto pButConn = static_cast<CMFCButton *>(GetDlgItem(IDC_MFC_BTN_CONN));
	// ���ı�����ɫ 
	pButConn->SetFaceColor(RGB(153, 217, 234));
	// ����������ɫ 
	//pbut->SetTextColor(RGB(255, 0, 0));
	// ���ĸ�����ɫ 
	pButConn->SetTextHotColor(RGB(63, 72, 204));

	auto pButtDisconn = static_cast<CMFCButton *>(GetDlgItem(IDC_MFC_BTN_DISCONN));
	// ���ı�����ɫ 
	pButtDisconn->SetFaceColor(RGB(153, 217, 234));
	// ����������ɫ 
	//pbut->SetTextColor(RGB(255, 0, 0));
	// ���ĸ�����ɫ 
	pButtDisconn->SetTextHotColor(RGB(63, 72, 204));

	//******************************************************************************
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_MFC_BTN_DISCONN)->EnableWindow(FALSE);
	/*GetDlgItem(IDC_BTN_CONN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_DISCONN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_REG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DBCLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_VERIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_IDENTIFY)->EnableWindow(FALSE);*/
	m_nLastRegTempLen = 0;
	for (int i = 0; i < 256; i++)
	{
		g_GrayTable[i].rgbRed = i;
		g_GrayTable[i].rgbGreen = i;
		g_GrayTable[i].rgbBlue = i;
		g_GrayTable[i].rgbReserved = 0;
	}
	m_bIdentify = TRUE;
	m_bRegister = FALSE;
	m_hDBCache = NULL;
	m_hDevice = NULL;
	m_bStopThread = FALSE;
	m_hThreadWork = NULL;
	m_nFakeFunOn = 0;
	memset(m_szLastRegTemplate, 0x0, MAX_TEMPLATE_SIZE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void ClibzkfpDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ClibzkfpDemoDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR ClibzkfpDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ClibzkfpDemoDlg::ShowImage(unsigned char* pImgBuf)
{
	ShowImage(pImgBuf, m_imgFPWidth, m_imgFPHeight);
}


void ClibzkfpDemoDlg::ShowImageFile(char* FileName)
{
	HBITMAP hbitmap;
	//װ��ͼƬ�ļ�MM.bmp
	hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),FileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	CStatic *imgFP = (CStatic *)GetDlgItem(IDC_STC_FPIMG);
	imgFP->SetBitmap(hbitmap);
}


void ClibzkfpDemoDlg::ShowImage(unsigned char* pImgBuf, int width, int height)
{
	CImage *cimFP = NULL;
	CStatic *imgFP = (CStatic *)GetDlgItem(IDC_STC_FPIMG);

	if (NULL == imgFP)
	{
		return;
	}
	CDC *cdcFP = imgFP->GetDC();
	if (NULL == cdcFP)
	{
		return;
	}
	HDC hdcFP = cdcFP->m_hDC;
	if (NULL == hdcFP)
	{
		return;
	}
	cdcFP->SetStretchBltMode(HALFTONE);

	cimFP = new CImage();
	cimFP->Create(width, height, 8);
	cimFP->SetColorTable(0, 256, g_GrayTable);
	for(int i = 0; i < height; i++)
	{				
		memcpy(cimFP->GetPixelAddress(0, i), (unsigned char*)pImgBuf + width * i, width);
	}
	//**********************************************************************
	//���Ա�����ʾ��ָ��ͼƬ�����Ǻ������ֱ���ͼƬû���ã���Ϊ����ָ��ģ������
	/*HRESULT hResult = cimFP->Save(_T("d:\\FingerBmp.bmp"));
	if (FAILED(hResult))
	{
		MessageBox(_T("����ͼ���ļ�ʧ�ܣ�"));
	}*/
	//**********************************************************************

	//������ʾ
	cimFP->StretchBlt(hdcFP, 1, 1, width/2, height/2);//Z���ų���2��ʾ
	if(cimFP)
	{
		cimFP->Destroy();
		delete cimFP;
	}
	return;
}

DWORD WINAPI ClibzkfpDemoDlg::ThreadCapture(LPVOID lParam)
{
	ClibzkfpDemoDlg* pDlg = (ClibzkfpDemoDlg*)lParam;
	if (NULL != pDlg)
	{
		pDlg->m_bStopThread = FALSE;
		while(!pDlg->m_bStopThread)
		{
			unsigned char szTemplate[MAX_TEMPLATE_SIZE];
			unsigned int tempLen = MAX_TEMPLATE_SIZE;
			int ret = ZKFPM_AcquireFingerprint(pDlg->m_hDevice, pDlg->m_pImgBuf, pDlg->m_imgFPWidth*pDlg->m_imgFPHeight, szTemplate, &tempLen);
			if (ZKFP_ERR_OK == ret)
			{
				if (1 == pDlg->m_nFakeFunOn)	//FakeFinger Test   Z����ǲ���α����ָһ���̶ȷ�α
				{
					int nFakeStatus = 0;
					unsigned int retLen = sizeof(int);
					if (0 == ZKFPM_GetParameters(pDlg->m_hDevice, 2004, (unsigned char*)&nFakeStatus, &retLen))
					{
						if ((nFakeStatus & 31) != 31)
						{
							pDlg->SetDlgItemText(IDC_EDIT_RESULT, _T("��������ָ��"));
							continue;
						}
					}
				}
				pDlg->ShowImage(pDlg->m_pImgBuf);

				if(pDlg->m_bRegister)
				{
					pDlg->DoRegister(szTemplate, tempLen);
				}
				else
				{
					pDlg->DoVerify(szTemplate, tempLen);
				}
			}
			
			Sleep(100);
		}
	}
	return 0;
}

void ClibzkfpDemoDlg::DoVerify(unsigned char* temp, int len)
{
	if (m_nLastRegTempLen > 0)	//have enroll one more template������һ������ģ���ʶ�𰡣�
	{
		CString strLog;
		if (m_bIdentify)//ѡ����ʶ��ָ�ƻ��ǶԱ�����ָ���Ƿ�ƥ��
		{
			int ret = ZKFP_ERR_OK;
			unsigned int tid = 0;
			unsigned int score = 0;
			ret = ZKFPM_DBIdentify(m_hDBCache, temp, len, &tid, &score);
			if (ZKFP_ERR_OK != ret)
			{
				strLog.Format(_T("ʶ��ʧ��, ���� = %d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
			else
			{
				strLog.Format(_T("ʶ��ɹ�,ָ��ID��tid��=%d, �Աȷ�����score��=%d"), tid, score);
				m_iIdentfyTid = tid;
				CString key;
				key.Format(_T("peoplelist%d"), tid - 1);
				::GetPrivateProfileString(key, "rank", NULL, m_strIdtifyRank.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				::GetPrivateProfileString(key, "Name", NULL, m_strIdtifyName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				::GetPrivateProfileString(key, "ID", NULL, m_strIdtifyId.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				::GetPrivateProfileString(key, "tID", NULL, m_strIdtifyTid.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				CWnd* HWnd = FindWindowA(NULL, _T("��½�˺�"));//�õ�ָ�����ھ��
				if ("��ͨ�û�" == m_strIdtifyRank)
				{
					SetDlgItemText(IDC_EDIT_RESULT, "ʶ��ɹ�����ͨ�û�");//ͨ���Ƿ�ɹ��ж��ܷ��¼
					if (NULL != HWnd)
					{
						::PostMessageA(HWnd->m_hWnd, WM_COMMAND, IDOK_FINGER, BN_CLICKED);//�������½���ڷ���½��Ϣ
					}
				}
				if ("������Ա" == m_strIdtifyRank)
				{
					SetDlgItemText(IDC_EDIT_RESULT, "ʶ��ɹ���������Ա");//ͨ���Ƿ�ɹ��ж��ܷ��¼
					if (NULL != HWnd)
					{
						::PostMessageA(HWnd->m_hWnd, WM_COMMAND, IDOK_FINGER, BN_CLICKED);//�������½���ڷ���½��Ϣ
					}
				}
				if ("" == m_strIdtifyRank)
				{
					SetDlgItemText(IDC_EDIT_RESULT, "ʶ��ɹ�����δ����ȼ�");
				}
			}
		}
		else
		{
			int ret = ZKFPM_DBMatch(m_hDBCache, m_szLastRegTemplate, m_nLastRegTempLen, temp, len);
			if (ZKFP_ERR_OK > ret)
			{
				strLog.Format(_T("Match finger fail, ret = %d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
			else
			{
				strLog.Format(_T("Match succ, score=%d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
		}
	}
	else
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("�㻹û��ע��ָ��ģ�壡"));
	}
}

void ClibzkfpDemoDlg::DoRegister(unsigned char* temp, int len)
{
	CString strLog;
	if (m_enrollIdx >= ENROLLCNT)
	{
		m_enrollIdx = 0;	//restart enroll
		return;
	}
	if (m_enrollIdx > 0)
	{
		if (0 >= ZKFPM_DBMatch(m_hDBCache, m_arrPreRegTemps[m_enrollIdx-1], m_arrPreTempsLen[m_enrollIdx-1], temp, len))
		{
			m_enrollIdx = 0;
			m_bRegister = FALSE;
			SetDlgItemText(IDC_EDIT_RESULT, _T("��ʹ��ͬ������ָע��ָ��ģ��"));
			return;
		}
	}
	m_arrPreTempsLen[m_enrollIdx] = len;
	memcpy(m_arrPreRegTemps[m_enrollIdx], temp, len);
	if (++m_enrollIdx >= ENROLLCNT)
	{
		int ret = 0;
		unsigned char szRegTemp[MAX_TEMPLATE_SIZE] = {0x0};
		unsigned int cbRegTemp = MAX_TEMPLATE_SIZE;
		ret = ZKFPM_DBMerge(m_hDBCache, m_arrPreRegTemps[0], m_arrPreRegTemps[1], m_arrPreRegTemps[2], szRegTemp, &cbRegTemp);//��ö�ϳ�һöָ��ģ��
		m_enrollIdx = 0;
		m_bRegister = FALSE;
		if (ZKFP_ERR_OK == ret)
		{
			ret = ZKFPM_DBAdd(m_hDBCache, m_Tid++, szRegTemp, cbRegTemp);//���ָ��ģ�嵽�������� m_tidָ��id
			if (ZKFP_ERR_OK == ret)
			{
				memcpy(m_szLastRegTemplate, szRegTemp, cbRegTemp);
				m_nLastRegTempLen = cbRegTemp;
				SetDlgItemText(IDC_EDIT_RESULT, _T("¼��ɹ�"));
				//*******************************************************
				//�ڱ���ָ��ģ�嵽ini��ʼ���ļ�
				char *pchar = new  char[MAX_TEMPLATE_SIZE];
				ZKFPM_BlobToBase64(szRegTemp, m_nLastRegTempLen, pchar, 2048);
				CString strSeTemp;
				CString strCbTemp;
				CString key;
				CString strName;
				strSeTemp.Format("%s", pchar);
				strCbTemp.Format("%d", cbRegTemp);
				key.Format(_T("peopleTemplist%d"), m_Tid - 2);
				WritePrivateProfileStringA(key, "Temp", strSeTemp, "d:\\finger\\fingerTemp.ini");
				WritePrivateProfileStringA(key, "Tempsize", strCbTemp, "d:\\finger\\fingerTemp.ini");
				/*FILE *pNewBmp = NULL;
				fopen_s(&pNewBmp, _T("d:\\FingerBmp\\new.tpl"), "wb+");
				fwrite(szRegTemp, 1, m_nLastRegTempLen, pNewBmp);*/
				key.Format(_T("peoplelist%d"), m_Tid - 2);
				::GetPrivateProfileString(key, "Name", NULL, strName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				//*******************************************************
			}
			else
			{
				strLog.Format(_T("Register fail, because add to db fail, ����ֵ��ret��=%d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
		}
		else
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("¼��ʧ��"));
			return;
		}
	}
	else
	{
		strLog.Format(_T("����Ҫ��ѹ %d ����ָ"), ENROLLCNT-m_enrollIdx);
		SetDlgItemText(IDC_EDIT_RESULT, strLog);

	}
}


void ClibzkfpDemoDlg::OnBnClickedBtnReg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	m_ipeople = m_icount;
	UpdateData(FALSE);
	//¼��ָ��
	if (NULL != m_hDevice)//Z���豸���ؾ����Ϊ0
	{
		if (!m_bRegister)//Z�豸û����¼������У�
		{
			m_bRegister = TRUE;
			m_enrollIdx = 0;
			SetDlgItemText(IDC_EDIT_RESULT, _T("����¼��, �밴ѹ����"));
		}
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnDbclear()
{
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	m_ipeople = m_icount;
	UpdateData(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strLog;
	if (NULL != m_hDevice && NULL != m_hDBCache)
	{
		//ZKFPM_DBClear(m_hDBCache);
		//***********************************************************
		CWnd* pWnd = FindWindow(NULL, _T("��Ϣ����"));//�õ�ָ�����ھ��
		auto  pList = static_cast<CListCtrl *>(pWnd->GetDlgItem(IDC_LIST1));
		int imark = pList->GetSelectionMark();//Z�õ�ѡ����list��
		CString strMarkId = pList->GetItemText(imark, 2);//Z���ж�Ӧ��ָ��id��
		int iszTemp = atoi(strMarkId);
		ZKFPM_DBDel(m_hDBCache, iszTemp);//Zɾ��ָ����ָ��ģ��
		CString key;
		int iId = _ttoi(strMarkId);
		key.Format(_T("peopleTemplist%d"), iId-1);
		::WritePrivateProfileString(key, NULL, NULL, _T("d:\\finger\\fingerTemp.ini"));//Zɾ��ָ��ģ��ini��Ӧ���ֶ�����
		strLog.Format(_T("ָ��id %d ��������ɹ�"), iszTemp);
		SetDlgItemText(IDC_EDIT_RESULT, strLog);
		key.Format(_T("peoplelist%d"), m_Tid - 2);
		//********************************************************************
		m_nLastRegTempLen--;//
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnVerify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL != m_hDevice)
	{
		if (m_bRegister)
		{
			m_bRegister = FALSE;
			SetDlgItemText(IDC_EDIT_RESULT, _T("�Ӻ��濪ʼʹ��ƥ��ķ�ʽ������ʾid����Start verify last register template��"));
		}
		m_bIdentify = FALSE;
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnIdentify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL != m_hDevice)
	{
		if (m_bRegister)
		{
			m_bRegister = FALSE;
			SetDlgItemText(IDC_EDIT_RESULT, _T("��ʾƥ��ľ���id��Start verify last register template��"));
		}
		m_bIdentify = TRUE;
	}
}

void ClibzkfpDemoDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();
}

void ClibzkfpDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	OnBnClickedMfcBtnDisConn();
}

void ClibzkfpDemoDlg::OnBnClickedBtnRegByImg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL == m_hDBCache)
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("Please connect device first"));
		return;
	}
	CString strFilePathName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BMP Files (*.bmp)|*.bmp|JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		strFilePathName=dlg.GetPathName(); //�ļ�����������FilePathName��
		unsigned char szTemp[MAX_TEMPLATE_SIZE] = {0x0};
		unsigned int sizeTemp = MAX_TEMPLATE_SIZE;
		CString strLog;
		int ret = ZKFPM_ExtractFromImage(m_hDBCache, strFilePathName.GetBuffer(), 500, szTemp, &sizeTemp);
		if (ZKFP_ERR_OK == ret)
		{
			ShowImageFile(strFilePathName.GetBuffer());
			ret = ZKFPM_DBAdd(m_hDBCache, m_Tid++, szTemp, sizeTemp);
			if (ZKFP_ERR_OK == ret)
			{
				memcpy(m_szLastRegTemplate, szTemp, sizeTemp);
				m_nLastRegTempLen = sizeTemp;
				SetDlgItemText(IDC_EDIT_RESULT, _T("Register succ"));
			}
			else
			{
				strLog.Format(_T("Register fail, because add to db fail, ret=%d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
		}
		else
		{
			strLog.Format(_T("Extract Fail, ret = %d"), ret);
			SetDlgItemText(IDC_EDIT_RESULT, strLog);
		}
	}
	else
	{
		return;
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnIdentifyByImg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL == m_hDBCache)//Zû��ʼ��������
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("Please connect device first"));
		return;
	}
	CString strFilePathName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BMP Files (*.bmp)|*.bmp|JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		strFilePathName=dlg.GetPathName(); //�ļ�����·����������FilePathName��
		unsigned char szTemp[MAX_TEMPLATE_SIZE] = {0x0};
		unsigned int sizeTemp = MAX_TEMPLATE_SIZE;
		CString strLog;
		int ret = ZKFPM_ExtractFromImage(m_hDBCache, strFilePathName.GetBuffer(), 500, szTemp, &sizeTemp);
		if (ZKFP_ERR_OK == ret)
		{
			ShowImageFile(strFilePathName.GetBuffer());
			if (m_bIdentify)
			{
				int ret = ZKFP_ERR_OK;
				unsigned int tid = 0;
				unsigned int score = 0;
				ret = ZKFPM_DBIdentify(m_hDBCache, szTemp, sizeTemp, &tid, &score);
				if (ZKFP_ERR_OK != ret)
				{
					strLog.Format(_T("Identify fail, ret = %d"), ret);
					SetDlgItemText(IDC_EDIT_RESULT, strLog);
				}
				else
				{
					strLog.Format(_T("Identify succ, tid=%d, score=%d"), tid, score);
					SetDlgItemText(IDC_EDIT_RESULT, strLog);
				}
			}
			else
			{
				int ret = ZKFPM_DBMatch(m_hDBCache, m_szLastRegTemplate, m_nLastRegTempLen, szTemp, sizeTemp);
				if (ZKFP_ERR_OK > ret)
				{
					strLog.Format(_T("Match finger fail, ret = %d"), ret);
					SetDlgItemText(IDC_EDIT_RESULT, strLog);
				}
				else
				{
					strLog.Format(_T("Match succ, score=%d"), ret);
					SetDlgItemText(IDC_EDIT_RESULT, strLog);
				}
			}
		}
		else
		{
			strLog.Format(_T("Extract Fail, ret = %d"), ret);
			SetDlgItemText(IDC_EDIT_RESULT, strLog);
		}
	}
	else
	{
		return;
	}
}







void ClibzkfpDemoDlg::OnBnClickedButAdmin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_EDIT_RESULT,"ѡ��ͨ��ָ�ƻ��߹���Ա�˺������¼");//��ˢ��һ�鱨��򣬷�ֹǰ���й���Ա����ָ�����С�ʶ��ɹ� ������Ա�������¸���ֱ�ӵ�½
	CPassDlg dlg_in;
	CString strPass;
	INT_PTR nResponse = dlg_in.DoModal();;//�������봰��
	if (IDCANCEL == nResponse)
	{
		return;
	}
	GetDlgItemTextA(IDC_EDIT_RESULT, strPass);
	if ("��ӭ������Ա��¼�ɹ���" == strPass)
	{
		CDlgAdmin  dlg_admin;
		if (IDCANCEL == dlg_admin.DoModal())//����������
		{
			return;
		}
	}
	if ("��ӭ���û���¼�ɹ���" == strPass)
	{
		CDlgUser dlg_user;
		if (IDCANCEL == dlg_user.DoModal())//�����û�����
		{
			return;
		}
	}
}


void ClibzkfpDemoDlg::OnBnClickedMfcBtnConn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (NULL == m_hDevice)
	{
		if (ZKFPM_Init() != ZKFP_ERR_OK)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("Init ZKFPM fail"));
			return;
		}
		if ((m_hDevice = ZKFPM_OpenDevice(0)) == NULL)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("�򿪴�����ʧ�ܣ�Open sensor fail��"));
			ZKFPM_Terminate();
			return;
		}
		m_hDBCache = ZKFPM_DBInit();//Z���ػ��������
		if (NULL == m_hDBCache)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("Create DBCache fail"));
			ZKFPM_CloseDevice(m_hDevice);
			ZKFPM_Terminate();
			return;
		}
		/*int nDPI = 750;
		ZKFPM_SetParameters(m_hDevice, 3, (unsigned char*)&nDPI, sizeof(int));*/
		//Set FakeFun On
		m_nFakeFunOn = 1;
		ZKFPM_SetParameters(m_hDevice, 2002, (unsigned char*)&m_nFakeFunOn, sizeof(int));//Z���òɼ�������

		/*TZKFPCapParams zkfpCapParams = {0x0};
		ZKFPM_GetCaptureParams(m_hDevice, &zkfpCapParams);
		m_imgFPWidth = zkfpCapParams.imgWidth;
		m_imgFPHeight = zkfpCapParams.imgHeight;*/
		unsigned int size = 4;
		ZKFPM_GetParameters(m_hDevice, 1, (unsigned char*)&m_imgFPWidth, &size);
		size = 4;
		ZKFPM_GetParameters(m_hDevice, 2, (unsigned char*)&m_imgFPHeight, &size);
		m_pImgBuf = new unsigned char[m_imgFPWidth*m_imgFPHeight];
		m_nLastRegTempLen = 0;
		GetDlgItem(IDC_MFC_BTN_CONN)->EnableWindow(FALSE);//���水ť����
		GetDlgItem(IDC_MFC_BTN_DISCONN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUT_ADMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUT_LOGIN)->EnableWindow(TRUE);
		m_hThreadWork = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCapture, this, 0, NULL);
		memset(&m_szLastRegTemplate, 0x0, sizeof(m_szLastRegTemplate));
		SetDlgItemText(IDC_EDIT_RESULT, _T("��ʼ���ɹ���Init Succ��"));
		m_Tid = 1;
		m_enrollIdx = 0;
		m_bRegister = FALSE;
		//	*****************************************************************************
		CString strFolderPath = "d:\\finger", strMsg;
		if (!PathIsDirectory(strFolderPath))
		{
			bool bSucc =  CreateDirectoryA(strFolderPath, NULL);
			SetFileAttributes(strFolderPath, FILE_ATTRIBUTE_HIDDEN); //����
			if (!bSucc)
			{
				strMsg.Format("����·��\"%s\"ʧ�ܣ��Ƿ����?", strFolderPath);
				if (AfxMessageBox(strMsg, MB_YESNO) == IDYES)
				return;
			}
		}
		unsigned char szTemplate[MAX_TEMPLATE_SIZE];
		char *pchar = new  char[MAX_TEMPLATE_SIZE];
		CString strSeTemp;
		CString strCbTemp;
		CString key;
		int iAddSeccTime = 0;
		int iAddFailTime = 0;
		int count = ::GetPrivateProfileIntA(_T("peopleTempcount"), _T("Count"), 0, "d:\\finger\\fingerTemp.ini");//Z�õ�ָ��ģ������
		for (int i = 0; i < count; i++)
		{
			key.Format(_T("peopleTemplist%d"), i);
			::GetPrivateProfileString(key, "Temp", NULL, strSeTemp.GetBuffer(MAX_TEMPLATE_SIZE), MAX_TEMPLATE_SIZE, "d:\\finger\\fingerTemp.ini");
			::GetPrivateProfileString(key, "Tempsize", NULL, strCbTemp.GetBuffer(128), 128, "d:\\finger\\fingerTemp.ini");
			char *pSeTemp = strSeTemp.GetBuffer(strSeTemp.GetLength());
			strSeTemp.ReleaseBuffer();
			int iszTemp = atoi(strCbTemp);
			ZKFPM_Base64ToBlob(pSeTemp, szTemplate, iszTemp);
			int  ret = -25;
			ret = ZKFPM_DBAdd(m_hDBCache, i + 1, szTemplate, iszTemp);
			m_nLastRegTempLen = count;//Z iniָ��ģ��count��������ָ��ģ����
			if (ZKFP_ERR_OK == ret)
			{
				iAddSeccTime++;
			}
			else
			{
				if (ZKFP_ERR_INVALID_PARAM == ret)
				{
					iAddFailTime++;
				}
				else
				{
					SetDlgItemText(IDC_EDIT_RESULT, _T("ע�⣡�豸�ѳ�ʼ��������ָ�ƿ������ļ�ʧ��"));
				}
			}
			
		//****************************************************************
		}
		m_bIdentify = true;
		m_Tid = count + 1;
		if (iAddSeccTime != 0 || iAddFailTime != 0)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("�豸�ѳ�ʼ�����ɹ�����ָ�ƿ������ļ�"));
		}
		else
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("ע�⣡�豸�ѳ�ʼ��������ָ�ƿ������ļ�Ϊ��"));
		}
	}
	else
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("�Ѿ���ʼ������(Already Init)"));
	}
}


void ClibzkfpDemoDlg::OnBnClickedMfcBtnDisConn()
{// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (NULL != m_hDevice)
	{
		if (NULL != m_pImgBuf)
		{
			delete[] m_pImgBuf;
			m_pImgBuf = NULL;
		}
		m_bStopThread = TRUE;
		if (NULL != m_hThreadWork)
		{
			WaitForSingleObject(m_hThreadWork, INFINITE);
			CloseHandle(m_hThreadWork);
			m_hThreadWork = NULL;
		}
		if (NULL != m_hDBCache)
		{//*****************************
			ZKFPM_DBFree(m_hDBCache);
			m_hDBCache = NULL;
		}
		ZKFPM_CloseDevice(m_hDevice);
		ZKFPM_Terminate();
		m_hDevice = NULL;
		SetDlgItemText(IDC_EDIT_RESULT, _T("�رճɹ���Close Succ)"));
		m_Tid = 1;
		GetDlgItem(IDC_MFC_BTN_CONN)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFC_BTN_DISCONN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_ADMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DBCLEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_VERIFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_IDENTIFY)->EnableWindow(FALSE);

	}
}

afx_msg LRESULT ClibzkfpDemoDlg::OnMymsgclear(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strLog;
	if (NULL != m_hDevice && NULL != m_hDBCache)//�豸�ͻ���ȥ����Ϊ�ղ���ɾ��
	{
		//ZKFPM_DBClear(m_hDBCache);
		//***********************************************************
		int i=ZKFPM_DBDel(m_hDBCache, m_Tid);//Zɾ��ָ����ָ��ģ��
		CString key;
		key.Format(_T("peopleTemplist%d"), m_Tid - 1);
		strLog.Format(_T("ָ��id %d ��������ɹ�"), m_Tid);
		SetDlgItemText(IDC_EDIT_RESULT, strLog);
		//********************************************************************
	}
	return 0;
}


void ClibzkfpDemoDlg::OnBnClickedButLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CPassDlg dlg_in;
	CString strPass;
	INT_PTR nResponse = dlg_in.DoModal();;//�������봰��
	if (IDCANCEL == nResponse)
	{
		return;
	}
	GetDlgItemTextA(IDC_EDIT_RESULT, strPass);
	if ("��ӭ������Ա��¼�ɹ���" == strPass || "��ӭ���û���¼�ɹ���" == strPass)
	{
		CDlgaData  dlg_Data;
		if (IDCANCEL == dlg_Data.DoModal())//����������
		{
			return;
		}
	}
}
