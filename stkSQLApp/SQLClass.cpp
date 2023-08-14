#include "pch.h"
#include "SQLClass.h"

// Treat this namespace as own. 
using namespace SQLDB;

SQLClass::SQLClass() : m_SuccessfullyConnected(false), m_Connection(0)
{
}

void SQLClass::ShowErr(std::string a_ErrMsg, sql::SQLException e) const
{
    CString err(a_ErrMsg.c_str());
    err.Append(L"\n");
    CString exceptinon(e.what());
    err.Append(exceptinon);
    MessageBox(NULL, 
              (LPCWSTR)err,
              (LPCWSTR)L"SQL Exception",
              MB_OK);
}

bool SQLClass::IsConnected() const {
    return m_SuccessfullyConnected;
}

void SQLClass::Connect(
  const std::string a_Server
, const std::string a_Username
, const std::string a_Password
, const std::string a_DBname
) 
{
    sql::Driver* driver;
 
    try {
        driver = get_driver_instance();
        m_Connection = driver->connect(a_Server, a_Username, a_Password);
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't connect to server.", e);
        return;
    }

    CreateDB(a_DBname);

    try {
        m_Connection->setSchema(a_DBname);
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't connect to DB.", e);
        return;
    }
    
    CreateTables();
    m_SuccessfullyConnected = true;
}

void SQLClass::CreateDB(const std::string a_DBname)
{
    sql::Statement* stmt;
    std::string statement("CREATE DATABASE IF NOT EXISTS ");
    statement += a_DBname + ";";

    stmt = m_Connection->createStatement();

    try {
        stmt->execute(statement);
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't create DB.", e);
    }

    delete stmt;
}

void SQLClass::CreateTables() 
{
    sql::Statement* stmt;
    stmt = m_Connection->createStatement();

    try {
        stmt->execute("CREATE TABLE IF NOT EXISTS Companies (id INTEGER AUTO_INCREMENT PRIMARY KEY, name VARCHAR(50), creation_date VARCHAR(50));");
        
        stmt->execute("CREATE TABLE IF NOT EXISTS Offices (id INTEGER AUTO_INCREMENT PRIMARY KEY, country VARCHAR(50), city VARCHAR(50), street VARCHAR(50), street_number INTEGER, headquarters BOOLEAN, companyid INTEGER);");
        stmt->execute("ALTER TABLE Offices ADD FOREIGN KEY (companyid) REFERENCES Companies(id);");

        stmt->execute("CREATE TABLE IF NOT EXISTS Employees (id INTEGER AUTO_INCREMENT PRIMARY KEY, first_name VARCHAR(50), last_name VARCHAR(50), starting_date VARCHAR(50), salary INTEGER, vacation_days INTEGER, position INTEGER, companyid INTEGER, officeid INTEGER, bossid INTEGER);");
        stmt->execute("ALTER TABLE Employees ADD FOREIGN KEY (companyid) REFERENCES Companies(id);");
        stmt->execute("ALTER TABLE Employees ADD FOREIGN KEY (officeid) REFERENCES Offices(id);");
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't create tables.", e);
    }

    delete stmt;
}

void SQLClass::AddCompany(const Company& a_Comp) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("INSERT INTO Companies(name, creation_date) VALUES(?,?);");
    pstmt->setString(1, a_Comp.name);
    pstmt->setString(2, a_Comp.creation_date);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't add company.", e);
    }

    delete pstmt;
}

void SQLClass::AddEmployee(const Employee& a_Employee) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("INSERT INTO Employees(first_name, last_name, starting_date, salary, vacation_days, position, companyid, officeid, bossid) VALUES(?,?,?,?,?,?,?,?,?);");
    pstmt->setString(1, a_Employee.first_name);
    pstmt->setString(2, a_Employee.last_name);
    pstmt->setString(3, a_Employee.starting_date);
    pstmt->setInt(4, a_Employee.salary);
    pstmt->setInt(5, a_Employee.vacation_days);
    pstmt->setInt(6, to_underlying(a_Employee.position));
    pstmt->setInt(7, a_Employee.companyid);
    pstmt->setInt(8, a_Employee.officeid);
    pstmt->setInt(9, a_Employee.bossid);
    
    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't add employee.", e);
    }

    delete pstmt;
}

