#pragma once


// CDlgAdmin �Ի���

class CDlgAdmin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAdmin)

public:
	CDlgAdmin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAdmin();

// �Ի�������
	enum { IDD = IDD_DLG_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��ʱ
	UINT m_UINTTime;
	UINT m_CountTime1=120;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CString m_strCursorPos;
	CString m_PointScreen;
	afx_msg void OnBnClickedBtnReg();
	// ����
	int m_iCountPeople;
	afx_msg void OnBnClickedBinCng();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	// �ȼ�
	int m_iRank;
	afx_msg void OnBnClickedBtnDel();
};
