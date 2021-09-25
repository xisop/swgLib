/** -*-c++-*-
 *  \class  pffp
 *  \file   pffp.cpp
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

#include <swgLib/pffp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

pffp::pffp() {
}

pffp::~pffp() {
}

std::size_t pffp::read(std::istream& file) {
	std::size_t pffpSize;
	std::size_t total = base::readFormHeader(file, "PFFP", pffpSize);
	pffpSize += 8;
	std::cout << "Found FORM PFFP: " << pffpSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);

	_version = base::tagToVersion(type);
	if (_version > 2)
	{
		std::cout << "Expected FORM of type 0000 or 0001. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found FORM " << type << ": " << size - 4 << " bytes\n";

	total += base::read(file, _lighting);
	total += base::read(file, _lightingSpecularEnable);
	total += base::read(file, _lightingColorVertex);

	std::cout << "Lighting: " << std::boolalpha << _lighting << "\n";
	std::cout << "Lighting specular enable: " << std::boolalpha << _lightingSpecularEnable << "\n";
	std::cout << "Lighting color vertex: " << std::boolalpha << _lightingColorVertex << "\n";

	if (0 == _version) {
		total += base::read(file, _lightingMaterialTag);
		std::cout << "Lighting material tag: " << _lightingMaterialTag << "\n";
	}

	total += base::read(file, _lightingAmbientColorSource);
	total += base::read(file, _lightingDiffuseColorSource);
	total += base::read(file, _lightingSpecularColorSource);
	total += base::read(file, _lightingEmissiveColorSource);

	std::cout << "Lighting ambient color source: " << (int)_lightingAmbientColorSource << "\n";
	std::cout << "Lighting diffuse color source: " << (int)_lightingDiffuseColorSource << "\n";
	std::cout << "Lighting specular color source: " << (int)_lightingSpecularColorSource << "\n";
	std::cout << "Lighting emissive color source: " << (int)_lightingEmissiveColorSource << "\n";

	if (pffpSize == total)
	{
		std::cout << "Finished reading PFFP\n";
	}
	else
	{
		std::cout << "FAILED in reading PFFP\n";
		std::cout << "Read " << total << " out of " << pffpSize << "\n";
		exit(0);
	}

	return total;
}
