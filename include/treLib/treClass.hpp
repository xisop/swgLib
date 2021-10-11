/** -*-c++-*-
 *  \class  treClass
 *  \file   treClass.hpp
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
#include <sstream>
#include <string>
#include <vector>
#include <treLib/treFileRecord.hpp>
#include <treLib/treDataBlock.hpp>

#ifndef TRECLASS_HPP
#define TRECLASS_HPP 1

class treClass
{
public:
	treClass();
	~treClass();

	bool readFile(const std::string& treName);
	bool readFile(std::ifstream& file);

	bool writeFile(const std::string& treName);

	void setFileBlockCompression(const uint32_t& f);
	void setNameBlockCompression(const uint32_t& f);
	uint32_t getFileBlockCompression() { return fileCompression; }
	uint32_t getNameBlockCompression() { return nameCompression; }

	const std::string& getVersion() const { return version; }
	void setVersion(const std::string& v) { version = v; }

	void printHeader(std::ostream& os) const;
	void printFileBlock(std::ostream& os) const;
	void printNameBlock(std::ostream& os, bool verbose=false) const;

	bool saveRecordAsFile(const uint32_t& recordNum);
	std::stringstream* saveRecordAsStream(const uint32_t& recordNum,
		bool verbose = false);

	std::vector<treFileRecord>& getFileRecordList() { return fileRecordList; }
	bool getFileRecordIndex(const std::string& recordName,
		uint32_t& index) const;
	const std::string& getFilename() const
	{
		return filename;
	}

	uint32_t getNumRecords() const
	{
		return numRecords;
	}

protected:
	bool readHeader(std::istream& file);
	bool readFileBlock(std::istream& file);
	bool readNameBlock(std::istream& file);
	bool readMD5sums(std::istream& file);

	bool writeHeader(std::ostream& file);
	bool writeFileBlock(std::ostream& file);

	std::string filename;
	std::ifstream treFile;

	std::string version;
	uint32_t numRecords;
	uint32_t fileOffset;
	uint32_t fileCompression;
	uint32_t fileSize;
	uint32_t fileFinalSize;
	uint32_t nameCompression;
	uint32_t nameSize;
	uint32_t nameFinalSize;

	std::vector<treFileRecord> fileRecordList;
	treDataBlock fileBlock;
	treDataBlock nameBlock;
};

#endif
