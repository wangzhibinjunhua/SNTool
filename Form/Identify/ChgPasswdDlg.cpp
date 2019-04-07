// ChgPasswdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SN Writer.h"
#include "ChgPasswdDlg.h"
#include "des.h"
#include "Base64Encode.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChgPasswdDlg dialog


CChgPasswdDlg::CChgPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChgPasswdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChgPasswdDlg)
	m_strOldPasswd = _T("");
	m_strNewPasswd = _T("");
	m_strConfirmPasswd = _T("");
	//}}AFX_DATA_INIT

    //memset(m_strNewPasswd, 0, sizeof(m_strNewPasswd));
}


void CChgPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChgPasswdDlg)
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWD, m_strOldPasswd);
	DDV_MaxChars(pDX, m_strOldPasswd, 64);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWD, m_strNewPasswd);
	DDV_MaxChars(pDX, m_strNewPasswd, 64);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_PASSWD, m_strConfirmPasswd);
	DDV_MaxChars(pDX, m_strConfirmPasswd, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChgPasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CChgPasswdDlg)
	ON_BN_CLICKED(ID_CHG_PASSWD_CANCEL, OnChgPasswdCancel)
	ON_BN_CLICKED(ID_CHG_PASSWD_OK, OnChgPasswdOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChgPasswdDlg message handlers

void CChgPasswdDlg::OnChgPasswdCancel() 
{
	// TODO: Add your control notification handler code here
    CDialog::OnCancel();
}

void CChgPasswdDlg::OnChgPasswdOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	if(VerifyOldPasswd())
    {
        if(ConfirmNewPasswd())
        {
			//modify by wzb
#if 1
			char SetupDir[512];
			GetCurrentDirectory(512, SetupDir);
			sprintf(SetupDir, "%s\\WDATA", SetupDir);
			CFile file;
			if (!file.Open(SetupDir, CFile::modeReadWrite))
			{
				AfxMessageBox(_T("ÅäÖÃÎÄ¼þËð»µ!\r\n ´íÎó´úÂë0x0x100019"));
				CDialog::OnOK();
			}
			else 
			{
				//AfxMessageBox(m_strNewPasswd);
				CEncode mCEnocde;
				CString str1 = mCEnocde.base64encode(m_strNewPasswd, strlen(m_strNewPasswd));
				//AfxMessageBox(str1);
				char pStrWrite[10] = { 0 };
				memcpy(pStrWrite, (char*)(LPCTSTR)str1, 8);
				file.Seek(6, CFile::begin);
				file.Write(pStrWrite, 8);
				file.Close();
				MessageBox("Change passwd success!", "Change Passwd", MB_OK);
				CDialog::OnOK();
			}
#endif
#if 0
			LPTSTR lpPasswd =(LPTSTR)(LPCTSTR)m_strNewPasswd;
            if(SaveLoginPasswd(lpPasswd))
            {
                MessageBox("Change passwd success!", "Change Passwd", MB_OK);
                CDialog::OnOK();
            }
            else
            {
                MessageBox("Save new passwd cause fail, recover old passwd!", "Change Passwd", MB_OK);
                lpPasswd =(LPTSTR)(LPCTSTR)m_strOldPasswd;
                SaveLoginPasswd(lpPasswd);
                CDialog::OnOK();
            }   
#endif
        }
        else
        {
            MessageBox("New passwd not the same with confirm passwd!!", "Warning", MB_ICONWARNING);
        }
    }
    else
    {
        MessageBox("Incorrect old password, please try again!", "Warning", MB_ICONWARNING);
    }
}

