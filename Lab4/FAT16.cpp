#include "FAT16.h"

FAT16Settings fat16_set;
std::vector<int32_t> memory_vec;
std::vector<FileStructure> fat16;

void initFAT()
{
	memory_vec.reserve(fat16_set.cluster_num + 1);
	memory_vec.push_back(0x0F0F);
	memory_vec.push_back(0xFFFF);
	for (uint32_t i = 2; i <= fat16_set.cluster_num; ++i)
	{
		memory_vec.push_back(0x0000);
		if (i % 16 == 0)
		{
			memory_vec.push_back(0xF7);
		}
	}
}

void addFile(std::string file_name, uint32_t file_size)
{
	for (const auto& a : fat16)
	{
		if (a.file_name == file_name)
		{
			std::cerr << "File name must be unique" << std::endl;
			return;
		}
	}
	uint32_t available_clusters = fat16_set.cluster_num - 2;
	for (const auto& a : fat16)
	{
		available_clusters -= a.size;
	}
	if (available_clusters < file_size)
	{
		std::cerr << "Not enough memory to load this file" << std::endl;
		return;
	}
	FileStructure file;
	file.file_name = file_name;
	file.size = file_size;
	for (uint32_t i = 2; i < fat16_set.cluster_num; ++i)
	{
		if (memory_vec[i] == 0x0000)
		{
			file.first_cluster_num = i;
			break;
		} 
	}
	uint32_t current_location = file.first_cluster_num;
	while (file_size != 0)
	{
		if (memory_vec[current_location] == 0x0000)
		{
			memory_vec[current_location] = current_location;
			++current_location;
			--file_size;
		}
		else if (memory_vec[current_location] == 0xF7)
		{
			++current_location;
			continue;
		}
	}
	memory_vec[current_location] = 0xffff;
	fat16.push_back(file);
}

void resizeFile(std::string file_name, uint32_t new_size)
{
	FileStructure* file = &fat16[0];
	bool flag = false;
	for (auto& fat : fat16)
	{
		if (fat.file_name == file_name)
		{
			file = &fat;
			flag = true;
			break;
		}
	}
	if (flag == false)
	{
		std::cerr << file_name << " file is not exist in this system" << std::endl;
		return;
	}

	uint32_t free_clusters = fat16_set.cluster_num - 1;
	int32_t new_data_size = new_size - file->size;

	for (const auto& fat : fat16)
	{
		free_clusters -= fat.size;
	}
	if (free_clusters < std::fabs(new_data_size) && new_data_size > 0)
	{
		std::cerr << "System does not have enough space" << std::endl;
		return;
	}
	if (new_data_size < 0)
	{
		int32_t new_data = 0;
		uint32_t current_cluster = 0;

		for (current_cluster = file->first_cluster_num; ; current_cluster++)
		{
			if (memory_vec[current_cluster] == 0xF7)
			{
				continue;
			}
			new_data++;
			if (new_data == new_size)
			{
				break;
			}
		}
		memory_vec[current_cluster + 1] = 0xFFFF;
		current_cluster += 2;

		for (current_cluster; memory_vec[current_cluster] != 0xFFFF; current_cluster++)
		{
			int tmp = current_cluster;
			if (memory_vec[current_cluster] == 0xF7)
			{
				continue;
			}
			if (memory_vec[current_cluster] != current_cluster)
			{
				tmp = memory_vec[current_cluster] - 1;
			}
			memory_vec[current_cluster] = 0x0000;
			current_cluster = tmp;
		}
		memory_vec[current_cluster++] = 0x0000;
	}
	else
	{
		uint32_t current_cluster = file->first_cluster_num;
		for (current_cluster; memory_vec[current_cluster] != 0xFFFF; current_cluster++)
		{
			int tmp = current_cluster;
			if (memory_vec[current_cluster] == 0xF7)
			{
				continue;
			}
			if (memory_vec[current_cluster] != current_cluster)
			{
				tmp = memory_vec[current_cluster] - 1;
			}
			current_cluster = tmp;
		}

		uint32_t new_cluster = current_cluster;
		memory_vec[current_cluster] = 0x0000;

		while (new_data_size + 1 != 0)
		{
			if (memory_vec[current_cluster] == 0x0000)
			{
				memory_vec[new_cluster] = current_cluster;
				new_cluster = current_cluster;
				--new_data_size;
			}
			++current_cluster;
		}
		memory_vec[new_cluster] = 0xFFFF;
	}
	file->size = new_size;
}

void deleteFile(std::string file_name)
{
	for (auto iter = fat16.begin(); iter != fat16.end(); ++iter)
	{
		if (iter->file_name == file_name)
		{
			fat16.erase(iter);
			return;
		}
	}
}

void findFile(std::string file_name)
{
	for (const auto& fat : fat16)
	{
		if (fat.file_name == file_name)
		{
			std::cout << file_name << ": ";
			for (uint32_t i = fat.first_cluster_num; memory_vec[i] != 0xFFFF; i++)
			{
				if (memory_vec[i] == 0xF7)
				{
					continue;
				}
				std::cout << " " << std::setw(4) << std::hex << memory_vec[i] << " ";
			}
			std::cout << std::endl << std::endl;
			return;
		}
	}
	std::cerr << file_name << " file is not exist in this system" << std::endl;
}
void coutFAT()
{
	if (fat16.empty())
	{
		std::cerr << "FAT16 is empty" << std::endl;
	}
	else
	{
		for (const auto& fat : fat16)
		{
			std::cout << fat.file_name << ": ";
			for (uint32_t i = fat.first_cluster_num; memory_vec[i] != 0xFFFF; i++)
			{
				if (memory_vec[i] == 0xF7)
				{
					continue;
				}
				std::cout << " " << std::setw(4) << std::hex << memory_vec[i] << " ";
				if (memory_vec[i] != i)
				{
					i = memory_vec[i] - 1;
				}
			}
			std::cout << std::endl;
		}
	}
}
void tableFAT()
{
	uint32_t size = 18;
	for (const auto& fat : fat16)
	{
		size += fat.size;
	}
	if (size % 16 != 0)
	{
		size += 15 - (size % 16);
	}
	if (size > fat16_set.cluster_num)
	{
		size = fat16_set.cluster_num;
	}
	for (uint32_t i = 0; i < size; ++i)
	{
		if (i % 16 == 0)
		{
			std::cout << std::endl;
			std::cout << std::setw(4) << i << " : ";
		}
		std::cout << " " << std::setw(4) << std::hex << memory_vec[i] << " ";
	}
	std::cout << std::endl;
}