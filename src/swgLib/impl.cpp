/** -*-c++-*-
 *  \class  impl
 *  \file   impl.cpp
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

#include <swgLib/impl.hpp>
#include <swgLib/base.hpp>

using namespace ml;

impl::impl() {
}

impl::~impl() {
}

std::size_t impl::read(std::istream& file) {
	std::size_t implSize;
	std::size_t total = base::readFormHeader(file, "IMPL", implSize);
	implSize += 8;
	std::cout << "Found FORM IMPL: " << implSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);

	if (_version > 9)
	{
		std::cout << "Expected FORM of type [0000..0009]. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found FORM " << type << ": " << size << " bytes\n";
	std::cout << "Implementation version: " << (int)_version << "\n";

	if (_version > 3) {
		// Read SCAP record
		total += base::readRecordHeader(file, "SCAP", size);
		std::cout << "Found record SCAP: " << size << "\n";

		uint32_t numCapabilities = uint32_t(size / 4);
		_shaderCapability.resize(numCapabilities);
		for (auto& sc : _shaderCapability) {
			total += base::read(file, sc);
			std::cout << "Shader capability: " << sc << "\n";
		}

		// Peek for optional OPTN record...
		std::string form;
		base::peekHeader(file, form, size, type);
		if ("OPTN" == form) {
			// Read OPTN record
			total += base::readRecordHeader(file, "OPTN", size);
			std::cout << "Found record OPTN: " << size << "\n";
			uint32_t numOptions = uint32_t(size / 4);
			_optionTag.resize(numOptions);
			for (auto& o : _optionTag) {
				total += base::read(file, o);
				std::cout << "Option tag: " << o << "\n";
			}
		}
	}

	// Read DATA record
	total += base::readRecordHeader(file, "DATA", size);
	std::cout << "Found record DATA: " << size << "\n";

	total += base::read(file, _numPasses);
	std::cout << "Number of passes: " << (int)_numPasses << "\n";

	// Version 1...
	if (1 == _version) {
		bool ignored;
		total += base::read(file, ignored);
	}

	// Versions 2+...
	if (_version > 1)
	{
		total += base::read(file, _phase);
		std::cout << "Implementation phase: " << _phase << "\n";
	}

	// Versions 5+
	if (_version > 4) {
		total += base::read(file, _castsShadow);
		total += base::read(file, _isCollidable);
		std::cout << "  Cast shadow: " << std::boolalpha << _castsShadow << "\n";
		std::cout << "Is collidable: " << std::boolalpha << _isCollidable << "\n";
	}

	if (8 == _version) {
		bool ignored;
		total += base::read(file, ignored);
	}

	_pass.resize(_numPasses);
	for (auto& p : _pass) {
		total += p.read(file);
	}

	if (implSize == total)
	{
		std::cout << "Finished reading IMPL\n";
	}
	else
	{
		std::cout << "FAILED in reading IMPL\n";
		std::cout << "Read " << total << " out of " << implSize << "\n";
		exit(0);
	}

	return total;
}
