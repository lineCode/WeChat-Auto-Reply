
// WeChatAutoReplyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WeChatAutoReply.h"
#include "WeChatAutoReplyDlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <Psapi.h>
#include <string.h>
#include <assert.h>
#include "shell.h"
#include "tools.h"
#include "divide.h"
#include "log.h"
#pragma comment ( lib, "Psapi.lib" )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CString Rebecca_exec_path;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWeChatDlg �Ի���



CWeChatDlg::CWeChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WECHATAUTOREPLY_DIALOG, pParent)
	, m_replyStateRadioGroup(0)
	, GetMsg(_T(""))
	, SendMsg(_T(""))
	, first_flag(true)
	, process_flag(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	lmshwnd = NULL;
	LastSendMsg = _T("");
	//  Rebecca_exec_path = _T("");
}

void CWeChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_REPLYTRUE, m_replyStateRadioGroup);
}

BEGIN_MESSAGE_MAP(CWeChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWeChatDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_START, &CWeChatDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_REPLYTRUE, &CWeChatDlg::OnClickedReplyStateRadioGroup)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CWeChatDlg ��Ϣ��������

BOOL CWeChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWeChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWeChatDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWeChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWeChatDlg::OnBnClickedOk()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CDialogEx::OnOK();
}




void CWeChatDlg::OnBnClickedStart()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	//wcout.imbue(locale("chs"));
	//setlocale(LC_ALL, "chs");
	//readFile();
	process_flag = true;
	readProperty();
	//printf("Loading Rebecca AIML files......\n");
	//if (!loadRebecca())
	//	return 1;
	writeLog(L"Program Start.", "wechat-_tmain()", START);
	bool first_flag = true;
	// �õ�����ID���б�
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	//�о����н��̵�ID�����ص�aProcesses������
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return;
	//����һ�������˶��ٸ�����ID
	cProcesses = cbNeeded / sizeof(DWORD);
	//CString str=L"";
	int len = 0;
	UpdateData(TRUE);
	if (m_replyStateRadioGroup == 1)
		getMsg(LastSendMsg, len); 
	SetTimer(RECVMSG, 1000, NULL);
}




void CWeChatDlg::OnClickedReplyStateRadioGroup()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	//MessageBox(LastSendMsg);
}

void CWeChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	int GetMsgLen = 0;
	bool send_flag = false;
	if (process_flag && nIDEvent == RECVMSG)
	{
		if (getMsg(GetMsg, GetMsgLen))
		{
			send_flag = getResponse(GetMsg, SendMsg, first_flag);
			if (first_flag)
			{
				sendMsg(L"Hello, I am Alice, the intelligent AI chatting with English. At the moment, my master is busy so I will help him reply the requests. Besides, you can leave messages and I will inform him.");
				int count = 0;
				for (int i = 0; i < wcslen(SendMsg); i++)
					if (SendMsg[i] == L' ')
						count++;
				if (count >= 3 && send_flag)
					sendMsg(SendMsg);
			}
			else if (send_flag)
				sendMsg(SendMsg);

			first_flag = false;
		}

	}
	CDialogEx::OnTimer(nIDEvent);
}

