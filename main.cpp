#define NOMINMAX

#include "sql_client.h"
#include "user_dao.h"
#include "user_record.h"

#include <limits>
#include <algorithm>

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