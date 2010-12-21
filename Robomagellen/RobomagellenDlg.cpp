
// RobomagellenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Robomagellen.h"
#include "RobomagellenDlg.h"
#include "afxdialogex.h"
#include "com port.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRobomagellenDlg dialog


CRobomagellenDlg::CRobomagellenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRobomagellenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobomagellenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_PORT_SELECT, m_ComPortSelect);
	DDX_Control(pDX, IDC_GPS_SENTENCE, m_GpsSentece);
	DDX_Control(pDX, IDC_USER_INPUT, m_TxMsg);
	DDX_Control(pDX, IDC_SONAR_1, m_Sonar1);
	DDX_Control(pDX, IDC_SONAR_2, m_Sonar2);
	DDX_Control(pDX, IDC_SONAR_3, m_Sonar3);
	DDX_Control(pDX, IDC_SONAR_4, m_Sonar4);
	DDX_Control(pDX, IDC_INFR_1, m_Infr1);
	DDX_Control(pDX, IDC_INFR_2, m_Infr2);
	DDX_Control(pDX, IDC_SPEED, m_TravelSpeed);
	DDX_Control(pDX, IDC_SPEED_SLIDER, m_SpeedCtrl);
	DDX_Control(pDX, IDC_STEER_SLIDER, m_SteerCtrl);
	DDX_Control(pDX, IDC_P_VALUE, m_Pvalue);
	DDX_Control(pDX, IDC_I_VALUE, m_Ivalue);
	DDX_Control(pDX, IDC_D_VALUE, m_Dvalue);
	DDX_Control(pDX, IDC_P_GAIN, m_Pgain);
	DDX_Control(pDX, IDC_I_GAIN, m_Igain);
	DDX_Control(pDX, IDC_D_GAIN, m_Dgain);
	DDX_Control(pDX, IDC_HEADING, m_heading);
	DDX_Control(pDX, IDC_COMPASS_X, m_heading_x);
	DDX_Control(pDX, IDC_COMPASS_Y, m_heading_y);
	DDX_Control(pDX, IDC_COMPASS_Z, m_heading_z);
	DDX_Control(pDX, IDC_NMEA_GGA, m_nmea_gga);
	DDX_Control(pDX, IDC_NMEA_GLL, m_nmea_gll);
	DDX_Control(pDX, IDC_NMEA_VTG, m_nmea_vtg);
	DDX_Control(pDX, IDC_NMEA_RMC, m_nmea_rmc);
}

BEGIN_MESSAGE_MAP(CRobomagellenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COM_CONNECT, &CRobomagellenDlg::OnBnClickedComConnect)
	ON_BN_CLICKED(IDC_TX, &CRobomagellenDlg::OnBnClickedTx)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SPEED_SLIDER, &CRobomagellenDlg::OnNMReleasedcaptureSpeedSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_STEER_SLIDER, &CRobomagellenDlg::OnNMReleasedcaptureSteerSlider)
	ON_BN_CLICKED(IDC_BUTTON3, &CRobomagellenDlg::OnBnClickedUpdateGain)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRobomagellenDlg message handlers

BOOL CRobomagellenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// This will popluate the drop down list for com
	// port selection only at app generation - Add refresh button??
	char s[24] = {""};
	int write_loc = 0;
	for(int cnt = 0; cnt<=255; cnt++)
	{
		if (ComIsAvailable(cnt+1) == 1){
			_itoa_s((cnt+1), s, 10);
			CString ss(s);
			m_ComPortSelect.InsertString(write_loc++, ss);
		}
	}
	// Set the sliders for the steering and speed to 0
	// Also initialize the values they return
	m_SteerCtrl.SetTicFreq(25);
	m_SteerCtrl.SetRange(-127, 127, TRUE);
	m_SteerCtrl.SetPos(0);
	m_SpeedCtrl.SetTicFreq(25);
	m_SpeedCtrl.SetRange(-127, 127, TRUE);
	m_SpeedCtrl.SetPos(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRobomagellenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRobomagellenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRobomagellenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRobomagellenDlg::OnBnClickedComConnect()
{
	// TODO: Add your control notification handler code here
	static int do_once = 0;
	int com_choice = 99;
	wchar_t s[99];
	// we are already connected
	if (do_once){
		com_choice = MessageBox(L"Com port already connected.\r\n"
				L"Disconnect from current COM device??", 
				L"Robomagellen Message", (MB_ICONEXCLAMATION | MB_YESNO));
		if (com_choice == 6){
			WincomClose();
			do_once = 0;
			m_GpsSentece.SetWindowTextW(L"");
			m_TxMsg.SetWindowTextW(L"");
			m_ComPortSelect.SetWindowTextW(L"");
		}
		return;
	}
	com_choice = m_ComPortSelect.GetWindowTextW(s, 99);
	if (s[0] == NULL)
	{
		// popup message to indicate error or event
		com_choice = MessageBox(L"No Com port selected", L"Robomagellen Error", MB_ICONEXCLAMATION);
		SetTimer(TEST, 1000, NULL);
	}
	else
	{
		CString com_s("COM");
		com_s.Append(s);
		if ( WincomInit( (wchar_t*) com_s.GetString() ) < 0){
			com_choice = MessageBox(L"Com port unavailable", L"Robomagellen Error", MB_ICONEXCLAMATION);
		}
		else{
			SetTimer(PARSE_RX_DATA_TIMER_EVENT, 100, NULL);
			do_once = 1;
		}
	}
	WriteCom(L"Connecting to device");
}

void CRobomagellenDlg::OnBnClickedTx()
{
	wchar_t data[1024] = {NULL};
	m_TxMsg.GetWindowTextW(data, 1024);
	if (WriteCom(data)){
		m_TxMsg.SetWindowTextW(L"");
	}
}

void CRobomagellenDlg::OnNMReleasedcaptureSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	// Returns an inverted value - up is negative and down is positive
	int tmp = m_SpeedCtrl.GetPos();
	char cmd[256] = {NULL};
	// Invert the result
	if (tmp < 0)
		tmp = abs(tmp);
	else
		tmp -= (tmp*2);
	sprintf(cmd, "[COMMAND<SPEED>%d]\n", tmp);
	WriteCom(cmd);
	*pResult = 0;
}


