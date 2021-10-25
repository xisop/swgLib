/** -*-c++-*-
 *  \class  prto
 *  \file   prto.cpp
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

#include <swgLib/base.hpp>
#include <swgLib/prto.hpp>
#include <swgLib/portal.hpp>
#include <swgLib/lght.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

prto::prto()
{
}

prto::~prto()
{
}

const int32_t& prto::getNumCells() const
{
	return _numCells;
}

const int32_t& prto::getNumPortals() const
{
	return _numPortals;
}

std::size_t prto::readPRTO(std::istream& file)
{
	std::size_t prtoSize;
	std::size_t total = base::readFormHeader(file, "PRTO", prtoSize);
	std::cout << "Found PRTO form" << ": " << prtoSize << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	std::cout << "Version: " << (int)_version << "\n";

	if (_version > 5) {
		std::cout << "Expected FORM of type 0000, 0001, 0002, 0003, or 0004: " << type << std::endl;
		exit(0);
	}

	// ********** Read data record **********
	std::size_t dataSize;
	total += base::readRecordHeader(file, "DATA", dataSize);
	std::cout << "Found record DATA: " << dataSize << " bytes\n";
	total += base::read(file, _numPortals);
	total += base::read(file, _numCells);
	std::cout << "Num portals: " << _numPortals << "\n"
		<< "Num cells: " << _numCells << "\n";

	// ********** Read portal list **********
	std::size_t prtsSize;
	total += base::readFormHeader(file, "PRTS", prtsSize);
	std::cout << "Found form PRTS: " << prtsSize << " bytes\n";
	_portalGeometry.resize(_numPortals);
	for (auto& pg : _portalGeometry) {
		total += pg.read(file);
	}

	// ********** Read Cell list **********
	std::size_t celsSize;
	total += base::readFormHeader(file, "CELS", celsSize);
	std::cout << "Found record CELS: " << celsSize << " bytes\n";
	_cells.resize(_numCells);
	for (auto& c : _cells) {
		total += c.read(file);
	}

	// Only for versions 2+
	if (1 < _version) {
		std::string form;
		base::peekHeader(file, form, size, type);
		std::cout << "Peek: " << form << "\n";
		if (("FORM" == form) && ("PGRF" == type)) {
			// Read Path Graph Factory record
			total += _pgrf.read(file);
		}
	}

	// Only for versions 1+
	if (0 < _version) {
		total += base::read(file, _crc);
		std::cout << "CRC: 0x" << std::hex << _crc << std::dec << "\n";
	}

	if (prtoSize == total) {
		std::cout << "Finished reading PRTO\n";
	}
	else
	{
		std::cout << "FAILED in reading PRTO\n"
			<< "Read " << total << " out of " << prtoSize << "\n";
		exit(0);
	}

	return total;
}

