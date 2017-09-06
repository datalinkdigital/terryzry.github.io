
// libzkfpDemoDlg.cpp : 实现文件
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



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// ClibzkfpDemoDlg 对话框




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


// ClibzkfpDemoDlg 消息处理程序

BOOL ClibzkfpDemoDlg::OnInitDialog()
{
	//************************************************************
	CDialog::OnInitDialog();
	//初始化list列表
	auto *plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	plistctrl->InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 230);
	plistctrl->InsertColumn(1, _T("工号"), LVCFMT_LEFT, 230);
	plistctrl->InsertColumn(2, _T("指纹ID（tid）"), LVCFMT_LEFT, 212);
	//读取ini文件内容到list控件
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
	m_editFont.CreatePointFont(130, "宋体");
	editPlace->SetFont(&m_editFont); // 设置新字体
	// 设置两个常用的大按钮颜色等参数 更显眼
	auto pButConn = static_cast<CMFCButton *>(GetDlgItem(IDC_MFC_BTN_CONN));
	// 更改背景颜色 
	pButConn->SetFaceColor(RGB(153, 217, 234));
	// 更改字体颜色 
	//pbut->SetTextColor(RGB(255, 0, 0));
	// 更改高亮颜色 
	pButConn->SetTextHotColor(RGB(63, 72, 204));

	auto pButtDisconn = static_cast<CMFCButton *>(GetDlgItem(IDC_MFC_BTN_DISCONN));
	// 更改背景颜色 
	pButtDisconn->SetFaceColor(RGB(153, 217, 234));
	// 更改字体颜色 
	//pbut->SetTextColor(RGB(255, 0, 0));
	// 更改高亮颜色 
	pButtDisconn->SetTextHotColor(RGB(63, 72, 204));

	//******************************************************************************
	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ClibzkfpDemoDlg::OnPaint()
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	//装载图片文件MM.bmp
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
	//可以保存显示的指纹图片，但是后来发现保存图片没有用，改为保存指纹模板数据
	/*HRESULT hResult = cimFP->Save(_T("d:\\FingerBmp.bmp"));
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}*/
	//**********************************************************************

	//缩放显示
	cimFP->StretchBlt(hdcFP, 1, 1, width/2, height/2);//Z缩放除以2显示
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
				if (1 == pDlg->m_nFakeFunOn)	//FakeFinger Test   Z检测是不是伪造手指一定程度防伪
				{
					int nFakeStatus = 0;
					unsigned int retLen = sizeof(int);
					if (0 == ZKFPM_GetParameters(pDlg->m_hDevice, 2004, (unsigned char*)&nFakeStatus, &retLen))
					{
						if ((nFakeStatus & 31) != 31)
						{
							pDlg->SetDlgItemText(IDC_EDIT_RESULT, _T("请重试手指！"));
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
	if (m_nLastRegTempLen > 0)	//have enroll one more template至少有一个以上模板才识别啊！
	{
		CString strLog;
		if (m_bIdentify)//选择是识别指纹还是对比两个指纹是否匹配
		{
			int ret = ZKFP_ERR_OK;
			unsigned int tid = 0;
			unsigned int score = 0;
			ret = ZKFPM_DBIdentify(m_hDBCache, temp, len, &tid, &score);
			if (ZKFP_ERR_OK != ret)
			{
				strLog.Format(_T("识别失败, 错误 = %d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
			else
			{
				strLog.Format(_T("识别成功,指纹ID（tid）=%d, 对比分数（score）=%d"), tid, score);
				m_iIdentfyTid = tid;
				CString key;
				key.Format(_T("peoplelist%d"), tid - 1);
				::GetPrivateProfileString(key, "rank", NULL, m_strIdtifyRank.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				::GetPrivateProfileString(key, "Name", NULL, m_strIdtifyName.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				::GetPrivateProfileString(key, "ID", NULL, m_strIdtifyId.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				::GetPrivateProfileString(key, "tID", NULL, m_strIdtifyTid.GetBuffer(128), 128, "d:\\finger\\fingerprint.ini");
				CWnd* HWnd = FindWindowA(NULL, _T("登陆账号"));//得到指定窗口句柄
				if ("普通用户" == m_strIdtifyRank)
				{
					SetDlgItemText(IDC_EDIT_RESULT, "识别成功，普通用户");//通过是否成功判断能否登录
					if (NULL != HWnd)
					{
						::PostMessageA(HWnd->m_hWnd, WM_COMMAND, IDOK_FINGER, BN_CLICKED);//给密码登陆窗口发登陆消息
					}
				}
				if ("管理人员" == m_strIdtifyRank)
				{
					SetDlgItemText(IDC_EDIT_RESULT, "识别成功，管理人员");//通过是否成功判断能否登录
					if (NULL != HWnd)
					{
						::PostMessageA(HWnd->m_hWnd, WM_COMMAND, IDOK_FINGER, BN_CLICKED);//给密码登陆窗口发登陆消息
					}
				}
				if ("" == m_strIdtifyRank)
				{
					SetDlgItemText(IDC_EDIT_RESULT, "识别成功，但未分配等级");
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
		SetDlgItemText(IDC_EDIT_RESULT, _T("你还没有注册指纹模板！"));
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
			SetDlgItemText(IDC_EDIT_RESULT, _T("请使用同样的手指注册指纹模板"));
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
		ret = ZKFPM_DBMerge(m_hDBCache, m_arrPreRegTemps[0], m_arrPreRegTemps[1], m_arrPreRegTemps[2], szRegTemp, &cbRegTemp);//三枚合成一枚指纹模板
		m_enrollIdx = 0;
		m_bRegister = FALSE;
		if (ZKFP_ERR_OK == ret)
		{
			ret = ZKFPM_DBAdd(m_hDBCache, m_Tid++, szRegTemp, cbRegTemp);//添加指纹模板到缓冲区， m_tid指纹id
			if (ZKFP_ERR_OK == ret)
			{
				memcpy(m_szLastRegTemplate, szRegTemp, cbRegTemp);
				m_nLastRegTempLen = cbRegTemp;
				SetDlgItemText(IDC_EDIT_RESULT, _T("录入成功"));
				//*******************************************************
				//在保存指纹模板到ini初始化文件
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
				strLog.Format(_T("Register fail, because add to db fail, 返回值（ret）=%d"), ret);
				SetDlgItemText(IDC_EDIT_RESULT, strLog);
			}
		}
		else
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("录入失败"));
			return;
		}
	}
	else
	{
		strLog.Format(_T("还需要按压 %d 次手指"), ENROLLCNT-m_enrollIdx);
		SetDlgItemText(IDC_EDIT_RESULT, strLog);

	}
}


void ClibzkfpDemoDlg::OnBnClickedBtnReg()
{
	// TODO: 在此添加控件通知处理程序代码
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	m_ipeople = m_icount;
	UpdateData(FALSE);
	//录入指纹
	if (NULL != m_hDevice)//Z打开设备返回句柄不为0
	{
		if (!m_bRegister)//Z设备没有在录入则进行，
		{
			m_bRegister = TRUE;
			m_enrollIdx = 0;
			SetDlgItemText(IDC_EDIT_RESULT, _T("正在录入, 请按压三次"));
		}
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnDbclear()
{
	auto plistctrl = static_cast<CListCtrl *>(GetDlgItem(IDC_LIST1));
	int m_icount = plistctrl->GetItemCount();
	m_ipeople = m_icount;
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	if (NULL != m_hDevice && NULL != m_hDBCache)
	{
		//ZKFPM_DBClear(m_hDBCache);
		//***********************************************************
		CWnd* pWnd = FindWindow(NULL, _T("信息管理"));//得到指定窗口句柄
		auto  pList = static_cast<CListCtrl *>(pWnd->GetDlgItem(IDC_LIST1));
		int imark = pList->GetSelectionMark();//Z得到选定的list行
		CString strMarkId = pList->GetItemText(imark, 2);//Z该行对应的指纹id号
		int iszTemp = atoi(strMarkId);
		ZKFPM_DBDel(m_hDBCache, iszTemp);//Z删除指定的指纹模板
		CString key;
		int iId = _ttoi(strMarkId);
		key.Format(_T("peopleTemplist%d"), iId-1);
		::WritePrivateProfileString(key, NULL, NULL, _T("d:\\finger\\fingerTemp.ini"));//Z删除指纹模板ini对应的字段数据
		strLog.Format(_T("指纹id %d 数据清除成功"), iszTemp);
		SetDlgItemText(IDC_EDIT_RESULT, strLog);
		key.Format(_T("peoplelist%d"), m_Tid - 2);
		//********************************************************************
		m_nLastRegTempLen--;//
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnVerify()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL != m_hDevice)
	{
		if (m_bRegister)
		{
			m_bRegister = FALSE;
			SetDlgItemText(IDC_EDIT_RESULT, _T("从后面开始使用匹配的方式（不显示id）（Start verify last register template）"));
		}
		m_bIdentify = FALSE;
	}
}

void ClibzkfpDemoDlg::OnBnClickedBtnIdentify()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL != m_hDevice)
	{
		if (m_bRegister)
		{
			m_bRegister = FALSE;
			SetDlgItemText(IDC_EDIT_RESULT, _T("显示匹配的具体id（Start verify last register template）"));
		}
		m_bIdentify = TRUE;
	}
}

void ClibzkfpDemoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();
}

void ClibzkfpDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	OnBnClickedMfcBtnDisConn();
}

void ClibzkfpDemoDlg::OnBnClickedBtnRegByImg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_hDBCache)
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("Please connect device first"));
		return;
	}
	CString strFilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BMP Files (*.bmp)|*.bmp|JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		strFilePathName=dlg.GetPathName(); //文件名保存在了FilePathName里
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
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_hDBCache)//Z没初始化缓冲区
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("Please connect device first"));
		return;
	}
	CString strFilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL, 
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("BMP Files (*.bmp)|*.bmp|JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
		NULL);
	if(dlg.DoModal()==IDOK)
	{
		strFilePathName=dlg.GetPathName(); //文件完整路径保存在了FilePathName里
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
	// TODO:  在此添加控件通知处理程序代码
	SetDlgItemText(IDC_EDIT_RESULT,"选择通过指纹或者管理员账号密码登录");//先刷新一遍报告框，防止前面有管理员按过指纹留有”识别成功 管理人员“这样下个人直接登陆
	CPassDlg dlg_in;
	CString strPass;
	INT_PTR nResponse = dlg_in.DoModal();;//弹出密码窗口
	if (IDCANCEL == nResponse)
	{
		return;
	}
	GetDlgItemTextA(IDC_EDIT_RESULT, strPass);
	if ("欢迎！管理员登录成功！" == strPass)
	{
		CDlgAdmin  dlg_admin;
		if (IDCANCEL == dlg_admin.DoModal())//弹出管理窗口
		{
			return;
		}
	}
	if ("欢迎！用户登录成功！" == strPass)
	{
		CDlgUser dlg_user;
		if (IDCANCEL == dlg_user.DoModal())//弹出用户窗口
		{
			return;
		}
	}
}


void ClibzkfpDemoDlg::OnBnClickedMfcBtnConn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_hDevice)
	{
		if (ZKFPM_Init() != ZKFP_ERR_OK)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("Init ZKFPM fail"));
			return;
		}
		if ((m_hDevice = ZKFPM_OpenDevice(0)) == NULL)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("打开传感器失败（Open sensor fail）"));
			ZKFPM_Terminate();
			return;
		}
		m_hDBCache = ZKFPM_DBInit();//Z返回缓冲区句柄
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
		ZKFPM_SetParameters(m_hDevice, 2002, (unsigned char*)&m_nFakeFunOn, sizeof(int));//Z设置采集器参数

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
		GetDlgItem(IDC_MFC_BTN_CONN)->EnableWindow(FALSE);//下面按钮点亮
		GetDlgItem(IDC_MFC_BTN_DISCONN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUT_ADMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUT_LOGIN)->EnableWindow(TRUE);
		m_hThreadWork = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCapture, this, 0, NULL);
		memset(&m_szLastRegTemplate, 0x0, sizeof(m_szLastRegTemplate));
		SetDlgItemText(IDC_EDIT_RESULT, _T("初始化成功（Init Succ）"));
		m_Tid = 1;
		m_enrollIdx = 0;
		m_bRegister = FALSE;
		//	*****************************************************************************
		CString strFolderPath = "d:\\finger", strMsg;
		if (!PathIsDirectory(strFolderPath))
		{
			bool bSucc =  CreateDirectoryA(strFolderPath, NULL);
			SetFileAttributes(strFolderPath, FILE_ATTRIBUTE_HIDDEN); //隐藏
			if (!bSucc)
			{
				strMsg.Format("创建路径\"%s\"失败！是否继续?", strFolderPath);
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
		int count = ::GetPrivateProfileIntA(_T("peopleTempcount"), _T("Count"), 0, "d:\\finger\\fingerTemp.ini");//Z得到指纹模板总数
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
			m_nLastRegTempLen = count;//Z ini指纹模板count给缓冲区指纹模板数
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
					SetDlgItemText(IDC_EDIT_RESULT, _T("注意！设备已初始化但载入指纹库配置文件失败"));
				}
			}
			
		//****************************************************************
		}
		m_bIdentify = true;
		m_Tid = count + 1;
		if (iAddSeccTime != 0 || iAddFailTime != 0)
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("设备已初始化并成功载入指纹库配置文件"));
		}
		else
		{
			SetDlgItemText(IDC_EDIT_RESULT, _T("注意！设备已初始化但载入指纹库配置文件为空"));
		}
	}
	else
	{
		SetDlgItemText(IDC_EDIT_RESULT, _T("已经初始化过了(Already Init)"));
	}
}


