#pragma once


// CDlgaData �Ի���

class CDlgaData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgaData)

public:
	CDlgaData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgaData();

// �Ի�������
	enum { IDD = IDD_DLG_USE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButSave();
};
