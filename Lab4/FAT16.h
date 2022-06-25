#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <math.h>

struct FileStructure	// FAT parameters
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
struct FAT16Settings	// define additional assignement about defective clusters
{
	uint16_t cluster_num = 0xffff;
	uint16_t defect_custer_num = 0x1000;
};

void addFile(std::string file_name, uint32_t file_size); // creating file
void resizeFile(std::string file_name, uint32_t var); // resizing file
void deleteFile(std::string file_name); // deleting file
void findFile(std::string file_name); // finding file by indentifier
void tableFAT(); // output of memory table
void coutFAT(); // FAT output
void initFAT(); // first zero initialization 
void menu(); // user-communication interface