bool CWeChatDlg::getResponse(CString GetMsg, CString& SendMsg, bool first)
{
	//memset(SendMsg, 0, MSG_SIZE + 1);
	SendMsg.Empty();
	//wchar_t GetMsg_unicode[MSG_SIZE + 1] = { 0 };
	char SendMsg_ansi[MSG_SIZE + 1] = { 0 };
	//wchar_t query_property[MSG_SIZE + 1] = { 0 };
	CString GetMsg_unicode = L"";
	CString query_property = L"";

	int lang = checkLang(GetMsg);

	char temp[MAXCHARSIZE] = { 0 };
	switch (lang)
	{
	case 0: printf("Input language: English\n");
		//UnicodeToANSI(GetMsg, GetMsg_ansi, MSG_SIZE);
		//wcscpy_s(GetMsg_unicode, wcslen(GetMsg) + 1, GetMsg);
		GetMsg_unicode = GetMsg;
		query_property = L"-ppf \""+Rebecca_exec_path+L"\\..\\..\\conf\\properties.xml\"" ;
		//wsprintf(query_property, L"-ppf \"%s\\..\\..\\conf\\properties.xml\"", Rebecca_exec_path);
		shell(query_property, temp, MAXCHARSIZE);

		break;
	case 1: printf("�������Z:�ձ��Z\n");
		//wcscpy_s(SendMsg, 68, L"Sorry, I can only understand English. Talk with me in English. THX~");
		SendMsg = L"Sorry, I can only understand English. Talk with me in English. THX~";
		sendMsg(SendMsg);
		//memset(SendMsg, 0, MSG_SIZE + 1);
		SendMsg.Empty();
		UnicodeStr2wchar(GetMsg, GetMsg_unicode, MSG_SIZE);//�����ת�������������֣�ÿ����һ�η���
		query_property = L"-ppf \"" + Rebecca_exec_path + L"\\..\\..\\conf\\properties.xml\"";
		shell(query_property, temp, MAXCHARSIZE);
		break;
	case 2: printf("�������ԣ�����\n");
		SendMsg = L"Sorry, I can only understand English. Talk with me in English. THX~";
		sendMsg(SendMsg);
		SendMsg.Empty();
		UnicodeStr2wchar(GetMsg, GetMsg_unicode, MSG_SIZE);//�����ת�������������֣�ÿ����һ�η���
		GetMsg_unicode += L"#";//ĩβ���Ͻ�������#������ƥ��AIML�еġ�*��
		query_property = L"-ppf \"" + Rebecca_exec_path + L"\\..\\..\\conf\\properties.xml\"";
		shell(query_property, temp, MAXCHARSIZE);
		break;
	default:
		printf("The language cannot be analyzed\n"); break;
	}
	//printf("GetMsg_ansi:%s\n", GetMsg_ansi);
	//if (GetMsg_ansi != NULL)
	if (!query(GetMsg_unicode, SendMsg_ansi, MSG_SIZE))
		return FALSE;
	if (lang == 0)
		ANSIToUnicode(SendMsg_ansi, SendMsg, MSG_SIZE);
	else
		char2Unicode(SendMsg_ansi, SendMsg, MSG_SIZE);
	//wprintf(L"Input:%ws, Output:%ws\n", GetMsg, SendMsg);
	return TRUE;

	return false;
}


BOOL CWeChatDlg::getMsg(CString& msg, int& len)
{
	openWnd(L"E:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe");

	for (int k = 0; k < 60 && lmshwnd == NULL; k++)
	{
		//if (k>0) Sleep(1000);
		lmshwnd = FindWindowW(L"WeChatMainWndForPC", NULL);
		if (lmshwnd != NULL)
			printf("Get WeChat window\n");
		else
			//printf("Not get WeChat window\n");
			MessageBox(L"û�л��΢�ų���Ĵ���");
	}
	if (lmshwnd == NULL)
	{
		MessageBox(L"û�л��΢�ų���Ĵ���");
		return FALSE;
	}

	RECT rect;
	lmshwnd->GetClientRect(&rect);
	POINT point;
	point.x = 385;
	point.y = abs(rect.bottom - rect.top) -EDIT_HEIGHT-INIT_HEIGHT;
	BOOL flag = true;
	do
	{
		if (OpenClipboard())
		{
			EmptyClipboard();                            //���������������
			CloseClipboard();
		}
		lmshwnd->PostMessageW(WM_LBUTTONDBLCLK, 0x1, MAKELPARAM(point.x, point.y));
		lmshwnd->PostMessageW(WM_LBUTTONUP, 0x0, MAKELPARAM(point.x, point.y));

		KeyAction(0x11, 1);
		KeyAction(0x43, 1);
		KeyAction(0x43, 0);
		KeyAction(0x11, 0);
		Sleep(50);
		flag = getClipboard(msg, len);

		point.y -= STEP_HEIGHT;
	} while (!flag && !process_flag && point.y > 50);

	if (wcscmp(msg, LastSendMsg) == 0)//���һ������һ���ҷ��͵ģ�Ҳ��û�н��յ�����Ϣ
		return FALSE;
	else
	{
		writeLog(msg, "wechat-getMSG()", RECVMSG);
		return TRUE;
	}
}

