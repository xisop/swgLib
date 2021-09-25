/** -*-c++-*-
 *  \class  lod
 *  \file   lod.cpp
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

#include <swgLib/lod.hpp>
#include <swgLib/mesh.hpp>
#include <swgLib/cshd.hpp>
#include <swgLib/sht.hpp>
#include <swgLib/exbx.hpp>

#include <swgLib/model.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

lod::lod() :
	_usePivotPoint(false),
	_disableLODCrossFade(false)
{
}

lod::~lod()
{
}

bool lod::getChild(int32_t id, struct lodChild& requestedChild) const
{
	for (auto c : _child) {
		if (id == c.id) {
			requestedChild = c;
		}
	}

	return false;
}

std::size_t lod::readLOD(std::istream& file, std::string path)
{
	basePath = path;
	std::size_t dtlaSize;
	std::size_t total = readFormHeader(file, "DTLA", dtlaSize);
	dtlaSize += 12;
	std::cout << "Found DTLA form: " << dtlaSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected Form " << std::endl;
		exit(0);
	}

	_version = base::tagToVersion(type);
	if ((_version < 1) || (_version > 8)) {
		std::cout << "Expected type [0001..0008]: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found form of type: " << type << std::endl;

	// Versions 4+...
	if (_version >= 4) {
		// Read appearance...
		total += _appearance.read(file);
	}

	// Versions 6+...
	if (_version >= 6) {
		// Read PIVT...
		total += base::readRecordHeader(file, "PIVT", size);
		std::cout << "Found record PIVT: " << size << "\n";
		total += base::read(file, _lodFlags);
		std::cout << "LOD Flags: 0x" << std::hex << (int)_lodFlags << std::dec << "\n";
		_usePivotPoint = (_lodFlags & 0x01) > 0;
		std::cout << "Use pivot point: " << std::boolalpha << _usePivotPoint << "\n";
		if (_version >= 8) {
			_disableLODCrossFade = (_lodFlags & 0x02) > 0;
			std::cout << "Disable LOD cross fade: " << std::boolalpha << _disableLODCrossFade << "\n";
		}
	}

	// Load Entries...
	{
		// Read INFO...
		{
			std::size_t infoSize;
			total += base::readRecordHeader(file, "INFO", infoSize);
			std::cout << "Found record INFO: " << infoSize << "\n";
			uint32_t numEntries = uint32_t(infoSize / 12);
			std::cout << "Number of entries: " << numEntries << "\n";
			_child.resize(numEntries);
			for (auto& c : _child) {
				total += base::read(file, c.id);
				total += base::read(file, c.near);
				total += base::read(file, c.far);
			}
		}

		// Read DATA...
		{
			std::size_t dataSize;
			total += base::readFormHeader(file, "DATA", dataSize);
			std::cout << "Found form DATA: " << dataSize << "\n";
			std::size_t dataRead = 4; // Count 4 bytes from the form header...
			while (dataRead < dataSize) {
				dataRead += base::readRecordHeader(file, "CHLD", size);
				std::cout << "Found record CHLD: " << size << "\n";
				int32_t id;
				dataRead += base::read(file, id);
				std::string name;
				dataRead += base::read(file, name);

				// Find matching child id in vector...
				for (auto& c : _child) {
					if (id == c.id) {
						// If found, set child name...
						c.name = std::string("appearance/") + name;
					}
				}
			}
			total += dataRead;
		}

		for (auto c : _child) {
			std::cout
				<< "Child " << c.id << ":\n"
				<< "  Near distance: " << c.near << "\n"
				<< "   Far distance: " << c.far << "\n"
				<< "  Name distance: " << c.name << "\n";
		}
	} // End entries...

	// Versions 7+...
	if (_version >= 7) {
		// Load Radar...
		std::size_t radrSize;
		total += base::readFormHeader(file, "RADR", radrSize);
		std::cout << "Found form RADR: " << radrSize << "\n";

		total += base::readRecordHeader(file, "INFO", size);
		std::cout << "Found record INFO: " << size << "\n";

		int32_t hasRadar = 0;
		total += base::read(file, hasRadar);

		if (hasRadar > 0) {
			_hasRadar = true;
			std::cout << "Has radar:  true\n";
			total += _radarShape.read(file);
		}
		else {
			std::cout << "Has radar:  false\n";
		}
	}

	// Versions 2+...
	if (_version >= 2) {
		// Load test shape...
		std::size_t testSize;
		total += base::readFormHeader(file, "TEST", testSize);
		std::cout << "Found form TEST: " << testSize << "\n";

		total += base::readRecordHeader(file, "INFO", size);
		std::cout << "Found record INFO: " << size << "\n";

		int32_t hasTest = 0;
		total += base::read(file, hasTest);

		if (hasTest > 0) {
			_hasTest = true;
			std::cout << "Has test:  true\n";
			total += _testShape.read(file);
		}
		else {
			std::cout << "Has test:  false\n";
		}

		// Load write shape...
		std::size_t writSize;
		total += base::readFormHeader(file, "WRIT", writSize);
		std::cout << "Found form WRIT: " << writSize << "\n";

		total += base::readRecordHeader(file, "INFO", size);
		std::cout << "Found record INFO: " << size << "\n";

		int32_t hasWrite = 0;
		total += base::read(file, hasWrite);

		if (hasWrite > 0) {
			_hasWrite = true;
			std::cout << "Has write:  true\n";
			total += _writeShape.read(file);
		}
		else {
			std::cout << "Has write:  false\n";
		}

	}

	// Versions 3 or 4
	if ((3 == _version) || (5 == _version)) {
		// Load floors...
		std::cout << "Needs handled...\n";
		exit(0);
	}

	if (dtlaSize == total)
	{
		std::cout << "Finished reading DTLA\n";
	}
	else
	{
		std::cout << "Failed in reading DTLA\n";
		std::cout << "Read " << total << " out of " << dtlaSize << "\n";
		exit(0);
	}

	return total;
}

