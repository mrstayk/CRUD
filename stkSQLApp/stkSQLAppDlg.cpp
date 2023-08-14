#include "pch.h"
#include "framework.h"
#include "stkSQLApp.h"
#include "stkSQLAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// C++20 feature.
using enum SQLDB::EmployeePos;

stkSQLAppDlg::stkSQLAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STKSQLAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Showing = ListShowing::NONE;
}

void stkSQLAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_EDIT_COMAPNYNAME, m_EditCompanyName);
	DDX_Control(pDX, IDC_EDIT_COMPANYDATE, m_EditCompanyDate);

	DDX_Control(pDX, IDC_EDIT_EMPLOYEEFIRSTNAME, m_EditEmployeeFirstName);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEELASTNAME, m_EditEmployeeLastName);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEDATE, m_EditEmployeeStartingDate);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEESALARY, m_EditEmployeeSalary);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEVACATIONDAYS, m_EditEmployeeVacationDays);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEPOSITION, m_EditEmployeePosition);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEECOMPID, m_EditEmployeeCompanyID);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEOFFICEID, m_EditEmployeeOfficeID);
	DDX_Control(pDX, IDC_EDIT_EMPLOYEEBOSSID, m_EditEmployeeBossID);

	DDX_Control(pDX, IDC_EDIT_OFFICECOUNTRY, m_EditOfficeCountry);
	DDX_Control(pDX, IDC_EDIT_OFFICECITY, m_EditOfficeCity);
	DDX_Control(pDX, IDC_EDIT_OFFICESTREET, m_EditOfficeStreet);
	DDX_Control(pDX, IDC_EDIT_OFFICESTREETNUM, m_EditOfficeStreetNum);
	DDX_Control(pDX, IDC_EDIT_OFFICEHEADQUARTERS, m_EditOfficeHeadquarters);
	DDX_Control(pDX, IDC_EDIT_OFFICECOMPANYID, m_EditOfficeCompanyID);

	DDX_Control(pDX, IDC_EDIT_SERVER, m_EditServer);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_EditUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_EditPassword);
	DDX_Control(pDX, IDC_EDIT_DBNAME, m_EditDBName);
}

BEGIN_MESSAGE_MAP(stkSQLAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADDCOMPANY, &stkSQLAppDlg::OnBnClickedAddCompany)
	ON_BN_CLICKED(IDC_BUTTON_ADDEMPLOYEE, &stkSQLAppDlg::OnBnClickedButtonAddEmployee)
	ON_BN_CLICKED(IDC_BUTTON_ADDOFFICE, &stkSQLAppDlg::OnBnClickedButtonAddOffice)
	ON_BN_CLICKED(IDC_BUTTON_SHOWCOMPANIES, &stkSQLAppDlg::OnBnClickedShowCompanies)
	ON_BN_CLICKED(IDC_BUTTON_SHOWEMPLOYEES, &stkSQLAppDlg::OnBnClickedButtonShowEmployees)
	ON_BN_CLICKED(IDC_BUTTON_SHOWOFFICES, &stkSQLAppDlg::OnBnClickedButtonShowOffices)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &stkSQLAppDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &stkSQLAppDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &stkSQLAppDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()

