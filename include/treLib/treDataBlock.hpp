/** -*-c++-*-
 *  \class  treDataBlock
 *  \file   treDataBlock.hpp
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

#include <istream>
#include <ostream>
#include <memory>
#include <string>
#include <vector>

#ifndef TREDATABLOCK_HPP
#define TREDATABLOCK_HPP

class treDataBlock
{
public:
	treDataBlock();
	~treDataBlock();

	bool readAndUncompress(
		std::istream& file,
		const int& format,
		const uint32_t& compSize,
		const uint32_t& uncompSize
	);

	bool compressAndWrite(
		std::ostream& file,
		const int& format
	);

	bool isChecksumCorrect(const uint32_t& csum);
	void calculateMD5sum(const char* mem, const uint32_t& memSize);
	const std::vector<unsigned char>& getMD5sum() const;

	void freeCompressedData();
	void freeUncompressedData();

	char* getCompressedDataPtr();
	char* getUncompressedDataPtr();

	bool allocateUncompressedData(const uint32_t& size);

	bool setUncompressedData(
		const char* newData,
		const uint32_t& newDataSize
	);

	uint32_t getUncompressedSize() const
	{
		return uncompressedSize;
	}
	uint32_t getCompressedSize() const
	{
		return compressedSize;
	}

protected:
	uint32_t checksum;
	uint32_t uncompressedSize;
	uint32_t compressedSize;
	std::vector<unsigned char> md5sum;
	std::unique_ptr<char> data;
	std::unique_ptr<char> compData;

};

#endif
