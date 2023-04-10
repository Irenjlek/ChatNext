#include "Chat.h"
#include "BadLogin.h"
#include "BadPassword.h"
#include <iostream>
#include <algorithm>

Chat::Chat() : _activeUser(nullptr),
				_users_count(0)
{

}

Chat::~Chat()
{

}

void Chat::createNewUser(const std::string& name, const std::string& login, const std::string& password)
{
	if (isLoginExist(login)) {
		std::cout << "User with login " << login << " is already exists." << std::endl;
		return;
	}		
	std::shared_ptr <User> newUser = std::make_shared <User>(name, login, password);
	addUser(newUser);
	setActiveUser(newUser);
}

void Chat::addUser(const std::shared_ptr<User>& user)
{
	_users.insert(std::make_pair(_users_count++, user));
}

void Chat::setActiveUser(const std::shared_ptr<User>& user)
{
	if (user != nullptr)
		std::cout << "Hello, " << user->getName() << "! Nice to see you!" << std::endl;
	_activeUser = user;
}

void Chat::login(std::string login, std::string password)
{
	bool found = isLoginExist(login);
	if (!found)
		throw BadLogin();
	for (std::map<int, std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getLogin() == login) {
			if (it->second->getPassword() == password) {
				setActiveUser(it->second);
			}
			else
				throw BadPassword();
		}
	}
}

void Chat::write(std::string text, std::shared_ptr<User> recipient)
{
	std::shared_ptr <Message> shp_mess = std::make_shared<Message>(text, getActiveUser()->getLogin(),
		                                           recipient->getLogin());
	recipient->addMessage(shp_mess);
}

void Chat::writeToAll(const std::string text)
{
	for (std::map<int, std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getLogin() != getActiveUser()->getLogin())
		{
			std::shared_ptr <Message> shp_mess = std::make_shared<Message>(text, getActiveUser()->getLogin(),
				                                         it->second->getLogin());
			it->second->addMessage(shp_mess);
		}
	}
}

bool Chat::isLoginExist(const std::string& login)
{
	for (std::map<int, std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getLogin() == login) 			
			return true;
	}
	return false;
}

std::shared_ptr<User> Chat::getActiveUser()
{	
	return _activeUser;
}

void Chat::showMenuAddMessege()
{
	std::cout << "Choose recipient mode : 1 - to One , 2 - to All, 3 - Exit \n";
}

std::shared_ptr<User> Chat::getUser(const std::string login)
{
	for (std::map<int, std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); ++it)
		if (it->second->getLogin() == login)				
			return it->second;    
		
		return std::make_shared <User>();
}

std::ostream& operator<< (std::ostream& os, const Chat& ch)
{
	int count(0);
	for (auto& user : ch._users)
	{		
		os << std::setw(3) << " < " << *user.second << " > ";
		count++;
		if (!(count % 6))
			os << std::endl;
	}
	return os;
}

void Chat::showAllLogin()
{
	for (auto& user : _users)
	{
		int count(0);
		std::cout << std::setw(3) << " < " << user.second->getLogin() << " > ";
		count++;
		if (count % 6 == 0)
			std::cout << std::endl;
	}
}


bool Chat::isUnicName(const std::string name)
{
	int count(0);
	
	for (auto& user : _users)
	{
		
		if (user.second->getName() == name)
			count++;
	}
	
	if (count == 1)
		return true;
	
	else 
		return false;

}

std::string Chat::getNameByLogin(const std::string login)
{
	for (auto& user : _users)
	{

		if (user.second->getLogin() == login)
			return user.second->getName();

	}
			std::cout << "Name by " << login << " is not found!\n";
			return "\0";		
	
}

std::string Chat::getLoginByName(const std::string name)
{
	for (auto& user : _users)
	{
		if (user.second->getName() == name)
			return user.second->getLogin();
	}
	std::cout << "Login by " << name << "is not found!\n";
	return "\0";
}


bool Chat::isontheList(const std::string name)
{
	for (auto& tempname : _users)
	{
		if (tempname.second->getName() == name)
			return true;
	}
	std::cout << "bad recipient, try again!\n";
	return false;
}

bool Chat::saveToFile(std::string fileName)
{
	std::ofstream file_writer = std::ofstream(fileName);
	if (!file_writer.is_open()) {
		std::cout << "Could not open file " << fileName << " !" << '\n';
		return false;
	}

	for (std::map<int, std::shared_ptr<User>>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if (!it->second->writeToFile(fileName)) 
			return false;
	}		
	return true;
}