BOOL stkSQLAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Set the selection style.
	CWnd* win = this->GetDlgItem(IDC_LIST);
	ListView_SetExtendedListViewStyle(win->m_hWnd, LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void stkSQLAppDlg::OnBnClickedButtonConnect()
{
	CString server;
	m_EditServer.GetWindowTextW(server);
	CString username;
	m_EditUsername.GetWindowTextW(username);
	CString password;
	m_EditPassword.GetWindowTextW(password);
	CString dbname;
	m_EditDBName.GetWindowTextW(dbname);

	m_SQLConnector.Connect(std::string(CStringA(server))
						 , std::string(CStringA(username))
						 , std::string(CStringA(password))
						 , std::string(CStringA(dbname)));

}

void stkSQLAppDlg::OnPaint()
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
HCURSOR stkSQLAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void stkSQLAppDlg::PopulateList()
{
	switch (m_Showing) {
		case ListShowing::COMPANIES: {
			PopulateCompanies();
		} break;
		case ListShowing::EMPLOYEES: {
			PopulateEmployees();
		} break;
		case ListShowing::OFFICES: {
			PopulateOffices();
		} break;
		default: break;
	}
}

void stkSQLAppDlg::ResetList() 
{
	m_Showing = ListShowing::NONE;
	m_ListCtrl.DeleteAllItems();

	int column_count = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	// Delete all of the columns.
	for (int i = 0; i < column_count; i++) {
		m_ListCtrl.DeleteColumn(0);
	}

	UpdateData(FALSE);
}

void stkSQLAppDlg::RePopulateList()
{
	ListShowing m_ShowingCurrent = m_Showing;
	ResetList();
	m_Showing = m_ShowingCurrent;
	PopulateList();
}

void stkSQLAppDlg::OnBnClickedAddCompany()
{
	if (!m_SQLConnector.IsConnected()) return;

	// Prepare strings by reading text fields. 
	CString name;
	m_EditCompanyName.GetWindowTextW(name);
	CString creation_date;
	m_EditCompanyDate.GetWindowTextW(creation_date);

	if (name.IsEmpty() || creation_date.IsEmpty()) {
		return;
	}

	Company company;
	company.name = std::string(CStringA(name));
	company.creation_date = std::string(CStringA(creation_date));

	m_SQLConnector.AddCompany(company);

	m_Showing = ListShowing::COMPANIES;
	RePopulateList();
}

void stkSQLAppDlg::OnBnClickedButtonAddEmployee()
{
	if (!m_SQLConnector.IsConnected()) return;

	// Prepare strings by reading text fields. 
	CString first_name;
	m_EditEmployeeFirstName.GetWindowTextW(first_name);
	CString last_name;
	m_EditEmployeeLastName.GetWindowTextW(last_name);
	CString starting_date;
	m_EditEmployeeStartingDate.GetWindowTextW(starting_date);
	CString salary;
	m_EditEmployeeSalary.GetWindowTextW(salary);
	CString vacation_days;
	m_EditEmployeeVacationDays.GetWindowTextW(vacation_days);
	CString position;
	m_EditEmployeePosition.GetWindowTextW(position);

	CString companyid;
	m_EditEmployeeCompanyID.GetWindowTextW(companyid);
	CString officeid;
	m_EditEmployeeOfficeID.GetWindowTextW(officeid);
	CString bossid;
	m_EditEmployeeBossID.GetWindowTextW(bossid);

	if (first_name.IsEmpty()
	|| last_name.IsEmpty()
	|| starting_date.IsEmpty()
	|| salary.IsEmpty()
	|| vacation_days.IsEmpty()
	|| position.IsEmpty()
	|| companyid.IsEmpty()
	|| officeid.IsEmpty()
	|| bossid.IsEmpty()) {
		return;
	}

	Employee employee;
	employee.first_name = std::string(CStringA(first_name));
	employee.last_name = std::string(CStringA(last_name));
	employee.starting_date = std::string(CStringA(starting_date));
	employee.salary = std::stoi(std::string(CStringA(salary)));
	employee.vacation_days = std::stoi(std::string(CStringA(vacation_days)));
	employee.position = (SQLDB::EmployeePos)std::stoi(std::string(CStringA(position)));

	employee.companyid = std::stoi(std::string(CStringA(companyid)));
	employee.officeid = std::stoi(std::string(CStringA(officeid)));
	employee.bossid = std::stoi(std::string(CStringA(bossid)));
	
	m_SQLConnector.AddEmployee(employee);

	m_Showing = ListShowing::EMPLOYEES;
	RePopulateList();
}

void stkSQLAppDlg::OnBnClickedButtonAddOffice()
{
	if (!m_SQLConnector.IsConnected()) return;

	// Prepare strings by reading text fields. 
	CString country;
	m_EditOfficeCountry.GetWindowTextW(country);
	CString city;
	m_EditOfficeCity.GetWindowTextW(city);
	CString street;
	m_EditOfficeStreet.GetWindowTextW(street);
	CString streetnum;
	m_EditOfficeStreetNum.GetWindowTextW(streetnum);
	CString headquarters;
	m_EditOfficeHeadquarters.GetWindowTextW(headquarters);
	CString companyid;
	m_EditOfficeCompanyID.GetWindowTextW(companyid);

	if (country.IsEmpty()
	|| city.IsEmpty()
	|| street.IsEmpty()
	|| streetnum.IsEmpty()
	|| headquarters.IsEmpty()
	|| companyid.IsEmpty()) {
		return;
	}

	Office office;
	office.country = std::string(CStringA(country));
	office.city = std::string(CStringA(city));
	office.street = std::string(CStringA(street));
	office.streetnum = std::stoi(std::string(CStringA(streetnum)));
	office.headquarters = std::stoi(std::string(CStringA(headquarters)));
	office.companyid = std::stoi(std::string(CStringA(companyid)));

	m_SQLConnector.AddOffice(office);

	m_Showing = ListShowing::OFFICES;
	RePopulateList();
}

void stkSQLAppDlg::OnBnClickedShowCompanies()
{
	m_Showing = ListShowing::COMPANIES;
	RePopulateList();
}

void stkSQLAppDlg::OnBnClickedButtonShowEmployees()
{
	m_Showing = ListShowing::EMPLOYEES;
	RePopulateList();
}

void stkSQLAppDlg::OnBnClickedButtonShowOffices() 
{
	m_Showing = ListShowing::OFFICES;
	RePopulateList();
}

void stkSQLAppDlg::PopulateCompanies()
{
	if (!m_SQLConnector.IsConnected()) return;

	auto companies = m_SQLConnector.QueryCompanies();

	m_ListCtrl.InsertColumn(to_underlying(ColumnsCompanies::ID), _T("ID"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsCompanies::NAME), _T("Name"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsCompanies::DATE), _T("Creation Date"), LVCFMT_LEFT, 100);

	int listIndex = 0;
	for (auto company : companies) {
		PopulateCompany(company, listIndex);
		++listIndex;
	}
}

void stkSQLAppDlg::PopulateCompany(Company a_Company, int a_ListIndex) 
{
	LVITEM lvi;

	// Conver int to a string and use CString.
	CString id(std::to_string(a_Company.id).c_str());
	CString name(a_Company.name.c_str());
	CString creation_date(a_Company.creation_date.c_str());

	lvi.mask = LVIF_TEXT;
	lvi.iItem = a_ListIndex;
	lvi.iSubItem = 0;
	lvi.pszText = id.GetBuffer(0);
	m_ListCtrl.InsertItem(&lvi);

	lvi.iSubItem = 1;
	lvi.pszText = name.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	lvi.iSubItem = 2;
	lvi.pszText = creation_date.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);
}

