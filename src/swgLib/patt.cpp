/** -*-c++-*-
 *  \class  patt
 *  \file   patt.cpp
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
#include <swgLib/patt.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

patt::patt()
{
}

patt::~patt()
{
}

std::size_t patt::read(std::istream& file)
{
	std::size_t pattSize;
	std::size_t total = base::readFormHeader(file, "PATT", pattSize);
	pattSize += 8;
	std::cout << "Found PATT: " << pattSize - 12 << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000..0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Attachment Version: " << _version << "\n";

	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _attachmentPath);
	total += base::read(file, _startPercentMin);
	total += base::read(file, _startPercentMax);
	if (0 == _version) {
		int32_t junk;
		total += base::read(file, junk);
		total += base::read(file, junk);
		total += base::read(file, junk);
		total += base::read(file, junk);
		total += base::read(file, junk);
	}
	total += base::read(file, _killAttachmentOnParticleDeath);
	if (_version > 0) {
		total += base::read(file, _spawn);
	}
	else {
		_spawn = 2;
	}

	if (pattSize == total) {
		std::cout << "Finished reading PATT\n";
	}
	else {
		std::cout << "FAILED in reading PATT\n"
			<< "Read " << total << " out of " << pattSize << "\n";
		exit(0);
	}

	return total;
}

const std::string patt::getAttachmentPath() const {
	return _attachmentPath;
}

const float& patt::getStartPercentMin() const {
	return _startPercentMin;
}

const float& patt::getStartPercentMax() const {
	return _startPercentMax;
}

bool patt::willKillAttachmentOnParticleDeath() const {
	return _killAttachmentOnParticleDeath;
}

const uint32_t patt::getSpawn() const {
	return _spawn;
}
