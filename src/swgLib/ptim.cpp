/** -*-c++-*-
 *  \class  ptim
 *  \file   ptim.cpp
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
#include <swgLib/ptim.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

ptim::ptim()
{
}

ptim::~ptim()
{
}

// Particle Timing
std::size_t ptim::read(std::istream& file)
{
	std::size_t ptimSize;
	std::size_t total = base::readFormHeader(file, "PTIM", ptimSize);
	ptimSize += 8;
	std::cout << "Found FORM PTIM: " << ptimSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);

	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000..0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "PTIM version: " << (int)_version << "\n";

	total += base::read(file, _startDelayMin);
	total += base::read(file, _startDelayMax);
	total += base::read(file, _loopDelayMin);
	total += base::read(file, _loopDelayMax);
	total += base::read(file, _loopCountMin);
	total += base::read(file, _loopCountMax);

	if (0 == _version) {
		if (_loopCountMin <= -1) {
			_loopCountMax = -1;
		}
	}

	std::cout
		<< "Start delay min: " << _startDelayMin << "\n"
		<< "Start delay max: " << _startDelayMax << "\n"
		<< "Loop delay min: " << _loopDelayMin << "\n"
		<< "Loop delay max: " << _loopDelayMax << "\n"
		<< "Loop count min: " << _loopCountMin << "\n"
		<< "Loop count max: " << _loopCountMax << "\n";

	if (ptimSize == total)
	{
		std::cout << "Finished reading PTIM\n";
	}
	else
	{
		std::cout << "FAILED in reading PTIM\n"
			<< "Read " << total << " out of " << ptimSize << "\n";
		exit(0);
	}

	return total;
}
