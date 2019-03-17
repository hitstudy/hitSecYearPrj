
// MFCVideoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCVideo.h"
#include "MFCVideoDlg.h"
#include "afxdialogex.h"
#include "interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

;class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCVideoDlg 对话框



CMFCVideoDlg::CMFCVideoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCVIDEO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, ip_control);
	DDX_Control(pDX, IDC_EDIT_UserName, edit_user);
	DDX_Control(pDX, IDC_EDIT_Port, edit_port);
	DDX_Control(pDX, IDC_EDIT_Password, edit_password);
	DDX_Control(pDX, IDC_STATIC_shwoPic, winStaticShowPic);
}

BEGIN_MESSAGE_MAP(CMFCVideoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Login, &CMFCVideoDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_ShwoPic, &CMFCVideoDlg::OnBnClickedButtonShwopic)
	ON_BN_CLICKED(IDC_BUTTON_SaveVideo, &CMFCVideoDlg::OnBnClickedButtonSavevideo)
	ON_BN_CLICKED(IDC_BUTTON_startPlay2, &CMFCVideoDlg::OnBnClickedButtonstartplay2)
END_MESSAGE_MAP()


// CMFCVideoDlg 消息处理程序

BOOL CMFCVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	NET_DVR_Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCVideoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCVideoDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCVideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString IPToStr(DWORD dwIP)
{
	CString strIP = _T("");
	WORD add1, add2, add3, add4;

	add1 = (WORD)(dwIP & 255);
	add2 = (WORD)((dwIP >> 8) & 255);
	add3 = (WORD)((dwIP >> 16) & 255);
	add4 = (WORD)((dwIP >> 24) & 255);
	strIP.Format("%d.%d.%d.%d", add4, add3, add2, add1);
	return strIP;
}       
//把数组后16位清零，保证最大长度？
bool setzero(char* p)
{
	for (int i = 99; i > 16; i--)
	{
		*(p+i) = 0;
	}
	return true;
}

void CMFCVideoDlg::OnBnClickedButtonLogin()
{
	// TODO: 登陆按钮
	DWORD dwDeviceIP;
	char DeviceIP[16] = { 0 };
	char cUserName[100] = { 0 };
	char cPassword[100] = { 0 };
	CString csTemp;
	//获取用户输入
	ip_control.GetAddress(dwDeviceIP);
	edit_password.GetWindowTextA(Password);
	edit_port.GetWindowTextA(Port);
	edit_user.GetWindowTextA(UserName);
	//转换
	csTemp = IPToStr(dwDeviceIP);
	//strcpy_s(DeviceIP, csTemp);
	sprintf_s(DeviceIP, "%s", csTemp.GetBuffer(0));
	//strcpy_s(cUserName, UserName);
	//strcpy_s(cPassword,Password);
	strncpy_s(cUserName, UserName, MAX_NAMELEN);
	strncpy_s(cPassword, Password, PASSWD_LEN);
	m_nLoginPort = (short)_ttoi(Port);
	m_nLoginPort = 8000;
	//16位后清零，这里会引发访问权限错误，但是我程序应该没写错吧，怎么回事。2019/2/25 -张弓
	//setzero(DeviceIP);
	setzero(cUserName);
	setzero(cPassword);
	//登陆设备
	NET_DVR_DEVICEINFO_V30 devInfo;
	UserID = NET_DVR_Login_V30(DeviceIP, m_nLoginPort, cUserName, cPassword, &devInfo);//这句无法解析的外部符号，应该是依赖库的问题，但是我尝试添加依赖库却失败了 2019/2/24-张弓
	//后来添加依赖库成功了，但是提示这句引发了访问权限冲突，我在想是不是字符数组的长度或者什么不对导致数组越界之类的错误了。
	//是因为我这样全部置零之后，字符数组最后没有空字符导致栈溢出？可是字符数组本来也没有空字符吧    2019/2/24-张弓
	//这里取消16位之后清零就一切正常了，这是为什么2019/2/25 -张弓
	//注意，这里不知道用户名和密码长度大于16位之后会是什么效果，可能引发莫名的bug 2019/2/26 -张弓
	
	DWORD dwReturned = 0;
	DWORD shwoError = NET_DVR_GetLastError();
	if (UserID < 0)
	{
		AfxMessageBox("Login failed!");
		
	}
		
	else
	{
		AfxMessageBox("Login successfully!");
	}
	return;
}


