
// MFCVideoDlg.h: 头文件
//
#include"GeneralDef.h"
#if !defined(_VIDEO_PLAY_DEMO_)
#define _VIDEO_PLAY_DEMO_
//头文件写法还需要看看
#pragma once


// CMFCVideoDlg 对话框
class CMFCVideoDlg : public CDialogEx
{
// 构造
public:
	CMFCVideoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCVIDEO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// IP地址控件
	CIPAddressCtrl ip_control;
	CString UserName;
	CString Password;
	CString Port;
	LONG UserID=-1;//-1是不可能值，我记得登陆之后的返回值是非负的，所以小于零说明未登录或登陆失败2019/2/27 -张弓

	short iPChannel;
	short m_nLoginPort;
	//不对吧，这里这么定义的话，当这个头文件被两个及以上的文件包含的时候会报错的吧 不是extern short a 吗？
	//奇怪的是没有报错，是因为我们有那个ifndef吗？ 2019/2/26 -张弓
	afx_msg void OnBnClickedButtonLogin();
	// 关联输入框
	CEdit edit_user;
	CEdit edit_port;
	CEdit edit_password;
	afx_msg void OnBnClickedButtonShwopic();
	// 图片显示区域
	CStatic winStaticShowPic;
	afx_msg void OnBnClickedButtonSavevideo();
	void StartPlay(int iChanIndex);
	void StartRecord();
	void StopPlay();
	void StopRecord();
	int m_iCurChanIndex = -1;  //当前通道在数组中索引
	LONG m_lPlayHandle;  //通道句柄
	BOOL m_bIsPlaying;
	BOOL m_bIsRecording;
	LOCAL_DEVICE_INFO m_struDeviceInfo;
	afx_msg void OnBnClickedButtonstartplay2();
};
#endif