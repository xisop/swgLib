/** -*-c++-*-
 *  \class  matl
 *  \file   matl.cpp
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

#include <swgLib/matl.hpp>
#include <swgLib/base.hpp>

using namespace ml;

matl::matl() {
}

matl::~matl() {
}

std::size_t matl::read(std::istream& file) {
	// Read MATL record
	std::size_t matlSize;
	std::size_t total = base::readRecordHeader(file, "MATL", matlSize);
	matlSize += 8; // Include Tag/Size from header...
	std::cout << "Found record MATL: " << matlSize << " bytes\n";

	if (76 != matlSize)	{
		std::cout << "Expected MATL size 68. Found: " << matlSize << "\n";
		exit(0);
	}

	total += _ambient.read32(file); // ARGB ordering
	total += _diffuse.read32(file); // ARGB ordering
	total += _emissive.read32(file); // ARGB ordering
	total += _specular.read32(file); // ARGB ordering
	total += base::read(file, _power); // ARGB ordering

	std::cout
		<< " Ambient: " << _ambient << "\n"
		<< " Diffuse: " << _diffuse << "\n"
		<< "Emissive: " << _emissive << "\n"
		<< "Specular: " << _specular << "\n"
		<< "   Power: " << _power << "\n";

	if (matlSize == total)
	{
		std::cout << "Finished reading MATL\n";
	}
	else
	{
		std::cout << "FAILED in reading MATL\n";
		std::cout << "Read " << total << " out of " << matlSize << "\n";
		exit(0);
	}

	return total;
}

void matl::setNameTag(const tag& nameTag) {
	_nameTag = nameTag;
}

const tag& matl::getNameTag() const {
	return _nameTag;
}

const color4& matl::getAmbient() const {
	return _ambient;
}

const color4& matl::getDiffuse() const {
	return _diffuse;
}

const color4& matl::getEmissive() const {
	return _emissive;
}

const color4& matl::getSpecular() const {
	return _specular;
}

const float& matl::getPower() const {
	return _power;
}
