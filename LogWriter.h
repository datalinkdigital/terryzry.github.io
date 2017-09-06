#if !defined(AFX_LOGWRITER_H__716E48A5_D4A0_4183_94BD_7B0BE82B4B2E__INCLUDED_)
#define AFX_LOGWRITER_H__716E48A5_D4A0_4183_94BD_7B0BE82B4B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogWriter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogWriter window

class CLogWriter : public CObject
{
// Construction
public:
	CLogWriter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogWriter)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogWriter();

	// Generated message map functions
public:
	void WriteString(CString str);
	void WriteString(TCHAR *ch);
	void WriteString(CString str, int nLine, TCHAR *fileName);
	void WriteString(TCHAR *ch, int nLine, TCHAR *fileName);
private:
	CString m_strFilePath;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGWRITER_H__716E48A5_D4A0_4183_94BD_7B0BE82B4B2E__INCLUDED_)
