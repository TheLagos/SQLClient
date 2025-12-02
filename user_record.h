#pragma once

#include "sql_client.h"
#include <string>

class UserRecord {
private:
    SQLClient* client;

    int id;
    std::string name;
    std::string email;
    int age;

public:
    UserRecord(SQLClient* client);

    void setName(std::string n);
    void setEmail(std::string e);
    void setAge(int a);

    void save();
    void remove();

    static UserRecord findById(SQLClient* client, int id);
};