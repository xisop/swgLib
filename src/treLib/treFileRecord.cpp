/** -*-c++-*-
 *  \class  treFileRecord
 *  \file   treFileRecord.cpp
 *  \author Ken Sewell

 treLib is used for the creation and deconstruction of .TRE files.
 Copyright (C) 2006-2021 Ken Sewell

 This file is part of treLib.

 treLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 treLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with treLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <treLib/treFileRecord.hpp>
#include <iostream>
#include <bitset>
#include <cstring> // For memcpy

treFileRecord::treFileRecord() :
	checksum(0),
	uncompressedSize(0),
	offset(0),
	format(0),
	size(0),
	nameOffset(0)
{
}

treFileRecord::treFileRecord(const treFileRecord& rec) :
	checksum(rec.checksum),
	uncompressedSize(rec.uncompressedSize),
	offset(rec.offset),
	format(rec.format),
	size(rec.size),
	nameOffset(rec.nameOffset)
{
}

bool treFileRecord::read(std::istream& file)
{
	file.read(reinterpret_cast<char*>(&checksum), sizeof(checksum));
	file.read(reinterpret_cast<char*>(&uncompressedSize),
		sizeof(uncompressedSize));
	file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
	file.read(reinterpret_cast<char*>(&format), sizeof(format));
	file.read(reinterpret_cast<char*>(&size), sizeof(size));
	file.read(reinterpret_cast<char*>(&nameOffset), sizeof(nameOffset));

	return true;
}

bool treFileRecord::write(std::ostream& file)
{
	file.write(reinterpret_cast<char*>(&checksum), sizeof(checksum));
	file.write(reinterpret_cast<char*>(&uncompressedSize),
		sizeof(uncompressedSize));
	file.write(reinterpret_cast<char*>(&offset), sizeof(offset));
	file.write(reinterpret_cast<char*>(&format), sizeof(format));
	file.write(reinterpret_cast<char*>(&size), sizeof(size));
	file.write(reinterpret_cast<char*>(&nameOffset), sizeof(nameOffset));

	return true;
}

uint32_t treFileRecord::readFromBuffer(const char* buffer)
{
	uint32_t bufOffset = 0;
	memcpy(&checksum, buffer + bufOffset, sizeof(checksum));
	bufOffset += sizeof(checksum);

	memcpy(&uncompressedSize, buffer + bufOffset, sizeof(uncompressedSize));
	bufOffset += sizeof(uncompressedSize);

	memcpy(&offset, buffer + bufOffset, sizeof(offset));
	bufOffset += sizeof(offset);

	memcpy(&format, buffer + bufOffset, sizeof(format));
	bufOffset += sizeof(format);

	memcpy(&size, buffer + bufOffset, sizeof(size));
	bufOffset += sizeof(size);

	memcpy(&nameOffset, buffer + bufOffset, sizeof(nameOffset));
	bufOffset += sizeof(nameOffset);

#if 0
	unsigned long int adler = crc32(0L, Z_NULL, 0);

	adler = crc32(adler, (Bytef*)buffer + 20, 4);
	adler = crc32(adler, (Bytef*)buffer + 16, 4);
	adler = crc32(adler, (Bytef*)buffer + 12, 4);
	adler = crc32(adler, (Bytef*)buffer + 8, 4);
	adler = crc32(adler, (Bytef*)buffer + 4, 4);

	std::cout << "            CRC32: ";
	std::cout.fill('0');
	std::cout.width(8);
	std::cout << std::hex << adler << std::endl;

	uint32_t sum = 0;
	for (int i = 4; i < 24; ++i)
	{
		sum += buffer[i];
	}

	std::cout << "              Sum: ";
	std::cout.fill('0');
	std::cout.width(8);
	std::cout << std::hex << sum << std::endl;



	std::cout << "         Checksum: ";
	std::cout.width(8);
	std::cout.fill('0');
	std::cout << std::hex << checksum;
	std::cout << " " << std::bitset<32>(checksum) << std::endl;

	std::cout << "Uncompressed size: ";
	std::cout.fill('0');
	std::cout.width(8);
	std::cout << std::hex << uncompressedSize;
	std::cout << " " << std::bitset<32>(uncompressedSize) << std::endl;

	std::cout << "           Offset: ";
	std::cout.width(8);
	std::cout.fill('0');
	std::cout << std::hex << offset;
	std::cout << " " << std::bitset<32>(offset) << std::endl;

	std::cout << "           Format: ";
	std::cout.width(8);
	std::cout.fill('0');
	std::cout << std::hex << format;
	std::cout << " " << std::bitset<32>(format) << std::endl;

	std::cout << "             Size: ";
	std::cout.width(8);
	std::cout.fill('0');
	std::cout << std::hex << size;
	std::cout << " " << std::bitset<32>(size) << std::endl;

	std::cout << "      Name offset: ";
	std::cout.width(8);
	std::cout.fill('0');
	std::cout << std::hex << nameOffset;
	std::cout << " " << std::bitset<32>(nameOffset) << std::endl;
	std::cout << std::dec << std::endl;

#endif

	return bufOffset;
}

uint32_t treFileRecord::writeToBuffer(char* buffer) const
{
	uint32_t bufOffset = 0;
	memcpy(buffer + bufOffset, &checksum, sizeof(checksum));
	bufOffset += sizeof(checksum);

	memcpy(buffer + bufOffset, &uncompressedSize, sizeof(uncompressedSize));
	bufOffset += sizeof(uncompressedSize);

	memcpy(buffer + bufOffset, &offset, sizeof(offset));
	bufOffset += sizeof(offset);

	memcpy(buffer + bufOffset, &format, sizeof(format));
	bufOffset += sizeof(format);

	memcpy(buffer + bufOffset, &size, sizeof(size));
	bufOffset += sizeof(size);

	memcpy(buffer + bufOffset, &nameOffset, sizeof(nameOffset));
	bufOffset += sizeof(nameOffset);

	return bufOffset;
}

void treFileRecord::print(std::ostream& os) const
{
	os << "Checksum: 0x" << std::hex << checksum
		<< std::dec << "\n"
		<< "Uncompressed size: " << uncompressedSize << "\n"
		<< "Offset: " << offset << "\n"
		<< "Format: " << format << "\n"
		<< "Size: " << size << "\n"
		<< "Name Offset: " << nameOffset << "\n"
		<< "MD5 sum: ";
	for (const auto &i : md5sum)
	{
		os << std::hex << (uint32_t)(i);
	}
	os << std::dec << "\n";
}

void treFileRecord::operator=(const treFileRecord& src)
{
	if (this != &src)
	{
		this->checksum = src.checksum;
		this->uncompressedSize = src.uncompressedSize;
		this->offset = src.offset;
		this->format = src.format;
		this->size = src.size;
		this->nameOffset = src.nameOffset;
	}
}

bool treFileRecord::operator==(const treFileRecord& src) const
{
	return(
		(this->checksum == src.checksum) &&
		(this->uncompressedSize == src.uncompressedSize) &&
		(this->offset == src.offset) &&
		(this->format == src.format) &&
		(this->size == src.size) &&
		(this->nameOffset == src.nameOffset)
		);
}

bool treFileRecord::readMD5(std::istream& file)
{
	md5sum.clear();
	for (int j = 0; j < 16; ++j)
	{
		char c;
		file.read(&c, 1);
		md5sum.push_back(c);
	}

	return true;
}

bool treFileRecord::writeMD5(std::ostream& file) const
{
	std::vector<unsigned char>::const_iterator j;
	for (j = md5sum.begin(); j < md5sum.end(); ++j)
	{
		file.write((char*)&(*j), 1);
	}

	return true;
}

uint32_t treFileRecord::generateChecksum() const
{
	unsigned char* temp;

	uint32_t cksum = 0;

	temp = (unsigned char*)&uncompressedSize;
	for (int i = 0; i < 4; ++i)
	{
		cksum += temp[i];
	}

	temp = (unsigned char*)&offset;
	for (int i = 0; i < 4; ++i)
	{
		cksum += temp[i];
	}

	temp = (unsigned char*)&format;
	for (int i = 0; i < 4; ++i)
	{
		cksum += temp[i];
	}

	temp = (unsigned char*)&size;
	for (int i = 0; i < 4; ++i)
	{
		cksum += temp[i];
	}

	temp = (unsigned char*)&nameOffset;
	for (int i = 0; i < 4; ++i)
	{
		cksum += temp[i];
	}

	//std::cout << "Calculated cksum: " << cksum << std::endl;

	return cksum;
}
