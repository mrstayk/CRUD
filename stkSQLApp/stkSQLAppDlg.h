#pragma once
#include "SQLClass.h"
#include <string>

namespace ListContent { 
	enum ListContents {
		NONE,
		COMPANIES,
		EMPLOYEES,
		OFFICES
	};

	enum ListColumnsCompanies {
		COMPANYID,
		NAME,
		DATE
	};

	enum ListColumnsEmployees {
		EMPLOYEEID,
		FIRSTNAME,
		LASTNAME,
		STARTINGDATE,
		SALARY,
		VACATIONDAYS,
		POSITION,
		EFKCOMPANYID,
		EFKOFFICEID,
		EFKBOSSID
	};

	enum ListColumnsOffices {
		OFFICEID,
		COUNTRY,
		CITY,
		STREET,
		STREETNUM,
		HEADQUARTERS,
		OFKCOMPANYID
	};
}

// stkSQLAppDlg dialog
class stkSQLAppDlg : public CDialogEx
{
// Construction
public:
	stkSQLAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STKSQLAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void PopulateList();
	void ResetList();
	void RePopulateList();

	SQLClass m_SQLConnector; 
	CListCtrl m_ListCtrl;
	ListContent::ListContents m_Showing;
	
	CEdit m_EditCompanyName;
	CEdit m_EditCompanyDate;

	CEdit m_EditEmployeeFirstName;
	CEdit m_EditEmployeeLastName;
	CEdit m_EditEmployeeStartingDate;
	CEdit m_EditEmployeeSalary;
	CEdit m_EditEmployeeVacationDays;
	CEdit m_EditEmployeePosition;
	CEdit m_EditEmployeeCompanyID;
	CEdit m_EditEmployeeOfficeID;
	CEdit m_EditEmployeeBossID;
	
	CEdit m_EditOfficeCountry;
	CEdit m_EditOfficeCity;
	CEdit m_EditOfficeStreet;
	CEdit m_EditOfficeStreetNum;
	CEdit m_EditOfficeHeadquarters;
	CEdit m_EditOfficeCompanyID;

	CEdit m_EditServer;
	CEdit m_EditUsername;
	CEdit m_EditPassword;
	CEdit m_EditDBName;

	afx_msg void OnBnClickedAddCompany();
	afx_msg void OnBnClickedButtonAddEmployee();
	afx_msg void OnBnClickedButtonAddOffice();

	afx_msg void OnBnClickedShowCompanies();
	afx_msg void OnBnClickedButtonShowOffices();
	afx_msg void OnBnClickedButtonShowEmployees();

	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonDel();

	afx_msg void OnBnClickedButtonConnect();

	void PopulateCompanies();
	void PopulateEmployees();
	void PopulateOffices();

	void PopulateCompany(Company, int);
	void PopulateEmployee(Employee, int);
	void PopulateOffice(Office, int);
};
