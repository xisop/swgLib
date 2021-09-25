/** -*-c++-*-
 *  \class  stag
 *  \file   stag.cpp
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

#include <swgLib/stag.hpp>
#include <swgLib/base.hpp>

using namespace ml;

stag::stag() {
}

stag::~stag() {
}

std::size_t stag::read(std::istream& file) {
	std::size_t stagSize;
	std::size_t total = base::readFormHeader(file, "STAG", stagSize);
	stagSize += 8;
	std::cout << "Found FORM STAG: " << stagSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	std::string form;
	base::peekHeader(file, form, size, type);
	std::cout << "Peek: " << form << " " << type << "\n";
	total += base::readRecordHeader(file, type, size);

	_version = base::tagToVersion(type);
	if (_version > 2)
	{
		std::cout << "Expected FORM of type 0000 or 0001. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found FORM " << type << ": " << size - 4 << " bytes\n";

	total += base::read(file, _colorOperation);
	std::cout << "Color operation: " << (int)_colorOperation << "\n";

	total += base::read(file, _colorArgument0);
	total += base::read(file, _colorArgument0Complement);
	total += base::read(file, _colorArgument0AlphaReplicate);

	std::cout << "Color argument 0: " << (int)_colorArgument0 << "\n";
	std::cout << "Color argument 0 complement: " << std::boolalpha << _colorArgument0Complement << "\n";
	std::cout << "Color argument 0 alpha replicate: " << std::boolalpha << _colorArgument0AlphaReplicate << "\n";

	total += base::read(file, _colorArgument1);
	total += base::read(file, _colorArgument1Complement);
	total += base::read(file, _colorArgument1AlphaReplicate);

	std::cout << "Color argument 1: " << (int)_colorArgument1 << "\n";
	std::cout << "Color argument 1 complement: " << std::boolalpha << _colorArgument1Complement << "\n";
	std::cout << "Color argument 1 alpha replicate: " << std::boolalpha << _colorArgument1AlphaReplicate << "\n";

	total += base::read(file, _colorArgument2);
	total += base::read(file, _colorArgument2Complement);
	total += base::read(file, _colorArgument2AlphaReplicate);

	std::cout << "Color argument 2: " << (int)_colorArgument2 << "\n";
	std::cout << "Color argument 2 complement: " << std::boolalpha << _colorArgument2Complement << "\n";
	std::cout << "Color argument 2 alpha replicate: " << std::boolalpha << _colorArgument2AlphaReplicate << "\n";

	total += base::read(file, _alphaOperation);
	std::cout << "Alpha operation: " << (int)_alphaOperation << "\n";

	total += base::read(file, _alphaArgument0);
	total += base::read(file, _alphaArgument0Complement);

	std::cout << "Alpha argument 0: " << (int)_alphaArgument0 << "\n";
	std::cout << "Alpha argument 0 complement: " << std::boolalpha << _alphaArgument0Complement << "\n";

	total += base::read(file, _alphaArgument1);
	total += base::read(file, _alphaArgument1Complement);

	std::cout << "Alpha argument 1: " << (int)_alphaArgument1 << "\n";
	std::cout << "Alpha argument 1 complement: " << std::boolalpha << _alphaArgument1Complement << "\n";

	total += base::read(file, _alphaArgument2);
	total += base::read(file, _alphaArgument2Complement);

	std::cout << "Alpha argument 2: " << (int)_alphaArgument2 << "\n";
	std::cout << "Alpha argument 2 complement: " << std::boolalpha << _alphaArgument2Complement << "\n";

	total += base::read(file, _resultArgument);
	total += base::read(file, _textureTag);
	total += base::read(file, _textureCoordinateSetTag);
	if (0 == _version) {
		total += base::read(file, _textureAddressU);
		total += base::read(file, _textureAddressV);
		total += base::read(file, _textureAddressW);
		total += base::read(file, _textureMipFilter);
		total += base::read(file, _textureMinificationFilter);
		total += base::read(file, _textureMagnificationFilter);
	}
	total += base::read(file, _textureCoordinateGeneration);

	if (stagSize == total)
	{
		std::cout << "Finished reading STAG\n";
	}
	else
	{
		std::cout << "FAILED in reading STAG\n";
		std::cout << "Read " << total << " out of " << stagSize << "\n";
		exit(0);
	}

	return total;
}
