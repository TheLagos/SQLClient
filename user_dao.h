#pragma once

#include "sql_client.h"
#include "user.h"

#include <vector>

class UserDAO {
private:
	SQLClient* client;
public:
	UserDAO(SQLClient* cl) : client(cl) {}

	void create(const User& user);
	User getById(int id);
	std::vector<User> getAll(); 
	void update(const User& user); 
	void remove(int id);
};