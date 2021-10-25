/** -*-c++-*-
 *  \class  base
 *  \file   base.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2006-2021 Ken Sewell

 This file is part of swgLib.

 swgLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 swgLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with swgLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <swgLib/base.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace ml;

#ifndef PLATFORM_LITTLE_ENDIAN
#if BYTE_ORDER == LITTLE_ENDIAN
#define PLATFORM_LITTLE_ENDIAN 1
#endif
#endif

tag::tag() {
}

tag::~tag() {
}

bool tag::operator<(const tag& t) const {
	return (this->_raw < t._raw);
}

bool tag::operator==(const tag& t) const {
	return (this->_raw == t._raw);
}

std::size_t tag::read(std::istream& file) {
	// Read 4 char tag as uint32_t...
	base::read(file, _raw);
	_str.clear();

	// Return empty string if 0...
	if (0 == _raw) {
		return 4;
	}

	// Reverse characters and assign to tag string...
	char* tagPtr = (char*)(&_raw);
	_str.clear();
	for (auto i = 3; i >= 0; --i) {
		_str += tagPtr[i];
	}

	return 4;
}

std::size_t tag::write(std::ostream& file) const {
	// Write 4 char tag as uint32_t...
	return base::write(file, _raw);
}

const uint32_t& tag::raw() const {
	return _raw;
}
const std::string& tag::str() const {
	return _str;
}

std::ostream& operator<<(std::ostream& os, const ml::tag& t) {
	os << t.str() << "(" << t.raw() << ")";
	return os;
}

// ****************************************************************************

std::string base::getType(std::istream& file)
{
	std::string form;
	std::size_t size;
	std::string type;

	// Peek at next record, but keep file at same place.
	peekHeader(file, form, size, type);

	if ("FORM" == form)
	{
		return type;
	}
	else
	{
		unsigned int x;
		read(file, x);

		// .str string file
		if (x == 0xabcd)
		{
			return std::string("ABCD");
		}

		return std::string("");
	}
}

bool base::isOfType(std::istream& file, const std::string& Type)
{
	std::string form;
	std::size_t size;
	std::string type;

	// Get current file position...
	const std::streampos position = file.tellg();

	// Read first FORM
	readFormHeader(file, form, size, type);

	// Reposition file to original location
	file.seekg(position, std::ios_base::beg);

	return("FORM" == form && Type == type);
}

std::size_t base::readBigEndian(std::istream& file,
	const std::size_t& size,
	char* buffer
)
{
#ifdef PLATFORM_LITTLE_ENDIAN
	for (unsigned int i = 0; i < size; ++i)
	{
		file.read(&(buffer[size - 1 - i]), 1);
	}
#else
	file.read(buffer, size);
#endif

	return size;
}

std::size_t base::writeBigEndian(std::ostream& file,
	const std::size_t& size,
	char* buffer
)
{
#ifdef PLATFORM_LITTLE_ENDIAN
	for (unsigned int i = 0; i < size; ++i)
	{
		file.write(&(buffer[size - 1 - i]), 1);
	}
#else
	file.write(buffer, size);
#endif

	return size;
}

void base::peekHeader(std::istream& file,
	std::string& form,
	std::size_t& size,
	std::string& type)
{
	// Peek at next record, but keep file at same place.
	const std::streampos position = file.tellg();
	readFormHeader(file, form, size, type);
	file.seekg(position, std::ios_base::beg);
}

// **************************************************

std::size_t base::readRecordHeader(std::istream& file,
	std::string& type,
	std::size_t& size)
{
	char tempType[5];
	file.read(tempType, 4);
	tempType[4] = 0;
	type = tempType;
	uint32_t tempSize = 0;
	readBigEndian(file, sizeof(tempSize), (char*)&tempSize);
	size = tempSize;

	return 8;
}

std::size_t base::readRecordHeader(std::istream& file,
	const std::string& expectedType,
	std::size_t& size)
{
	char tempType[5];
	file.read(tempType, 4);
	tempType[4] = 0;
	if (expectedType != tempType) {
		std::cout << "Expected record type of " << expectedType
			<< ", found type of '" << tempType << "'\n";
		exit(0);
	}
	uint32_t tempSize = 0;
	readBigEndian(file, sizeof(tempSize), (char*)&tempSize);
	size = tempSize;

	return 8;
}

std::size_t base::writeRecordHeader(std::ostream& file,
	const std::string& type,
	const std::size_t& size)
{
	file.write(type.c_str(), 4);

	uint32_t tempSize = (uint32_t)size;
	writeBigEndian(file, sizeof(tempSize), (char*)&tempSize);

	return 8;
}

// **************************************************

std::size_t base::readFormHeader(std::istream& file,
	std::string& form,
	std::size_t& size,
	std::string& type)
{
	std::size_t total = readRecordHeader(file, form, size);
	char tempType[5];
	file.read(tempType, 4);
	total += 4;

	tempType[4] = 0;
	type = tempType;

	return total;
}

std::size_t base::writeFormHeader(std::ostream& file,
	const std::size_t& size,
	const std::string& type)
{
	file.write("FORM", 4);
	uint32_t tempSize = (uint32_t)size;
	writeBigEndian(file, sizeof(tempSize), (char*)&tempSize);
	file.write(type.c_str(), 4);

	return 12;
}

// **************************************************

std::size_t base::readFormHeader(std::istream& file,
	std::string& type,
	std::size_t& size)
{
	std::size_t total = readRecordHeader(file, "FORM", size);

	char tempType[5];
	file.read(tempType, 4);
	total += 4;
	tempType[4] = 0;

	type = std::string(tempType);

	return total;
}

std::size_t base::readFormHeader(std::istream& file,
	const std::string& expectedType,
	std::size_t& size)
{
	std::size_t total = readRecordHeader(file, "FORM", size);

	char tempType[5];
	file.read(tempType, 4);
	total += 4;
	tempType[4] = 0;

	std::string type(tempType);
	if (expectedType != type)
	{
		std::cout << "Expected FORM of type " << expectedType
			<< ", found: " << type << "\n";
		exit(0);
	}

	return total;
}

std::size_t base::readUnknown(std::istream& file,
	const std::size_t size)
{
	file.sync();
	for (unsigned int i = 0; i < size; ++i)
	{
		unsigned char data;
		file.read((char*)&data, 1);
		if (file.eof()) {
			std::cout << "\nUnexpected EOF\n";
			exit(0);
		}
		if (
			(data >= '.' && data <= '9') ||
			(data >= 'A' && data <= 'Z') ||
			(data >= 'a' && data <= 'z') ||
			(data == '\\') ||
			(data == '_')
			)
		{
			std::cout << data;
		}
		else
		{
			std::cout << std::hex << "0x" << std::setw(2)
				<< std::setfill('0')
				<< (unsigned int)data
				<< " " << std::dec;
		}
	}
	std::cout << std::endl;
	return size;
}

// **************************************************

std::size_t base::read(std::istream& file, bool& data)
{
	uint8_t temp;
	read(file, temp);
	if (0 == temp) { data = false; }
	else { data = true; }
	return sizeof(uint8_t);
}

std::size_t base::write(std::ostream& file, const bool& data)
{
	uint8_t temp = 0;
	if (data) { temp = 1; }
	return write(file, temp);
}

// **************************************************

std::size_t base::read(std::istream& file, int8_t& data)
{
	file.read((char*)&data, sizeof(char));
	return sizeof(char);
}

std::size_t base::write(std::ostream& file, const int8_t& data)
{
	file.write((char*)&data, sizeof(char));
	return sizeof(char);
}

// **************************************************

std::size_t base::read(std::istream& file, char& data)
{
	file.read(&data, sizeof(char));
	return sizeof(char);
}

std::size_t base::write(std::ostream& file, const char& data)
{
	file.write(&data, sizeof(char));
	return sizeof(char);
}

// **************************************************

std::size_t base::read(std::istream& file, unsigned char& data)
{
	file.read((char*)&data, sizeof(unsigned char));
	return sizeof(unsigned char);
}

std::size_t base::write(std::ostream& file, const unsigned char& data)
{
	file.write((char*)&data, sizeof(unsigned char));
	return sizeof(unsigned char);
}

// **************************************************

std::size_t base::read(std::istream& file, int16_t& data)
{
	file.read((char*)&data, sizeof(int16_t));
	return sizeof(int16_t);
}

std::size_t base::write(std::ostream& file, const int16_t& data)
{
	file.write((char*)&data, sizeof(int16_t));
	return sizeof(int16_t);
}

// **************************************************

std::size_t base::read(std::istream& file, uint16_t& data)
{
	file.read((char*)&data, sizeof(uint16_t));
	return sizeof(uint16_t);
}

std::size_t base::write(std::ostream& file, const uint16_t& data)
{
	file.write((char*)&data, sizeof(uint16_t));
	return sizeof(uint16_t);
}

// **************************************************

std::size_t base::read(std::istream& file, int32_t& data)
{
	file.read((char*)&data, sizeof(int32_t));
	return sizeof(int32_t);
}

std::size_t base::write(std::ostream& file, const int32_t& data)
{
	file.write((char*)&data, sizeof(int32_t));
	return sizeof(int32_t);
}

// **************************************************

std::size_t base::read(std::istream& file, uint32_t& data)
{
	file.read((char*)&data, sizeof(uint32_t));
	return sizeof(uint32_t);
}

std::size_t base::write(std::ostream& file, const uint32_t& data)
{
	file.write((char*)&data, sizeof(uint32_t));
	return sizeof(uint32_t);
}

// **************************************************

std::size_t base::read(std::istream& file, float& data)
{
	file.read((char*)&data, sizeof(float));
	return sizeof(float);
}

std::size_t base::write(std::ostream& file, const float& data)
{
	file.write((char*)&data, sizeof(float));
	return sizeof(float);
}

// **************************************************
std::size_t base::read(std::istream& file, std::string& data, const std::size_t &length) {
	char temp[255];
	file.read(temp, length);
	for (uint32_t i = 0; i < length; ++i) {
		if (0 == temp[i]) { temp[i] = ' '; }
	}
	temp[length-1] = 0;
	data = temp;
	return length;
}

std::size_t base::read(std::istream& file, std::string& data)
{
	char temp[255];
	file.getline(temp, 255, 0);
	data = temp;
	return(data.size() + 1);
}

std::size_t base::write(std::ostream& file, const std::string& data)
{
	file.write(data.c_str(), data.size() + 1);
	return(data.size() + 1);
}

// **************************************************

std::size_t base::read(std::istream& file, vector3& v) {
	return v.read(file);
}

std::size_t base::write(std::ostream& file, const vector3& v) {
	return v.write(file);
}

// **************************************************

std::size_t base::read(std::istream& file, matrix3x3& mat) {
	return mat.read(file);
}

std::size_t base::write(std::ostream& file, const matrix3x3& mat) {
	return mat.write(file);
}

// **************************************************

std::size_t base::read(std::istream& file, matrix3x4& mat) {
	return mat.read(file);
}

std::size_t base::write(std::ostream& file, const matrix3x4& mat) {
	return mat.write(file);
}

// **************************************************

std::size_t base::read(std::istream& file, tag& t) {
	return t.read(file);
}

std::size_t base::write(std::ostream& file, const tag& t) {
	return t.write(file);
}

// **************************************************

bool base::fixSlash(std::string& filename)
{
	for (unsigned int i = 0; i < filename.size(); ++i)
	{
		if (filename[i] == '\\')
		{
			filename[i] = '/';
		}
	}

	return true;
}

uint8_t base::tagToVersion(const std::string& tag) {
	if (tag.size() != 4) { return 0; }

	if ("0000" == tag) { return 0; }
	else if ("0001" == tag) { return  1; }
	else if ("0002" == tag) { return  2; }
	else if ("0003" == tag) { return  3; }
	else if ("0004" == tag) { return  4; }
	else if ("0005" == tag) { return  5; }
	else if ("0006" == tag) { return  6; }
	else if ("0007" == tag) { return  7; }
	else if ("0008" == tag) { return  8; }
	else if ("0009" == tag) { return  9; }
	else if ("0010" == tag) { return 10; }

	return 0;
}

uint32_t base::typeToNumber(const std::string& type) {
	std::istringstream sstr(type);
	uint32_t number;
	sstr >> number;
	return number;
}
