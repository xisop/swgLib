/** -*-c++-*-
 *  \class  shaderPrimitive
 *  \file   shaderPrimitive.cpp
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

#include <swgLib/shaderPrimitive.hpp>
#include <swgLib/base.hpp>

using namespace ml;

shaderPrimitive::shaderPrimitive() {
}

shaderPrimitive::~shaderPrimitive() {
}

std::size_t shaderPrimitive::read(std::istream& file, bool skipSIDX) {
	std::size_t spSize;
	std::string type;
	std::size_t total = base::readFormHeader(file, type, spSize);
	spSize += 8;
	int32_t spNumber = base::typeToNumber(type);
	std::cout << "Shader primitive " << spNumber << "\n";

	// Read Shader filename
	std::size_t size;
	total += base::readRecordHeader(file, "NAME", size);
	total += base::read(file, _name);
	base::fixSlash(_name);
	std::cout << "  name: " << _name << "\n";

	// Read Shader filename
	total += base::readRecordHeader(file, "INFO", size);

	// Read number of primitives...
	int32_t numberOfPrimitives = 0;
	total += base::read(file, numberOfPrimitives);
	_primitives.resize(numberOfPrimitives);

	for (auto& p : _primitives) {
		total += p.read(file, skipSIDX);
	}

	if (spSize == total)
	{
		std::cout << "Finished reading shaderPrimitive\n";
	}
	else
	{
		std::cout << "FAILED in reading shaderPrimitive\n";
		std::cout << "Read " << total << " out of " << spSize << "\n";
		exit(0);
	}
	return total;
}

const std::string& shaderPrimitive::getName() const {
	return _name;
}

const std::vector<primitive>& shaderPrimitive::getPrimitives() const {
	return _primitives;
}

const primitive& shaderPrimitive::getPrimitive(const std::size_t& p) const {
	return _primitives.at(p);
}