void CRobomagellenDlg::OnNMReleasedcaptureSteerSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int tmp = m_SteerCtrl.GetPos();
	char cmd[256] = {NULL};
	sprintf(cmd, "[COMMAND<STEER>%d]\n", tmp);
	WriteCom(cmd);
	*pResult = 0;
}


void CRobomagellenDlg::OnBnClickedUpdateGain()
{
	// TODO: Add your control notification handler code here
}


void CRobomagellenDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	wchar_t data[4096] = {NULL}; 
	// stop the timer so that the parse time does not take longer than
	// an event timeout
	KillTimer(nIDEvent);		
	if (nIDEvent == PARSE_RX_DATA_TIMER_EVENT){	
		// see if there was anything on the comport
		if (ReadCom(data)){
			// this will store a packet that was pulled but not complete
			static CString s;
			s.Append(data);
			int loc = s.FindOneOf(L"[");
			// left justify the packet
			s = s.Right((s.GetLength()-loc));
			// look for the end of a packet
			int packets_available = 0;
			loc = 0;
			// how many packets are in the buffer
			while (1){
				loc = s.Find(L"]", loc);
				if (loc > 0)
					packets_available++;
				else
					break;
			}
			// service all packets that have been recieved
			for (int i=1; i<packets_available; i++)
			{
				if (s.Find(L"[STATUS<SONAR_L1>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SONAR_L1>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_Sonar1.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<SONAR_L2>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SONAR_L2>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_Sonar2.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<SONAR_R2>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SONAR_R2>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_Sonar3.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<SONAR_R1>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SONAR_R1>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_Sonar4.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<IR1>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<IR1>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_Infr1.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<IR2>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<IR2>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_Infr2.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<SPEED>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SPEED>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_TravelSpeed.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<HEADING>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<HEADING>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_heading.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<HEADING-X>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<HEADING-X>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_heading_x.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<HEADING-Y>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<HEADING-Y>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_heading_y.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<HEADING-Z>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<HEADING-Z>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_heading_z.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<GPS-GGA>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<GPS-GGA>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_nmea_gga.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<GPS-GLL>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<GPS-GLL>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_nmea_gll.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<GPS-RMC>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<GPS-RMC>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_nmea_rmc.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<GPS-VTG>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<GPS-VTG>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
					m_nmea_vtg.SetWindowTextW(s_disp);
					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<SERVO_STEER>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SERVO_STEER>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
				
					// make into hex and use setpos() to update the debug console

					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
				else if (s.Find(L"[STATUS<SERVO_SPEED>") == 0)
				{
					// delete the packet header and left justify the parameter
					s = s.Right((s.GetLength()-strlen("[STATUS<SERVO_SPEED>")));
					// find out how big the parameter is 
					loc = s.Find(L"]");
					CString s_disp(s);
					// store the parameter in a temporary string object
					s_disp = s_disp.Left(loc);
					// FORMAT or just put the parameter on the debug console
				
					// make into hex and use setpos() to update the debug console

					// find the next start of packet
					loc = s.Find(L"[");
					// left justify the next packet
					s = s.Right((s.GetLength()-loc));
				}
			}
		}
		// before the exit - restart the timer
		SetTimer(PARSE_RX_DATA_TIMER_EVENT, 100, NULL);
	}
	else if (nIDEvent == TEST){
		SetTimer(TEST, 1000, NULL);
		// Just for debugging to make sure the timer is working
		MessageBeep(0xffffffff);
	}

	CDialogEx::OnTimer(nIDEvent);
}
