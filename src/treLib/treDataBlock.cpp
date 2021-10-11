/** -*-c++-*-
 *  \class  treDataBlock
 *  \file   treDataBlock.cpp
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

#include <treLib/treDataBlock.hpp>
#include <iostream>
#include <zlib.h>
#include <md5.h> // For md5
#include <memory.h>

treDataBlock::treDataBlock()
{
}

treDataBlock::~treDataBlock()
{
	freeCompressedData();
	freeUncompressedData();
}

void treDataBlock::freeCompressedData()
{
	compressedSize = 0;
	compData.reset();
}

void treDataBlock::freeUncompressedData()
{
	uncompressedSize = 0;
	data.reset();
}

char* treDataBlock::getUncompressedDataPtr()
{
	return data.get();
}

char* treDataBlock::getCompressedDataPtr()
{
	return compData.get();
}

bool treDataBlock::isChecksumCorrect(const uint32_t&/*csum*/)
{
	// Needs work...
	return true;
}

void treDataBlock::calculateMD5sum(
	const char* mem,
	const uint32_t& memSize
)
{
	// Clear old sum...
	md5sum.clear();

	if (NULL != mem)
	{
		md5_context md5;
		md5_starts(&md5);
		md5_update(&md5, (unsigned char*)mem, memSize);
		unsigned char mdArray[16];
		md5_finish(&md5, mdArray);
		for (int i = 0; i < 16; ++i)
		{
			md5sum.push_back(mdArray[i]);
		}
	}
}

const std::vector<unsigned char>& treDataBlock::getMD5sum() const
{
	return md5sum;
}

bool treDataBlock::allocateUncompressedData(const uint32_t& size)
{
	// Free any previously allocated memory...
	freeCompressedData();
	freeUncompressedData();

	// Allocate new memory...
	uncompressedSize = size;
	data.reset( new char[uncompressedSize] );

	return(nullptr != data );
}

bool treDataBlock::setUncompressedData(const char* newData,
	const uint32_t& newDataSize)
{
	freeCompressedData();
	freeUncompressedData();

	allocateUncompressedData(newDataSize);
	data.reset( new char[uncompressedSize] );
	if (nullptr == data)
	{
		return false;
	}

	memcpy(data.get(), newData, uncompressedSize);

	return true;
}

bool treDataBlock::readAndUncompress(
	std::istream& file,
	const int& format,
	const uint32_t& compSize,
	const uint32_t& uncompSize
)
{
	freeCompressedData();
	freeUncompressedData();

	if (2 == format)
	{
		data.reset(new char[uncompSize]);

		compressedSize = compSize;
		compData.reset( new char[compSize]);
		file.read(compData.get(), compSize);
		uncompressedSize = uncompSize;

		//std::cout << "Uncompressing data block...";
		int result = uncompress((Bytef*)data.get(),
			(uLongf*)&uncompressedSize,
			(Bytef*)compData.get(),
			compSize);

		if (Z_OK == result)
		{
			//std::cout << "success." << std::endl;
		}
		else if (Z_MEM_ERROR == result)
		{
			std::cout << __FILE__ << ": " << __LINE__
				<< ": uncompress: Memory error!" << std::endl;
			return false;
		}
		else if (Z_BUF_ERROR == result)
		{
			std::cout << __FILE__ << ": " << __LINE__
				<< ": uncompress: Buffer error!" << std::endl;
			return false;
		}
		else if (Z_DATA_ERROR == result)
		{
			std::cout << __FILE__ << ": " << __LINE__
				<< ": uncompress: Data error!" << std::endl;
			return false;
		}
		else
		{
			std::cout << __FILE__ << ": " << __LINE__
				<< ": uncompress: Unknown error!" << std::endl;
			return false;
		}

		if (uncompressedSize != uncompSize)
		{
			std::cout << __FILE__ << ": " << __LINE__
				<< ": Uncompressed size does not match expected size!"
				<< std::endl;
			return false;
		}
	}
	else if (0 == format) // No compression
	{
		data.reset( new char[uncompSize]);
		file.read(data.get(), uncompSize);
	}
	else
	{
		std::cout << __FILE__ << ": " << __LINE__
			<< ": Unknown format: " << format << std::endl;
		return false;
	}

	return true;
}

bool treDataBlock::compressAndWrite(
	std::ostream& file,
	const int& format
)
{
	// Fail if no data to write...
	if (NULL == data)
	{
		return false;
	}

	// Delete any previously compressed data...
	freeCompressedData();

	// Zlib compression...
	if (2 == format)
	{
		// Allocate temp buffer to compress data into...
		uint32_t tempDataLength = uncompressedSize * 2;
		char* tempData = new char[tempDataLength];

		// Compress the data...
		int result = compress(
			(Bytef*)tempData,
			(uLongf*)&tempDataLength,
			(const Bytef*)data.get(),
			uncompressedSize
		);

		if (Z_OK == result)
		{
			//std::cout << "success." << std::endl;
		}
		else if (Z_MEM_ERROR == result)
		{
			std::cout << "compress: Memory error!" << std::endl;
			delete[] tempData;
			return false;
		}
		else if (Z_BUF_ERROR == result)
		{
			std::cout << "compress: Buffer error!" << std::endl;
			delete[] tempData;
			return false;
		}
		else
		{
			std::cout << "compress: Unknown error!" << std::endl;
			delete[] tempData;
			return false;
		}

		// Create final compressed data and copy into it...
		compressedSize = tempDataLength;
		compData.reset( new char[compressedSize]);
		memcpy(compData.get(), tempData, compressedSize);

		// Calculate md5sum
		calculateMD5sum(compData.get(), compressedSize);

		// Delete the temp buffer...
		delete[] tempData;

		// Write compressed data...
		file.write(compData.get(), compressedSize);
	}
	else if (0 == format) // No compression
	{
		// Calculate md5sum
		calculateMD5sum(data.get(), uncompressedSize);

		file.write(data.get(), uncompressedSize);
	}
	else
	{
		std::cout << "Unknown format: " << format << std::endl;
		return false;
	}

	return true;
}