bool CChgPasswdDlg::VerifyOldPasswd()
{
    UpdateData(TRUE);
	//modify by wzb
#if 1 //new password wdata
	char SetupDir[512];
	GetCurrentDirectory(512, SetupDir);
	sprintf(SetupDir, "%s\\WDATA", SetupDir);
	CFile file;
	if (!file.Open(SetupDir, CFile::modeReadWrite))
	{
		AfxMessageBox(_T("ÅäÖÃÎÄ¼þËð»µ!\r\n ´íÎó´úÂë0x0x100019"));
		return false;
	}
	file.Seek(6, CFile::begin);
	char buffer[16] = { 0 };
	file.Read(buffer, 8);
	file.Close();
	CString tmpStr;
	tmpStr.Format(_T("%s"), buffer);
	CEncode mCEnocde;
	CString strInputPw = mCEnocde.base64encode(m_strOldPasswd, strlen(m_strOldPasswd));
	if (tmpStr != strInputPw)
	{
		m_strOldPasswd = "";
		UpdateData(FALSE);
		MessageBox("Incorrect old password, please try again!", "Warning", MB_ICONWARNING);
		return false;
	}
	else
	{
		return true;
	}

	return false;
#endif
#if 0
    BYTE key[10]="03055030";    
    BYTE strPassword[LOGIN_PASSWORD_MAX] = {0};
    BYTE *pOldPasswd = strPassword;
    Login_Identify_e Identify = UNKNOW_IDENTIFY;
    
    if(ReadDataFromFile(strPassword, LOGIN_PASSWORD_MAX, key, g_pPasswdFilepath) == S_DES_SUCCESS)
    {
        CString tmpStr;
        pOldPasswd += 2;
        tmpStr.Format(_T("%s"), pOldPasswd);
        if (tmpStr != m_strOldPasswd)
        {
            m_strOldPasswd = "";
            UpdateData(FALSE);
            MessageBox("Incorrect old password, please try again!", "Warning", MB_ICONWARNING);
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
#endif 
	//end
}

bool CChgPasswdDlg::ConfirmNewPasswd()
{
    if (m_strNewPasswd.GetLength() == 0 || m_strConfirmPasswd.GetLength() == 0)
    {
        return false;
    }

	//add by wzb limit password len 6
	if (m_strNewPasswd.GetLength() != 6 || m_strConfirmPasswd.GetLength() != 6)
	{
		MessageBox("password length must be 6!!", "Warning", MB_ICONWARNING);
		return false;
	}
	//end
    if (m_strNewPasswd != m_strConfirmPasswd)
    {
        return false;
    }
    else
    {
        return true;
    }   
}


BOOL CChgPasswdDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam ==VK_ESCAPE)
    {
        return true;
    }
   
    int currentID = ID_CHG_PASSWD_OK;
    int nextID = ID_CHG_PASSWD_OK;
    UpdateData(TRUE);
    if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
    {                  
        currentID = GetFocus()->GetDlgCtrlID();
        switch(currentID)
        {
        case IDC_EDIT_OLD_PASSWD:
            if (VerifyOldPasswd())
            {
                nextID = IDC_EDIT_NEW_PASSWD;
            }
            else
            {
                nextID = IDC_EDIT_OLD_PASSWD;
            }
            break;

        case IDC_EDIT_NEW_PASSWD:           
            nextID = IDC_EDIT_CONFIRM_PASSWD;           
            break;

        case IDC_EDIT_CONFIRM_PASSWD:
            if (ConfirmNewPasswd())
            {
                if (VerifyOldPasswd())
                {
                    nextID = ID_CHG_PASSWD_OK;
                }
                else
                {
                    nextID = IDC_EDIT_OLD_PASSWD;
                    SetDlgItemText(IDC_EDIT_OLD_PASSWD, "");
                    MessageBox("Incorrect old password, please try again!!", "Warning", MB_ICONWARNING);
                }
            }
            else
            {
                nextID = IDC_EDIT_NEW_PASSWD;
                SetDlgItemText(IDC_EDIT_NEW_PASSWD, "");
                SetDlgItemText(IDC_EDIT_CONFIRM_PASSWD, "");
                MessageBox("New passwd not the same with confirm passwd!!", "Warning", MB_ICONWARNING);
            }
            break;
        }       
    }
    
    if (nextID != ID_CHG_PASSWD_OK)
    {
        GotoDlgCtrl(GetDlgItem(nextID));        
        return TRUE;
    }
    else
    {
	    return CDialog::PreTranslateMessage(pMsg);
    }
}

BOOL CChgPasswdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GotoDlgCtrl(GetDlgItem(IDC_EDIT_OLD_PASSWD));
    return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
