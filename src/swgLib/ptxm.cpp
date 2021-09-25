/** -*-c++-*-
 *  \class  ptxm
 *  \file   ptxm.cpp
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

#include <swgLib/ptxm.hpp>
#include <swgLib/base.hpp>

using namespace ml;

ptxm::ptxm() {
}

ptxm::~ptxm() {
}

std::size_t ptxm::read(std::istream& file) {
	std::size_t ptxmSize;
	std::size_t total = base::readFormHeader(file, "PTXM", ptxmSize);
	ptxmSize += 8;
	std::cout << "Found FORM PTXM: " << ptxmSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);

	_version = base::tagToVersion(type);
	if (_version > 2)
	{
		std::cout << "Expected FORM of type 0000, 0001, or 0002. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found FORM " << type << ": " << size - 4 << " bytes\n";

	total += base::read(file, _textureIndex);
	std::cout << "Texture index: " << (int)_textureIndex << "\n";

	total += base::read(file, _textureTag);
	std::cout << "Texture tag: " << _textureTag << "\n";

	if (0 == _version)
	{
		uint32_t ignored;
		total += base::read(file, ignored);

		total += base::read(file, _textureAddressU);
		std::cout << "Texture address U: " << (int)_textureAddressU << "\n";
		total += base::read(file, _textureAddressV);
		std::cout << "Texture address V: " << (int)_textureAddressV << "\n";
		total += base::read(file, _textureAddressW);
		std::cout << "Texture address W: " << (int)_textureAddressW << "\n";

		total += base::read(file, _textureMipFilter);
		std::cout << "Texture mip filter: " << (int)_textureMipFilter << "\n";
		total += base::read(file, _textureMinFilter);
		std::cout << "Texture min filter: " << (int)_textureMinFilter << "\n";
		total += base::read(file, _textureMagFilter);
		std::cout << "Texture mag filter: " << (int)_textureMagFilter << "\n";

		uint8_t ignored2;
		total += base::read(file, ignored2);
	}

	if (1 == _version)
	{
		uint32_t ignored;
		total += base::read(file, ignored);
		uint8_t ignored2;
		total += base::read(file, ignored2);
	}

	if (ptxmSize == total)
	{
		std::cout << "Finished reading PTXM\n";
	}
	else
	{
		std::cout << "FAILED in reading PTXM\n";
		std::cout << "Read " << total << " out of " << ptxmSize << "\n";
		exit(0);
	}

	return total;
}