// ��΢�ŵ�ǰ�Ự������Ϣ
BOOL CWeChatDlg::sendMsg(CString msg)
{
	openWnd(L"E:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe");

	//CWnd* pWnd = NULL;
	//���ܳ���������������Ҫ���Զ�β����ҵ�Ŀ�괰�ڡ���ೢ��60�Σ�ÿ��һ�볢��һ�Σ�����ʧ�����˳�
	for (int k = 0; k < 60 && lmshwnd == NULL; k++)
	{
		if (k>0) Sleep(1000);
		lmshwnd = FindWindowW(L"WeChatMainWndForPC", NULL);
		if (lmshwnd != NULL)
			printf("Get WeChat window\n");
		else
			printf("Not get WeChat window\n");
	}

	//PostMessageW(lmshwnd, WM_KEYDOWN, 0x30, 0xB0001);
	//ȷ�����������λ��
	RECT rect;
	lmshwnd->GetClientRect(&rect);
	POINT point;
	point.x = abs(rect.right - rect.left) - OFFSET_SET_X;
	point.y = abs(rect.bottom - rect.top) - OFFSET_SET_Y;
	//cout << "set point:" << point.x << "," << point.y << endl;
	//wchar_t wstr[255] = { 0 };
	//wsprintf(wstr, L"%d, %d", point.x, point.y);
	//MessageBox(NULL, wstr, L"", 0);
	lmshwnd->PostMessageW(WM_LBUTTONDOWN, 0x1, MAKELPARAM(point.x, point.y));
	lmshwnd->PostMessageW(WM_LBUTTONUP, 0x1, MAKELPARAM(point.x, point.y)/*0x021501A3*/);

	//�������д����
	for (int i = 0; i<lstrlen(msg); i++)
	{
		//printf("%d ", msg[i]);
		lmshwnd->PostMessageW(WM_CHAR, msg[i], 0x1);
	}
	putchar('\n');

	//�������д�س������Է�����Ϣ
	lmshwnd->PostMessageW(WM_KEYDOWN, 0xD, 0x1C0001);
	lmshwnd->PostMessageW(WM_CHAR, 0xD, 0x1C0001);
	lmshwnd->PostMessageW(WM_KEYUP, 0xD, 0x1C0001);
	//wcscpy_s(LastSendMsg, wcslen(msg) + 1, msg);
	LastSendMsg = msg;
	writeLog(msg, "wechat-sendMSG()", SENDMSG);
	return TRUE;
}

BOOL CWeChatDlg::openWnd(const wchar_t* path)
{
	HINSTANCE hRet = 0;
	//HWND hwnd;
	hRet = ShellExecute(*lmshwnd, NULL, path, NULL, NULL, SW_HIDE);
	if ((int)hRet <= 32)
	{
		MessageBoxW(NULL, L"��WeChatʧ�ܣ�", 0);
		return FALSE;
	}
	Sleep(2000);
	return TRUE;
}

// ģ��һ�����Ķ�����vKey �Ǽ��������� Orientation ��ʾ���»����ͷ� , 1���� 0�ͷ�
void CWeChatDlg::KeyAction(int vKey, int Orientation)
{
	INPUT Inputs888;
	SecureZeroMemory(&Inputs888, sizeof(INPUT));

	Inputs888.type = INPUT_KEYBOARD;

	// ki �� 5 ����Ա����3����Ҫ��ֵ
	if (Orientation)
		Inputs888.ki.dwFlags = 0;
	else
		Inputs888.ki.dwFlags = KEYEVENTF_KEYUP;

	Inputs888.ki.wScan = MapVirtualKey(vKey, 0);
	if (Inputs888.ki.wScan == 0) // ʧ�ܾͷ������
	{

		return;
	}
	Inputs888.ki.wVk = vKey;

	UINT ret77 = SendInput(1, &Inputs888, sizeof(INPUT));

	assert(1 == ret77);
}

bool CWeChatDlg::getClipboard(CString& text, int& text_len)
{
	if (OpenClipboard())
	{
		HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
		if (hClipboardData)
		{
			WCHAR *pchData = (WCHAR*)GlobalLock(hClipboardData);
			if (pchData)
			{
				text_len = wcslen(pchData);
				text.Format(L"%ls", pchData);
				//wcscpy_s(text, text_len + 1, pchData);
				wprintf(L"Get clipboard: %ls\n", text);
				GlobalUnlock(hClipboardData);
				CloseClipboard();
				return true;
			}
			else
				wprintf(L"Clipboard text is null.\n");
		}
		CloseClipboard();
	}
	else
		wprintf(L"Cannot open clipboard.\n");
	return false;

}




void CWeChatDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	KillTimer(RECVMSG);
}

void CWeChatDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴�������Ϣ�����������
	if (nState == WA_INACTIVE)
		;// process_flag = true;
	else
		process_flag = false;
}