void stkSQLAppDlg::PopulateEmployees() 
{
	if (!m_SQLConnector.IsConnected()) return;

	auto employees = m_SQLConnector.QueryEmployees();

	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::ID), _T("ID"), LVCFMT_LEFT, 100); 
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::FIRSTNAME), _T("First Name"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::LASTNAME), _T("Last Name"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::STARTINGDATE), _T("Starting Date"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::SALARY), _T("Salary"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::VACATIONDAYS), _T("Vacation Days"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::POSITION), _T("Position"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::FKCOMPANYID), _T("Company ID"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::FKOFFICEID), _T("Office ID"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsEmployees::FKBOSSID), _T("Boss ID"), LVCFMT_LEFT, 100);

	int listIndex = 0;
	for (auto employee : employees) {
		PopulateEmployee(employee, listIndex);
		++listIndex;
	}
}

void stkSQLAppDlg::PopulateEmployee(Employee a_Employee, int a_ListIndex) 
{
	LVITEM lvi;

	CString id(std::to_string(a_Employee.id).c_str());
	lvi.mask = LVIF_TEXT;
	lvi.iItem = a_ListIndex;
	lvi.iSubItem = 0;
	lvi.pszText = id.GetBuffer(0);
	m_ListCtrl.InsertItem(&lvi);

	CString first_name(a_Employee.first_name.c_str());
	lvi.iSubItem = 1;
	lvi.pszText = first_name.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString last_name(a_Employee.last_name.c_str());
	lvi.iSubItem = 2;
	lvi.pszText = last_name.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString starting_date(a_Employee.starting_date.c_str());
	lvi.iSubItem = 3;
	lvi.pszText = starting_date.GetBuffer(0);
	m_ListCtrl.SetItem(&lvi);

	CString salary(std::to_string(a_Employee.salary).c_str());
	lvi.iSubItem = 4;
	lvi.pszText = salary.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString vacation_days(std::to_string(a_Employee.vacation_days).c_str());
	lvi.iSubItem = 5;
	lvi.pszText = vacation_days.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString position(a_Employee.position == HEADCHIEF ? "Head Chief" : (a_Employee.position == SPECIALIST ? "Specialist" : "Junior"));
	lvi.iSubItem = 6;
	lvi.pszText = position.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString companyid(std::to_string(a_Employee.companyid).c_str());
	lvi.iSubItem = 7;
	lvi.pszText = companyid.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString officeid(std::to_string(a_Employee.officeid).c_str());
	lvi.iSubItem = 8;
	lvi.pszText = officeid.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString bossid(std::to_string(a_Employee.bossid).c_str());
	lvi.iSubItem = 9;
	lvi.pszText = bossid.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);
}