void CMFCVideoDlg::OnBnClickedButtonShwopic()
{
	// TODO: 这里是尝试点击一次按钮就显示一张图片，如果成功的话就把函数的点击事件放到一个线程的循环里 2019/2/26 -张弓
	if (UserID < 0)
	{	
		AfxMessageBox("请登录");
		return;
	}
	int x, y;
	x = y = 0;
	int picSize;
	LPVOID p_pic = getPic(picSize, UserID, ipChannel);
	HGLOBAL h_global = getGlobalHandle(p_pic, picSize);
	ShowJpgGif( winStaticShowPic.GetDC(),h_global, picSize, x, y);
}





void CMFCVideoDlg::OnBnClickedButtonSavevideo()
{
	// TODO: 录像保存按钮
	if (m_lPlayHandle == -1)
	{
		MessageBox("请先选择一个通道播放");
		return;
	}
	if (!m_bIsRecording)
	{
		StartRecord();
	}
	else
	{
		StopRecord();
	}
}
void CMFCVideoDlg::StartPlay(int iChanIndex)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd = GetDlgItem(IDC_STATIC_showVideo)->m_hWnd;
	ClientInfo.lChannel = m_iCurChanIndex + 1;
	ClientInfo.lLinkMode = 0;
	ClientInfo.sMultiCastIP = NULL;
	TRACE("Channel number:%d\n", ClientInfo.lChannel);
	//m_lPlayHandle = NET_DVR_RealPlay_V30(UserID, &ClientInfo, NULL, NULL, TRUE);
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID, &ClientInfo, NULL, NULL, TRUE);
	if (-1 == m_lPlayHandle)
	{
		DWORD err = NET_DVR_GetLastError();
		CString m_csErr;
		m_csErr.Format("播放出错，错误代码%d", err);
		MessageBox(m_csErr);
	}

	m_bIsPlaying = TRUE;
	//GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("停止播放");

}

void CMFCVideoDlg::StartRecord()
{
	char RecName[256] = { 0 };

	CTime CurTime = CTime::GetCurrentTime();;
	sprintf_s(RecName, "G:\\程序\\cpp\\MFC\\Video\\MFCVideo\\videoREC\\%04d%02d%02d%02d%02d%02d_ch.mp4", CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(), \
		CurTime.GetHour(), CurTime.GetMinute(), CurTime.GetSecond()
							/*, m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex    对应%02d*/);

	if (!NET_DVR_SaveRealData(m_lPlayHandle, RecName))
	{
		MessageBox("启动录像失败");
		return;
	}
	m_bIsRecording = TRUE;
	GetDlgItem(IDC_BUTTON_SaveVideo)->SetWindowText("停止录像");
}
void CMFCVideoDlg::StopPlay()
{
	if (m_lPlayHandle != -1)
	{
		if (m_bIsRecording)  //正在录像，先停止
		{
			StopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle);
		m_lPlayHandle = -1;
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_STATIC_showVideo)->Invalidate();
	}
}

void CMFCVideoDlg::StopRecord()
{
	if (!NET_DVR_StopSaveRealData(m_lPlayHandle))
	{
		MessageBox("停止录像失败");
		return;
	}
	m_bIsRecording = FALSE;
	GetDlgItem(IDC_BUTTON_SaveVideo)->SetWindowText("录像");
}

void CMFCVideoDlg::OnBnClickedButtonstartplay2()
{
	// TODO: 原视频播放按钮
	m_iCurChanIndex = 1;
	if (m_iCurChanIndex == -1)
	{
		MessageBox("选择一个通道");
		return;
	}
	if (!m_bIsPlaying)
	{
		StartPlay(m_iCurChanIndex);
		m_bIsPlaying = TRUE;
		GetDlgItem(IDC_BUTTON_startPlay2)->SetWindowText("停止播放(2)");

	}
	else
	{
		StopPlay();
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_BUTTON_startPlay2)->SetWindowText("播放(2)");
	}
}
