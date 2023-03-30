#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "User.h"
#include "Message.h"
#include <iomanip>
#include <typeinfo>
#include <map>

class Chat
{
private:
	std::map<int, std::shared_ptr <User>> _users;
	int _users_count;
	std::shared_ptr <User> _activeUser;

public:
	Chat();
	~Chat();

	bool createNewUser(const std::string& name, const std::string& login, const std::string& password);
	void setActiveUser(const std::shared_ptr<User>& user);
	void login(std::string login, std::string password);
	void write(std::string text, std::shared_ptr<User>);
	void writeToAll(const std::string text);
	bool isLoginExist(const std::string& login);
	std::shared_ptr <User> getActiveUser();
	std::shared_ptr <User> getUser(const std::string login);
	void showMenuAddMessege();
	friend std::ostream& operator<< (std::ostream& os, const Chat&);
	void showAllLogin();
	bool isUnicName(const std::string name);
	std::string getNameByLogin(const std::string login);
	std::string getLoginByName(const std::string name);
	bool isontheList(const std::string name);
};