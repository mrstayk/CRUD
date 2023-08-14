#pragma once

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <vector>

// Used to extract the underlying type of a scoped enum. 
// Why does it need to be noexcept?
template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T t) noexcept {
    return static_cast<std::underlying_type_t<T>>(t);
}

// Namespace is given to specify the origin of these structs. 
namespace SQLDB {
	enum class EmployeePos {
		HEADCHIEF,
		SPECIALIST,
		JUNIOR
	};

	struct Company {
		int id;
		std::string name;
		std::string creation_date;
	};

	struct Employee {
		int id;
		std::string first_name;
		std::string last_name;
		std::string starting_date;
		int salary;
		int vacation_days;
		EmployeePos position;
		int companyid;
		int officeid;
		int bossid;
	};

	struct Office {
		int id;
		std::string country;
		std::string city;
		std::string street;
		int streetnum;
		bool headquarters;
		int companyid;
	};

	class SQLClass
	{
	public:
		SQLClass();
		SQLClass(SQLClass&) = default;
		~SQLClass();

		bool IsConnected() const;
		void Connect(
		  const std::string //a_Server
		, const std::string //a_Username
		, const std::string //a_Password
		, const std::string //a_DBname
		);
		void AddCompany(const Company&);
		void AddEmployee(const Employee&);
		void AddOffice(const Office&);
		std::vector<Company> QueryCompanies();
		Company QueryCompany(int);
		std::vector<Employee> QueryEmployees();
		Employee QueryEmployee(int);
		std::vector<Office> QueryOffices();
		Office QueryOffice(int);
		void DelCompany(int);
		void DelEmployee(int);
		void DelOffice(int);
		void UpdateCompany(const Company&);
		void UpdateEmployee(const Employee&);
		void UpdateOffice(const Office&);

	private:
		void ShowErr(std::string a_ErrMsg, sql::SQLException) const; 
		void CreateDB(const std::string a_DBname);
		void CreateTables();
		sql::Connection* m_Connection;
		bool m_SuccessfullyConnected;
	};
}

