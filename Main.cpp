#include "Chat.h"
#include "Message.h"
#include "User.h"
#include "BadLogin.h"
#include "BadPassword.h"
#include <iostream>
#include <ctime>

bool selectAction(std::shared_ptr<Chat> chat) {
	char choose;
	while (true)
	{
		while (chat->getActiveUser() == nullptr) {
			std::cout << "Would you like login(enter 'L'), register(enter 'R') or quit(enter 'Q')?" << std::endl;
			std::string selection;
			std::cin >> selection;
			if (selection.size() > 1) {
				std::cout << "I think you mean '" << selection.at(0) << "'" << std::endl;
			}
			switch (selection.at(0))
			{
			case 'R':
			{
				std::string name, login, password;
				std::cout << "Enter login" << std::endl;
				std::cin >> login;
				if (chat->isLoginExist(login)) {
					std::cout << "User with login " << login << " is already exists." << std::endl;
					break;
				}
				std::cout << "Enter password" << std::endl;
				std::cin >> password;
				std::cout << "Enter name" << std::endl;
				std::cin >> name;
				chat->createNewUser(name, login, password);
				break;
			}
			case 'L':
			{
				std::string login, password;
				std::cout << "Enter login" << std::endl;
				std::cin >> login;
				std::cout << "Enter password" << std::endl;
				std::cin >> password;

				try {
					chat->login(login, password);
					std::cout << std::endl;
					chat->getActiveUser()->showUnreadedMessages();
					std::cout << std::endl;
				}
				catch (BadLogin& e) {
					std::cout << e.what() << std::endl;
					break;
				}
				catch (BadPassword& e) {
					std::cout << e.what() << std::endl;
					int tries = 3;
					while (--tries > 0) {
						try {
							std::cout << "Enter password (" << tries << " tries left)" << std::endl;
							std::cin >> password;
							chat->login(login, password);
							std::cout << std::endl;
							chat->getActiveUser()->showUnreadedMessages();
							std::cout << std::endl;
						}
						catch (BadPassword& err) {
							std::cout << err.what() << std::endl;
						}
					}
					break;
				}
				break;
			}
			case 'Q':
				std::cout << "Good luck, bye!" << std::endl;
				return false;
			default:
				std::cout << "Enter L, R or Q" << std::endl;
				break;
			}
		}

		chat->showMenuAddMessege(); // 1 - One, 2 - All, 3 - Exit	    
		std::cin >> choose;
		while (std::cin.get() != '\n')
			continue;
		std::string message;
		switch (choose)
		{
		case '1':
		{
			std::cout << "You are : " << chat->getActiveUser()->getName() << std::endl;
			std::cout << "Cho�se the recipient:" << std::endl;
			std::cout << *chat << std::endl;
			std::string name_recipient;
			std::string log_recipient;
			std::getline(std::cin, name_recipient);

			if (!(chat->isontheList(name_recipient)))
				continue;

			if (!chat->isUnicName(name_recipient)) // e��� ��� �� ���������� �� ��������, ��� - ��������� �� else
			{
				std::cout << "The name is not unique, chose name by login list : " << std::endl;
				chat->showAllLogin();
				std::cout << std::endl;
				std::string login_from_list;
				std::getline(std::cin, login_from_list);
				if (!(chat->isontheList(chat->getNameByLogin(login_from_list))))
					continue;
				log_recipient = std::move(login_from_list);
			}
			else
				log_recipient = chat->getLoginByName(name_recipient);

			std::cout << std::endl;
			std::cout << "The logrecipient is :" << log_recipient << std::endl;
			std::cout << std::endl;
			std::cout << "write a message" << std::endl;
			std::getline(std::cin, message);
			std::cout << std::endl;

			chat->write(message, chat->getUser(log_recipient));
			time_t result = time(NULL);
			char str[26];
			ctime_s(str, sizeof str, &result);
			std::cout << chat->getActiveUser()->getName() << std::setw(35) << str << std::setw(30)
				<< "--->" << std::setw(30)
				<< chat->getNameByLogin(log_recipient) << std::endl << std::endl << std::setw(15) << "\" " << message << " \""
				<< std::endl << std::endl;
		}
		continue;

		case '2':
		{
			std::cout << "You are : " << chat->getActiveUser()->getName() << std::endl;
			std::cout << "write a message : \n";
			std::getline(std::cin, message);
			chat->writeToAll(message);
			std::cout << std::endl;
			time_t result = time(NULL);
			char str[26];
			ctime_s(str, sizeof str, &result);
			std::cout << chat->getActiveUser()->getName() << std::setw(35) << str << std::setw(30)
				<< "--->" << std::setw(30)
				<< "All" << std::endl << std::endl << std::setw(15) << "\" " << message << " \""
				<< std::endl << std::endl;
		}
		continue;

		case '3':
		{
			std::cout << "Exit!" << std::endl;
			chat->setActiveUser(nullptr);
			break;
		}

		default: std::cout << "bad choose mode, try again! \n";
			continue;
		}
	}
}

int main() {
	std::shared_ptr<Chat> chat = std::make_shared<Chat>();

	std::cout << "Hello! Let's start!" << std::endl;

	if (!selectAction(chat))
		return 0;	
		
	std::cout << "Done!" << std::endl;

	return 0;
}