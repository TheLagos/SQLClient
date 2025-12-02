#define NOMINMAX

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <limits>
#include <algorithm>
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
	SQLClient(std::string _host, int _port, std::string _user, std::string _password, std::string _database)
	: host(_host), port(_port), user(_user), password(_password), database(_database), connect(nullptr) {
		connect = mysql_init(NULL);
		if (connect == NULL) {
			throw std::runtime_error("Could not initialize database! \n\n");
		}

		if (mysql_real_connect(connect, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0) == NULL) {
			std::string error = mysql_error(connect);
			mysql_close(connect);
			throw std::runtime_error("Could not connect to database: " + error + "! \n\n");
		}
		std::cout << "Successfully connected to '" << database << "'! \n\n";
	}

	void print_result(MYSQL_RES* result) {
		unsigned int num_fields = mysql_num_fields(result);
		MYSQL_FIELD* fields = mysql_fetch_fields(result);
		const int width = 20;


		for (unsigned int i = 0; i < num_fields; ++i) {
			std::cout << std::left << std::setw(width) << fields[i].name << "|";
		}
		std::cout << "\n";

		for (unsigned int i = 0; i < num_fields; ++i) {
			std::cout << std::string(width, '-') << "+";
		}
		std::cout << "\n";

		MYSQL_ROW row;
		while (row = mysql_fetch_row(result)) {
			for (unsigned int i = 0; i < num_fields; ++i) {
				const char* value = row[i] ? row[i] : "NULL";
				std::cout << std::left << std::setw(width) << value << "|";
			}
			std::cout << "\n";
		}
		std::cout << "\n\n";
	}

	void do_query(const std::string& command) {
		if (mysql_query(connect, command.c_str()) != 0) {
			std::cout << "Query error: " << mysql_error(connect) << "!\n\n";
			return;
		}

		MYSQL_RES* result = mysql_store_result(connect);

		if (result != NULL) {
			print_result(result);
		}
		else {
			if (mysql_field_count(connect) == 0) {
				std::cout << "Query successfull, " << mysql_affected_rows(connect) << " rows affected! \n\n";
			}
			else {
				std::cout << "Retrieval error: " << mysql_error(connect) << "!\n\n";
			}
		}
		
		mysql_free_result(result);
	}

	~SQLClient() {
		if (connect != NULL) {
			mysql_close(connect);
			std::cout << "Connection was successfylly closed, have a good day! \n";
		}
	}
};

std::string to_lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return std::tolower(c);
	});
	return str;
}

int main() {
	try {
		std::string db_type;
		int port = 0;

		while (port == 0) {
			std::cout << "Choose database: local or docker >> ";
			std::cin >> db_type;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (to_lower(db_type) == "local") {
				port = 3306;
			}
			else if (to_lower(db_type) == "docker") {
				port = 3307;
			}
			else {
				std::cout << "Incorrect input, choose correct type!" << "\n\n";
			}
		}

		SQLClient c("localhost", port, "root", "root", "lab9_db");
		std::string command = "";

		std::cout << "Enter SQL command (or 'EXIT' to quit): \n\n>> ";

		while (to_lower(command) != "exit") {
			std::getline(std::cin, command);

			if (to_lower(command) == "exit") {
				break;
			}

			if (command.empty()) {
				continue;
			}
			c.do_query(command);
			std::cout << ">> ";
		}
	}
	catch (const std::runtime_error& error) {
		std::cout << "Initialization error: " << error.what();
		return 1;
	}
}