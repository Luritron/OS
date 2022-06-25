#include "FAT16.h"

void menu();

void main()
{
	initFAT();
	while (1)
	{
		menu();
	}

	return;
}

void menu()
{
	std::cout << "--------------------" << std::endl;
	std::cout << "\n\tMENU" << std::endl;
	std::cout << " 1: Add file" << std::endl;
	std::cout << " 2: Resize the file" << std::endl;
	std::cout << " 3: Delete the file" << std::endl;
	std::cout << " 4: Find the file" << std::endl;
	std::cout << " 5: Output all files" << std::endl;
	std::cout << " 6: Output FAT-table" << std::endl;
	std::cout << " 7: Exit" << std::endl;

	char command;
	std::cout << "\nEnter the key: ";
	std::cin >> command;

	std::string file_name;
	uint32_t file_size;
	switch (command)
	{
	case '1':
		system("CLS");
		std::cout << "\nEnter file name and size in a row which you want to create: ";
		std::cin >> file_name >> file_size;
		addFile(file_name, file_size);
		system("pause");
		system("CLS");
		break;
	case '2':
		system("CLS");
		std::cout << "\nEnter file name and new size in a row which you want to resize: ";
		std::cin >> file_name >> file_size;
		resizeFile(file_name, file_size);
		system("pause");
		system("CLS");
		break;
	case '3':
		system("CLS");
		std::cout << "\nEnter file name which you want to delete: ";
		std::cin >> file_name;
		deleteFile(file_name);
		system("pause");
		system("CLS");
		break;
	case '4':
		system("CLS");
		std::cout << "\nEnter the name of the file which you are looking for: ";
		std::cin >> file_name;
		findFile(file_name);
		system("pause");
		system("CLS");
		break;
	case '5':
		system("CLS");
		std::cout << "\nYour files are:\n";
		coutFAT();
		system("pause");
		system("CLS");
		break;
	case '6':
		system("CLS");
		std::cout << "\nMemory view:";
		tableFAT();
		system("pause");
		system("CLS");
		break;
	case '7':
		exit(0);
	default:
		std::cout << "\nThe specified command does not exist" << std::endl;
		break;
	}
}
