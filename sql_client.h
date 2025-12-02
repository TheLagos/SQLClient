#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <mysql.h>

class SQLClient {
private:
	const std::string host;
	const int port;
	const std::string user;
	const std::string password;
	const std::string database;
	MYSQL* connect;
public:
	SQLClient(std::string _host, int _port, std::string _user, std::string _password, std::string _database);
	~SQLClient();

	void print_result(MYSQL_RES* result);
	void do_query(const std::string& command);
};