void stkSQLAppDlg::PopulateOffices() 
{
	if (!m_SQLConnector.IsConnected()) return;

	auto offices = m_SQLConnector.QueryOffices();

	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::ID), _T("ID"), LVCFMT_LEFT, 100); 
	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::COUNTRY), _T("Country"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::CITY), _T("City"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::STREET), _T("Street"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::STREETNUM), _T("Street Num"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::HEADQUARTERS), _T("Headquarters?"), LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(to_underlying(ColumnsOffices::FKCOMPANYID), _T("Company ID"), LVCFMT_LEFT, 100);

	int listIndex = 0;
	for (auto office : offices) {
		PopulateOffice(office, listIndex);
		++listIndex;
	}
}

void stkSQLAppDlg::PopulateOffice(Office a_Office, int a_ListIndex) 
{
	LVITEM lvi;

	CString id(std::to_string(a_Office.id).c_str());
	lvi.mask = LVIF_TEXT;
	lvi.iItem = a_ListIndex;
	lvi.iSubItem = 0;
	lvi.pszText = id.GetBuffer(0);
	m_ListCtrl.InsertItem(&lvi);

	CString country(a_Office.country.c_str());
	lvi.iSubItem = 1;
	lvi.pszText = country.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString city(a_Office.city.c_str());
	lvi.iSubItem = 2;
	lvi.pszText = city.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString street(a_Office.street.c_str());
	lvi.iSubItem = 3;
	lvi.pszText = street.GetBuffer(0);
	m_ListCtrl.SetItem(&lvi);

	CString streetnum(std::to_string(a_Office.streetnum).c_str());
	lvi.iSubItem = 4;
	lvi.pszText = streetnum.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString headquarters(a_Office.headquarters ? "YES" : "NO");
	lvi.iSubItem = 5;
	lvi.pszText = headquarters.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);

	CString companyid(std::to_string(a_Office.companyid).c_str());
	lvi.iSubItem = 6;
	lvi.pszText = companyid.GetBuffer(0); 
	m_ListCtrl.SetItem(&lvi);
}

void stkSQLAppDlg::OnBnClickedButtonDel()
{
	if (!m_SQLConnector.IsConnected()) return;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int item = m_ListCtrl.GetNextSelectedItem(pos);
	if (item < 0) return;

	CString idstr = m_ListCtrl.GetItemText(item, 0);
	int ID = std::stoi(std::string(CStringA(idstr)));

	switch (m_Showing) {
	case ListShowing::COMPANIES: {
		m_SQLConnector.DelCompany(ID);
	} break;
	case ListShowing::EMPLOYEES: {
		m_SQLConnector.DelEmployee(ID);
	} break;
	case ListShowing::OFFICES: {
		m_SQLConnector.DelOffice(ID);
	} break;
	default: break;
	}

	RePopulateList();
}

