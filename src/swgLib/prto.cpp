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

bool prto::isRightType(std::istream& file)
{
	return isOfType(file, "PRTO");
}

const int32_t &prto::getNumCells() const
{
	return _numCells;
}

const int32_t &prto::getNumPortals() const
{
	return _numPortals;
}

std::size_t prto::readPRTO(std::istream& file, std::string path)
{
	basePath = path;
	std::size_t prtoSize;
	std::size_t total = base::readFormHeader(file, "PRTO", prtoSize);
	std::cout << "Found PRTO form" << ": " << prtoSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}

	_version = 0;
	if ("0000" == type) {
		_version = 1;
	}
	else if ("0001" == type) {
		_version = 1;
	}
	else if ("0002" == type) {
		_version = 2;
	}
	else if ("0003" == type) {
		_version = 3;
	}
	else if ("0004" == type) {
		_version = 4;
	}
	else {
		std::cout << "Expected FORM of type 0000, 0001, 0002, 0003, or 0004: " << type << std::endl;
		exit(0);
	}

	std::cout << "Found " << form << " " << type
		<< ": " << size << " bytes\n";


	// ********** Read data record **********
	std::size_t dataSize;
	total += base::readRecordHeader(file, "DATA", dataSize);
	std::cout << "Found record DATA: "
		<< dataSize << " bytes\n";

	total += base::read(file, _numPortals);
	total += base::read(file, _numCells);

	std::cout << "Num portals: " << _numPortals << "\n"
		<< "Num cells: " << _numCells << "\n";

	// ********** Read portal list **********
	std::size_t prtsSize;
	total += base::readFormHeader(file, "PRTS", prtsSize);
	std::cout << "Found form PRTS: "
		<< prtsSize << " bytes\n";

	_portalGeometry.resize(_numPortals);
	for (auto& pg : _portalGeometry) {
		total += pg.read(file);
		//		IndexedTriangleList* const portalGeometry = createPortalGeometry(vertexList); ???
	}

	// ********** Read Cell list **********
	std::size_t celsSize;
	total += base::readFormHeader(file, "CELS", celsSize);
	std::cout << "Found record CELS: "
		<< celsSize << " bytes\n";

	_cells.resize(_numCells);
	for (auto& c : _cells) {
		total += c.read(file);
	}

	// Only for versions 2+
	if (1 < _version) {
		peekHeader(file, form, size, type);
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

	if (prtoSize == total)
	{
		std::cout << "Finished reading PRTO" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PRTO" << std::endl;
		std::cout << "Read " << total << " out of " << prtoSize << "\n";
		exit(0);
	}

	return total;
}

