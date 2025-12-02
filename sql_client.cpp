#include "sql_client.h"

SQLClient::SQLClient(std::string _host, int _port, std::string _user, std::string _password, std::string _database)
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

void SQLClient::print_result(MYSQL_RES* result) {
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

void SQLClient::do_query(const std::string& command) {
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

SQLClient::~SQLClient() {
	if (connect != NULL) {
		mysql_close(connect);
		std::cout << "Connection was successfylly closed, have a good day! \n";
	}
}