void ClibzkfpDemoDlg::OnBnClickedMfcBtnDisConn()
{// TODO:  在此添加控件通知处理程序代码
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
		SetDlgItemText(IDC_EDIT_RESULT, _T("关闭成功（Close Succ)"));
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
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	if (NULL != m_hDevice && NULL != m_hDBCache)//设备和缓冲去都不为空才能删除
	{
		//ZKFPM_DBClear(m_hDBCache);
		//***********************************************************
		int i=ZKFPM_DBDel(m_hDBCache, m_Tid);//Z删除指定的指纹模板
		CString key;
		key.Format(_T("peopleTemplist%d"), m_Tid - 1);
		strLog.Format(_T("指纹id %d 数据清除成功"), m_Tid);
		SetDlgItemText(IDC_EDIT_RESULT, strLog);
		//********************************************************************
	}
	return 0;
}


void ClibzkfpDemoDlg::OnBnClickedButLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	CPassDlg dlg_in;
	CString strPass;
	INT_PTR nResponse = dlg_in.DoModal();;//弹出密码窗口
	if (IDCANCEL == nResponse)
	{
		return;
	}
	GetDlgItemTextA(IDC_EDIT_RESULT, strPass);
	if ("欢迎！管理员登录成功！" == strPass || "欢迎！用户登录成功！" == strPass)
	{
		CDlgaData  dlg_Data;
		if (IDCANCEL == dlg_Data.DoModal())//弹出管理窗口
		{
			return;
		}
	}
}
