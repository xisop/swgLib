/** -*-c++-*-
 *  \class  sps
 *  \file   sps.cpp
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

#include <swgLib/sps.hpp>
#include <swgLib/base.hpp>

using namespace ml;

sps::sps() {
}

sps::~sps() {
}

std::size_t sps::read(std::istream& file, bool skipSIDX) {
	std::size_t spsSize;
	std::size_t total = base::readFormHeader(file, "SPS ", spsSize);
	spsSize += 8;// Add omitted fields from form header...
	std::cout << "Found form SPS: " << spsSize << "\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	_version = base::tagToVersion(type);
	if (_version > 1)
	{
		std::cout << "Expected FORM of type 0000 or 0001. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found form " << type << "\n";
	std::cout << "SPS version " << (int)_version << "\n";

	total += base::readRecordHeader(file, "CNT ", size);
	total += base::read(file, _numberOfShaders);
	_shaderPrimitives.resize(_numberOfShaders);
	std::cout << "Number of shaders: " << _numberOfShaders << "\n";

	for (auto &sp : _shaderPrimitives ) {

		total += sp.read(file, skipSIDX);
	}

	if (total == spsSize)
	{
		std::cout << "Finished reading SPS\n";
	}
	else
	{
		std::cout << "Error reading SPS!\n";
		std::cout << "Read " << total << " out of " << spsSize << "\n";
		exit(0);
	}

	return total;
}

const std::vector<std::string>& sps::getShaderFiles() const {
	return _shaderFiles;
}

const std::vector<shaderPrimitive>& sps::getShaderPrimitives() const {
	return _shaderPrimitives;
}

const shaderPrimitive& sps::getShaderPrimitive(const std::size_t& sp) const {
	return _shaderPrimitives.at(sp);
}
