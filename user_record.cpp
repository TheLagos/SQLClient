#include "user_record.h"

UserRecord::UserRecord(SQLClient * cl) : client(cl), id(0), age(0) {}

void UserRecord::setName(std::string n) { this->name = n; }
void UserRecord::setEmail(std::string e) { this->email = e; }
void UserRecord::setAge(int a) { this->age = a; }
void UserRecord::setId(int i) { this->id = i; }

void UserRecord::save() {
    std::string sql;

    if (this->id == 0) {
        sql = "INSERT INTO users (name, email, age) VALUES ('" +
            name + "', '" + email + "', " + std::to_string(age) + ");";
    }
    else {
        sql = "UPDATE users SET name = '" + name +
            "', email = '" + email +
            "', age = " + std::to_string(age) +
            " WHERE id = " + std::to_string(id) + ";";
    }

    MYSQL_RES* res = client->do_query(sql);
    if (res) mysql_free_result(res);
}

void UserRecord::remove() {
    if (this->id != 0) {
        std::string sql = "DELETE FROM users WHERE id = " + std::to_string(id) + ";";
        MYSQL_RES* res = client->do_query(sql);
        if (res) mysql_free_result(res);
        this->id = 0;
    }
}

UserRecord UserRecord::findById(SQLClient* client, int id) {
    UserRecord user(client);
    std::string sql = "SELECT * FROM users WHERE id = " + std::to_string(id) + ";";
    MYSQL_RES* result = client->do_query(sql);

    if (result != nullptr) {
        MYSQL_ROW row = mysql_fetch_row(result);

        if (row) {
            user.id = std::atoi(row[0]);
            user.name = row[1] ? row[1] : "";
            user.email = row[2] ? row[2] : "";
            user.age = std::atoi(row[3]);
        }

        mysql_free_result(result);
    }

    return user;
}