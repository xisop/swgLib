/** -*-c++-*-
 *  \class  treFileRecord
 *  \file   treFileRecord.hpp
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

#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <treLib/treDataBlock.hpp>

#ifndef TREINDEX_HPP
#define TREINDEX_HPP 1

class treFileRecord
{
public:

	enum Size
	{
		SIZE = sizeof(int) * 6
	};

	treFileRecord();
	treFileRecord(const treFileRecord& rec);
	~treFileRecord() {};

	bool read(std::istream& file);
	bool write(std::ostream& file);
	void print(std::ostream& os) const;
	uint32_t readFromBuffer(const char* buffer);
	uint32_t writeToBuffer(char* buffer) const;

	void operator=(const treFileRecord& src);
	bool operator==(const treFileRecord& src) const;

	void setChecksum(const uint32_t& x) { checksum = x; }
	void setUncompressedSize(const uint32_t& x) { uncompressedSize = x; }
	void setOffset(const uint32_t& x) { offset = x; }
	void setFormat(const uint32_t& x) { format = x; }
	void setSize(const uint32_t& x) { size = x; }
	void setNameOffset(const uint32_t& x) { nameOffset = x; }
	void setFileName(const std::string& fn) { fileName = fn; }
	void setMD5sum(const std::vector<unsigned char>& newSum)
	{
		md5sum = newSum;
	}

	uint32_t getChecksum() const { return checksum; }
	uint32_t getUncompressedSize() const { return uncompressedSize; }
	uint32_t getOffset() const { return offset; }
	uint32_t getFormat() const { return format; }
	uint32_t getSize() const { return size; }
	uint32_t getNameOffset() const { return nameOffset; }
	const std::string& getFileName() const { return fileName; }
	const std::vector<unsigned char>& getMD5sum() const
	{
		return md5sum;
	}

	bool readMD5(std::istream& file);
	bool writeMD5(std::ostream& file) const;
	uint32_t generateChecksum() const;

	treDataBlock& getDataBlock() { return dataBlock; }

	static std::string getFormatStr(const uint32_t format);

protected:
	uint32_t checksum;
	uint32_t uncompressedSize;
	uint32_t offset;
	uint32_t format;
	uint32_t size;
	uint32_t nameOffset;
	std::vector<unsigned char> md5sum;
	std::string fileName;
	treDataBlock dataBlock;
};

#endif
