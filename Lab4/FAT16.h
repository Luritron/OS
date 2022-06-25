#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <math.h>

struct FileStructure	// ��������� FAT
{
	std::string file_name;
	uint16_t attribute;
	uint16_t reserve_field;
	int32_t create_time;
	int32_t create_date;
	int32_t time_last_access;
	int32_t date_last_access;
	int32_t reserverd;
	uint16_t time_modified;
	uint16_t date_modified;
	uint32_t first_cluster_num;
	uint32_t size;
};
struct FAT16Settings	// �������� ����� ���������� ��������� ��������
{
	uint16_t cluster_num = 0xffff;
	uint16_t defect_custer_num = 0x1000;
};

void addFile(std::string file_name, uint32_t file_size); //��������� �����
void resizeFile(std::string file_name, uint32_t var); //���� ������ �����
void deleteFile(std::string file_name); // ��������� �����
void findFile(std::string file_name); //����� ����� �� ���������������
void tableFAT(); //���� ������� ���'��
void coutFAT(); // ���� FAT
void initFAT(); // ��������� ���������� ������� ������
void menu(); // ��������� ��� ���������� � ������������