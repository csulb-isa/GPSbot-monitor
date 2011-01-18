
// RobomagellenDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CRobomagellenDlg dialog
class CRobomagellenDlg : public CDialogEx
{
// Construction
public:
	CRobomagellenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ROBOMAGELLEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComPortSelect;
	CEdit m_GpsSentece;
	afx_msg void OnBnClickedComConnect();
	CEdit m_TxMsg;
	afx_msg void OnBnClickedTx();
	CEdit m_Sonar1;
	CEdit m_Sonar2;
	CEdit m_Sonar3;
	CEdit m_Sonar4;
	CEdit m_Infr1;
	CEdit m_Infr2;
	CEdit m_TravelSpeed;
	afx_msg void OnNMReleasedcaptureSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSteerSlider(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SpeedCtrl;
	CSliderCtrl m_SteerCtrl;
	CEdit m_Pvalue;
	CEdit m_Ivalue;
	CEdit m_Dvalue;
	CEdit m_Pgain;
	CEdit m_Igain;
	CEdit m_Dgain;
	afx_msg void OnBnClickedUpdateGain();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_heading;
	CEdit m_heading_x;
	CEdit m_heading_y;
	CEdit m_heading_z;
	CEdit m_nmea_gga;
	CEdit m_nmea_gll;
	CEdit m_nmea_vtg;
	CEdit m_nmea_rmc;
};

// Timer event IDs
#define PARSE_RX_DATA_TIMER_EVENT	100
#define UPDATE_SPEED_STEER			102
#define TEST						101

