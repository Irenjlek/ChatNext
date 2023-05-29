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
#include <arpa/inet.h>

class Chat
{
private:	
	std::map<int, std::shared_ptr <User>> _users;
	int _users_count;
	std::shared_ptr <User> _activeUser;
    int client_socket_file_descriptor, server_socket_file_descriptor;
    bool _connected;

    bool initClient(sockaddr_in serveraddress);
    bool initServer(sockaddr_in serveraddress);
    void parseMessage(std::string message);

public:
	Chat();
	~Chat();

	void createNewUser(const std::string& name, const std::string& login, const std::string& password);
	void addUser(const std::shared_ptr<User>& user);
	void setActiveUser(const std::shared_ptr<User>& user);
	void login(std::string login, std::string password);
    void writeToOne(std::string text, std::shared_ptr<User>);
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
    bool saveToFile(std::string fileName);
    bool initClientServerMode();
};
