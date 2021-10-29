/** -*-c++-*-
 *  \class  emgp
 *  \file   emgp.cpp
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
#include <swgLib/emgp.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

emgp::emgp()
{
}

emgp::~emgp()
{
}

std::size_t emgp::read(std::istream& file)
{
	std::size_t emgpSize;
	std::size_t total = base::readFormHeader(file, "EMGP", emgpSize);
	emgpSize += 8;
	std::cout << "Found FORM EMGP: " << emgpSize - 12 << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000..0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "Emitter Group Version: " << _version << "\n";

	if (1 == _version) {
		total += _timing.read(file);
	}

	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _numEmitterDescriptions);
	std::cout << "Emitter Group: Num Descriptions: " << _numEmitterDescriptions << "\n";

	_emitterDescriptions.resize(_numEmitterDescriptions);
	for (auto& ed : _emitterDescriptions) {
		total += ed.read(file);
	}

	if (emgpSize == total)
	{
		std::cout << "Finished reading EMGP" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading EMGP\n"
			<< "Read " << total << " out of " << emgpSize << "\n";
		exit(0);
	}

	return total;
}
