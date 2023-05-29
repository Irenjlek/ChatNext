#include "User.h"
#include <iostream>
#include <fstream>
#include <string.h>

User::User() = default;

User::User(const std::string& name, const std::string& login, const std::string& password): 
	        _name (name), _login(login), _password(password) {}

void User::addMessage(std::shared_ptr<Message> msg)
{
    _messages.push(msg);
}

void User::showUnreadedMessages()
{
	if (_messages.size() > 0) {
		std::cout << _name << ", your have unreaded messages: " << std::endl << std::endl;
		while (_messages.size() > 0) {
			std::shared_ptr<Message> msg = _messages.front();
			std::cout << msg->getSender() << std::setw(35) << msg->getTime() << std::setw(30)
				<< "--->" << std::setw(30)
				<< msg->getRecipient() << std::endl << std::endl << std::setw(20) << "\" "
				<< msg->getText() << " \""
				<< std::endl << std::endl;		
			_messages.pop();
		}
	}
	else {
		std::cout << "You have not unreaded messages." << std::endl;
	}
}

bool User::writeToFile(std::string fileName)
{
	std::ofstream file_writer = std::ofstream(fileName, std::ios::app);
	if (!file_writer.is_open()) {
		std::cout << "Could not open file " << fileName << " !" << '\n';
		return false;
	}

	file_writer << _name;
	file_writer << '\n';
	file_writer << _login;
	file_writer << '\n';
	file_writer << _password;
	file_writer << '\n';
	file_writer << _messages.size();
	file_writer << '\n';
	
	std::queue <std::shared_ptr <Message>> messages = _messages;
	while (messages.size() > 0) {
		std::shared_ptr<Message> msg = messages.front();
		file_writer << msg->getSender();
		file_writer << '\n';
		file_writer << msg->getRecipient();
		file_writer << '\n';
		file_writer << msg->getTime();
		file_writer << '\n';
		file_writer << msg->getText();
		file_writer << '\n';
		messages.pop();
	}	
	return true;
}

std::queue <std::shared_ptr <User>> User::readFromFile(std::string fileName)
{
	std::queue <std::shared_ptr <User>> users;
	std::string str;
	std::vector<std::string> strings;
	std::ifstream file_reader(fileName);
	if (!file_reader.is_open()) {
		std::cout << "Could not open input file!" << '\n';
		return users;
	}

	while (std::getline(file_reader, str, '\n')) 
		strings.push_back(str);

	int start = 0;
	std::shared_ptr <User> newUser;
	while (start < strings.size()) {
		newUser = std::make_shared <User>(strings.at(start++), strings.at(start++), strings.at(start++));
		int countMsg = std::stoi(strings.at(start++));
		if (countMsg > 0) {
			for (int i = 0; i < countMsg; i++) {
				std::shared_ptr <Message> shp_mess = std::make_shared<Message>(strings.at(start++), strings.at(start++), strings.at(start++), strings.at(start++));
                                newUser->addMessage(shp_mess);
			}			
		}
		users.push(newUser);
	}

	return users;
}

std::ostream& operator<< (std::ostream& os, const User& us)
{
	os << us._name;
	return os;
}