void stkSQLAppDlg::OnBnClickedButtonUpdate()
{
	if (!m_SQLConnector.IsConnected()) return;
	
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int item = m_ListCtrl.GetNextSelectedItem(pos);
	if (item < 0) return;

	CString idstr = m_ListCtrl.GetItemText(item, 0);
	int ID = std::stoi(std::string(CStringA(idstr)));

	switch (m_Showing) {
		case ListShowing::COMPANIES: {
			Company company = m_SQLConnector.QueryCompany(ID);
			
			// Prepare strings by reading text fields. 
			CString name;
			m_EditCompanyName.GetWindowTextW(name);
			if (!name.IsEmpty()) {
				company.name = std::string(CStringA(name));
			}

			CString creation_date;
			m_EditCompanyDate.GetWindowTextW(creation_date);
			if (!creation_date.IsEmpty()) {
				company.creation_date = std::string(CStringA(creation_date));
			}

			m_SQLConnector.UpdateCompany(company);
		} break;
		case ListShowing::EMPLOYEES: {
			Employee employee = m_SQLConnector.QueryEmployee(ID);

			// Prepare strings by reading text fields. 
			CString first_name;
			m_EditEmployeeFirstName.GetWindowTextW(first_name);
			if (!first_name.IsEmpty()) {
				employee.first_name = std::string(CStringA(first_name));
			}

			CString last_name;
			m_EditEmployeeLastName.GetWindowTextW(last_name);
			if (!last_name.IsEmpty()) {
				employee.last_name = std::string(CStringA(last_name));
			}

			CString starting_date;
			m_EditEmployeeStartingDate.GetWindowTextW(starting_date);
			if (!starting_date.IsEmpty()) {
				employee.starting_date = std::string(CStringA(starting_date));
			}

			CString salary;
			m_EditEmployeeSalary.GetWindowTextW(salary);
			if (!salary.IsEmpty()) {
				employee.salary = std::stoi(std::string(CStringA(salary)));
			}

			CString vacation_days;
			m_EditEmployeeVacationDays.GetWindowTextW(vacation_days);
			if (!vacation_days.IsEmpty()) {
				employee.vacation_days = std::stoi(std::string(CStringA(vacation_days)));
			}

			CString position;
			m_EditEmployeePosition.GetWindowTextW(position);
			if (!position.IsEmpty()) {
				employee.position = (SQLDB::EmployeePos)std::stoi(std::string(CStringA(position)));
			}

			CString companyid;
			m_EditEmployeeCompanyID.GetWindowTextW(companyid);
			if (!companyid.IsEmpty()) {
				employee.companyid = std::stoi(std::string(CStringA(companyid)));
			}

			CString officeid;
			m_EditEmployeeOfficeID.GetWindowTextW(officeid);
			if (!officeid.IsEmpty()) {
				employee.officeid = std::stoi(std::string(CStringA(officeid)));
			}

			CString bossid;
			m_EditEmployeeBossID.GetWindowTextW(bossid);
			if (!bossid.IsEmpty()) {
				employee.bossid = std::stoi(std::string(CStringA(bossid)));
			}

			m_SQLConnector.UpdateEmployee(employee);
		} break;
		case ListShowing::OFFICES: {
			Office office = m_SQLConnector.QueryOffice(ID);

			// Prepare strings by reading text fields. 
			CString country;
			m_EditOfficeCountry.GetWindowTextW(country);
			if (!country.IsEmpty()) {
				office.country = std::string(CStringA(country));
			}

			CString city;
			m_EditOfficeCity.GetWindowTextW(city);
			if (!city.IsEmpty()) {
				office.city = std::string(CStringA(city));
			}

			CString street;
			m_EditOfficeStreet.GetWindowTextW(street);
			if (!street.IsEmpty()) {
				office.street = std::string(CStringA(street));
			}

			CString streetnum;
			m_EditOfficeStreetNum.GetWindowTextW(streetnum);
			if (!streetnum.IsEmpty()) {
				office.streetnum = std::stoi(std::string(CStringA(streetnum)));
			}

			CString headquarters;
			m_EditOfficeHeadquarters.GetWindowTextW(headquarters);
			if (!headquarters.IsEmpty()) {
				office.headquarters = std::stoi(std::string(CStringA(headquarters)));
			}

			CString companyid;
			m_EditOfficeCompanyID.GetWindowTextW(companyid);
			if (!companyid.IsEmpty()) {
				office.companyid = std::stoi(std::string(CStringA(companyid)));
			}

			m_SQLConnector.UpdateOffice(office);
		} break;
		default: break;
	}
	
	RePopulateList();
}