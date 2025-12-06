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

void ignoreLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ActiveRecord(SQLClient* client) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- ACTIVE RECORD MODE ---\n";
        std::cout << "1. Add User (Save)\n";
        std::cout << "2. Find User by ID\n";
        std::cout << "3. Delete User by ID\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << ">> ";
        std::cin >> choice;
        ignoreLine();

        if (choice == 1) {
            UserRecord user(client);

            std::string name, email;
            int age;

            std::cout << "Enter Name: ";
            std::getline(std::cin, name);
            std::cout << "Enter Email: ";
            std::getline(std::cin, email);
            std::cout << "Enter Age: ";
            std::cin >> age;
            ignoreLine();

            user.setName(name);
            user.setEmail(email);
            user.setAge(age);

            user.save();
        }
        else if (choice == 2) {
            int id;
            std::cout << "Enter ID: ";
            std::cin >> id;
            ignoreLine();

            UserRecord u = UserRecord::findById(client, id);

            if (u.getId() != 0) {
                std::cout << "Found: " << u.getId() << " | " << u.getName()
                    << " | " << u.getEmail() << " | " << u.getAge() << "\n";
            }
            else {
                std::cout << "User not found!\n";
            }
        }
        else if (choice == 3) {
            int id;
            std::cout << "Enter ID to delete: ";
            std::cin >> id;
            ignoreLine();

            UserRecord user(client);
            user.setId(id);
            user.remove();
        }
    }
}

void DAO(SQLClient* client) {
    UserDAO dao(client);
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n--- DAO MODE ---\n";
        std::cout << "1. Create User\n";
        std::cout << "2. Get All Users\n";
        std::cout << "3. Get User by ID\n";
        std::cout << "4. Update User (by ID)\n";
        std::cout << "5. Delete User\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << ">> ";
        std::cin >> choice;
        ignoreLine();

        if (choice == 1) {
            User u;
            std::cout << "Enter Name: ";
            std::getline(std::cin, u.name);
            std::cout << "Enter Email: ";
            std::getline(std::cin, u.email);
            std::cout << "Enter Age: ";
            std::cin >> u.age;
            ignoreLine();

            dao.create(u);
        }
        else if (choice == 2) {
            std::vector<User> users = dao.getAll();

            std::cout << "\nID | Name | Email | Age\n";
            std::cout << "------------------------\n";
            for (const auto& u : users) {
                std::cout << u.id << " | " << u.name << " | " << u.email << " | " << u.age << "\n";
            }
        }
        else if (choice == 3) {
            int id;
            std::cout << "Enter ID: ";
            std::cin >> id;
            ignoreLine();

            User u = dao.getById(id);
            if (u.id != 0) {
                std::cout << "Found: " << u.id << " | " << u.name << " | " << u.email << "\n";
            }
            else {
                std::cout << "User not found!\n";
            }
        }
        else if (choice == 4) {
            User u;
            std::cout << "Enter ID to update: ";
            std::cin >> u.id;
            ignoreLine();
            std::cout << "Enter New Name: ";
            std::getline(std::cin, u.name);
            std::cout << "Enter New Email: ";
            std::getline(std::cin, u.email);
            std::cout << "Enter New Age: ";
            std::cin >> u.age;
            ignoreLine();

            dao.update(u);
        }
        else if (choice == 5) {
            int id;
            std::cout << "Enter ID: ";
            std::cin >> id;
            ignoreLine();
            dao.remove(id);
        }
    }
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

		SQLClient client("localhost", port, "root", "root", "lab9_db");

        int mainChoice = -1;
        while (mainChoice != 0) {
            std::cout << "\n=== MAIN MENU ===\n";
            std::cout << "1. Active Record Pattern\n";
            std::cout << "2. DAO Pattern\n";
            std::cout << "3. Raw SQL Console\n";
            std::cout << "0. Exit\n";
            std::cout << "Select mode >> ";

            std::cin >> mainChoice;
            ignoreLine();

            if (mainChoice == 1) {
                ActiveRecord(&client);
            }
            else if (mainChoice == 2) {
                DAO(&client);
            }
            else if (mainChoice == 3) {
                std::string command;
                std::cout << "Enter SQL (or 'exit'): ";

                while (true) {
                    std::cout << "\nSQL >> ";
                    std::getline(std::cin, command);

                    if (command == "exit") break;

                    MYSQL_RES* result = client.do_query(command);
                    
                    if (result != nullptr) {
                        client.print_result(result);
                        mysql_free_result(result);
                    }
                }
            }
        }
    }
    catch (const std::runtime_error& error) {
        std::cout << "Error: " << error.what();
        return 1;
    }
    return 0;
}