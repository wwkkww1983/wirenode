//#include "c:\Users\peiwen\documents\visual studio 2013\Projects\视觉目标检测\视觉目标检测\vedio\ACom.h"
// MFCApplication1Dlg.h : 头文件
//
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "tchart1.h"
#include "nodelist.h"

using namespace std;
// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialog
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void PaintCoordinateSystem(int x_min, int x_step, int y_min, int y_step);

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
	afx_msg void OnBnClickedOk();

private:
	static int convert(string strr, unsigned char* buffer);
	CListCtrl mlist;

public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	CTchart1 m_chart;

	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton2();
};