void SQLClass::AddOffice(const Office& a_Office) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("INSERT INTO Offices(country, city, street, street_number, headquarters, companyid) VALUES(?,?,?,?,?,?);");
    pstmt->setString(1, a_Office.country);
    pstmt->setString(2, a_Office.city);
    pstmt->setString(3, a_Office.street);
    pstmt->setInt(4, a_Office.streetnum);
    pstmt->setBoolean(5, a_Office.headquarters);
    pstmt->setInt(6, a_Office.companyid);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't add office.", e);
    }   

    delete pstmt;
}

std::vector<Company> SQLClass::QueryCompanies() 
{
    sql::Statement* stmt;
    std::vector<Company> Companies;

    stmt = m_Connection->createStatement();

    try {
        stmt->execute("SELECT * FROM Companies;");
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't query companies.", e);
        delete stmt;
        return Companies;
    }

    auto res = stmt->getResultSet();

    while (res->next()) {
        Company company;
        company.id = res->getInt(1);
        company.name = res->getString(2);
        company.creation_date = res->getString(3);
        Companies.push_back(company);
    }

    delete stmt;
    return Companies;
}

Company SQLClass::QueryCompany(int a_ID) 
{
    sql::PreparedStatement* pstmt;

    Company company;

    pstmt = m_Connection->prepareStatement("SELECT * FROM Companies WHERE id = ?;");
    pstmt->setInt(1, a_ID);
    
    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't query company.", e);
        delete pstmt;
        return company;
    }

    auto res = pstmt->getResultSet();

    while (res->next()) {
        company.id = res->getInt(1);
        company.name = res->getString(2);
        company.creation_date = res->getString(3);
    }

    delete pstmt;
    return company;
}

std::vector<Employee> SQLClass::QueryEmployees() 
{
    sql::Statement* stmt;
    std::vector<Employee> Employees;

    stmt = m_Connection->createStatement();

    try {
        stmt->execute("SELECT * FROM Employees;");
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't query employees.", e);
        delete stmt;
        return Employees;
    }

    auto res = stmt->getResultSet();

    while (res->next()) {
        Employee employee;
        employee.id = res->getInt(1);
        employee.first_name = res->getString(2);
        employee.last_name = res->getString(3);
        employee.starting_date = res->getString(4);
        employee.salary = res->getInt(5);
        employee.vacation_days = res->getInt(6);
        employee.position = (EmployeePos)res->getInt(7);
        employee.companyid = res->getInt(8);
        employee.officeid = res->getInt(8);
        employee.bossid = res->getInt(10);
        Employees.push_back(employee);
    }

    delete stmt;
    return Employees;
}

Employee SQLClass::QueryEmployee(int a_ID) 
{
    sql::PreparedStatement* pstmt;

    Employee employee;

    pstmt = m_Connection->prepareStatement("SELECT * FROM Employees WHERE id = ?;");
    pstmt->setInt(1, a_ID);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't query employee.", e);
        delete pstmt;
        return employee;
    }

    auto res = pstmt->getResultSet();

    while (res->next()) {
        employee.id = res->getInt(1);
        employee.first_name = res->getString(2);
        employee.last_name = res->getString(3);
        employee.starting_date = res->getString(4);
        employee.salary = res->getInt(5);
        employee.vacation_days = res->getInt(6);
        employee.position = (EmployeePos)res->getInt(7);
        employee.companyid = res->getInt(8);
        employee.officeid = res->getInt(9);
        employee.bossid = res->getInt(10);
    }

    delete pstmt;
    return employee;
}

