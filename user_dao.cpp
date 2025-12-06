#include "user_dao.h"

void UserDAO::create(const User& user) {
    std::string sql = "INSERT INTO users (name, email, age) VALUES ('" +
        user.name + "', '" + user.email + "', " + std::to_string(user.age) + ");";
    MYSQL_RES* result = client->do_query(sql);
    if (result) mysql_free_result(result);
}

void UserDAO::update(const User& user) {
    std::string sql = "UPDATE users SET name = '" + user.name +
        "', email = '" + user.email +
        "', age = " + std::to_string(user.age) +
        " WHERE id = " + std::to_string(user.id) + ";";
    MYSQL_RES* result = client->do_query(sql);
    if (result) mysql_free_result(result);
}

void UserDAO::remove(int id) {
    std::string sql = "DELETE FROM users WHERE id = " + std::to_string(id) + ";";
    MYSQL_RES* result = client->do_query(sql);
    if (result) mysql_free_result(result);
}

User UserDAO::getById(int id) {
    User u;
    std::string sql = "SELECT * FROM users WHERE id = " + std::to_string(id) + ";";
    MYSQL_RES* result = client->do_query(sql);

    if (result != nullptr) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            u.id = std::atoi(row[0]);
            u.name = row[1];
            u.email = row[2];
            u.age = std::atoi(row[3]);
        }
        mysql_free_result(result);
    }
    return u;
}

std::vector<User> UserDAO::getAll() {
    std::vector<User> users;
    std::string sql = "SELECT * FROM users;";
    MYSQL_RES* result = client->do_query(sql);

    if (result != nullptr) {
        MYSQL_ROW row;

        while (row = mysql_fetch_row(result)) {
            User u;
            u.id = std::atoi(row[0]);
            u.name = row[1];
            u.email = row[2];
            u.age = std::atoi(row[3]);

            users.push_back(u);
        }
        mysql_free_result(result);
    }

    return users;
}