std::vector<Office> SQLClass::QueryOffices() 
{
    sql::Statement* stmt;
    std::vector<Office> Offices;

    stmt = m_Connection->createStatement();

    try {
        stmt->execute("SELECT * FROM Offices;");
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't query offices.", e);
        delete stmt;
        return Offices;
    }

    auto res = stmt->getResultSet();

    while (res->next()) {
        Office office;
        office.id = res->getInt(1);
        office.country = res->getString(2);
        office.city = res->getString(3);
        office.street = res->getString(4);
        office.streetnum = res->getInt(5);
        office.headquarters = (bool)res->getBoolean(6);
        office.companyid = res->getInt(7);
        Offices.push_back(office);
    }

    delete stmt;
    return Offices;
}

Office SQLClass::QueryOffice(int a_ID) 
{
    sql::PreparedStatement* pstmt;

    Office office;

    pstmt = m_Connection->prepareStatement("SELECT * FROM Offices WHERE id = ?;");
    pstmt->setInt(1, a_ID);
    
    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't query office.", e);
        delete pstmt;
        return office;
    }

    auto res = pstmt->getResultSet();

    while (res->next()) {
        office.id = res->getInt(1);
        office.country = res->getString(2);
        office.city = res->getString(3);
        office.street = res->getString(4);
        office.streetnum = res->getInt(5);
        office.headquarters = (bool)res->getBoolean(6);
        office.companyid = res->getInt(7);
    }

    delete pstmt;
    return office;
}

void SQLClass::DelCompany(int a_ID) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("DELETE FROM Companies WHERE id = ?;");
    pstmt->setInt(1, a_ID);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't delete company.", e);
    }

    delete pstmt;
}

void SQLClass::DelEmployee(int a_ID) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("DELETE FROM Employees WHERE id = ?;");
    pstmt->setInt(1, a_ID);

    try {
        pstmt->execute();

    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't delete employee.", e);
    }

    delete pstmt;
}

void SQLClass::DelOffice(int a_ID) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("DELETE FROM Offices WHERE id = ?;");
    pstmt->setInt(1, a_ID);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't delete office.", e);
    }

    delete pstmt;
}

void SQLClass::UpdateCompany(const Company& a_Comp) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("UPDATE Companies SET name = ?, creation_date = ? WHERE id = ?;");
    pstmt->setString(1, a_Comp.name);
    pstmt->setString(2, a_Comp.creation_date);
    pstmt->setInt(3, a_Comp.id);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't update company.", e);
    }

    delete pstmt;
}

void SQLClass::UpdateEmployee(const Employee& a_Employee) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("UPDATE Employees SET first_name = ?, last_name = ?, starting_date = ?, salary = ?, vacation_days = ?, position = ?, companyid = ?, officeid = ?, bossid = ? WHERE id = ?;");
    pstmt->setString(1, a_Employee.first_name);
    pstmt->setString(2, a_Employee.last_name);
    pstmt->setString(3, a_Employee.starting_date);
    pstmt->setInt(4, a_Employee.salary);
    pstmt->setInt(5, a_Employee.vacation_days);
    pstmt->setInt(6, to_underlying(a_Employee.position));
    pstmt->setInt(7, a_Employee.companyid);
    pstmt->setInt(8, a_Employee.officeid);
    pstmt->setInt(9, a_Employee.bossid);
    pstmt->setInt(10, a_Employee.id);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't update employee.", e);
    }

    delete pstmt;
}

void SQLClass::UpdateOffice(const Office& a_Office) 
{
    sql::PreparedStatement* pstmt;

    pstmt = m_Connection->prepareStatement("UPDATE Offices SET country = ?, city = ?, street = ?, street_number = ?, headquarters = ?, companyid = ? WHERE id = ?;");
    pstmt->setString(1, a_Office.country);
    pstmt->setString(2, a_Office.city);
    pstmt->setString(3, a_Office.street);
    pstmt->setInt(4, a_Office.streetnum);
    pstmt->setBoolean(5, a_Office.headquarters);
    pstmt->setInt(6, a_Office.companyid);
    pstmt->setInt(7, a_Office.id);

    try {
        pstmt->execute();
    }
    catch (sql::SQLException e) {
        ShowErr("Couldn't update office.", e);
    }

    delete pstmt;
}

SQLClass::~SQLClass() 
{
    delete m_Connection;
}