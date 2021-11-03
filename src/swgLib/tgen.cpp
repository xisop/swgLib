/** -*-c++-*-
 *  \class  tgen
 *  \file   tgen.cpp
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

#include <swgLib/tgen.hpp>
#include <swgLib/base.hpp>

using namespace ml;

tgen::tgen() :
	_tgenVersion(0) {
}

tgen::~tgen() {
}

std::size_t tgen::read(std::istream& file) {
	std::size_t tgenSize;
	std::size_t total = base::readFormHeader(file, "TGEN", tgenSize);
	tgenSize += 8;
	std::cout << "Found TGEN form: " << tgenSize << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);
	_tgenVersion = base::tagToVersion(type);
	if (0 != _tgenVersion) {
		std::cout << "Expected type [0000]: " << type << "\n";
		exit(0);
	}
	std::cout << "Terrain Generator version: " << _tgenVersion << "\n";

	// Read Shader group
	total += _shaderGroup.read(file);

	// Read Flora group
	total += _floraGroup.read(file);

	// Read Radial group
	total += _radialGroup.read(file);

	// Read Environment group
	total += _environmentGroup.read(file);

	// Read Fractal group
	total += _fractalGroup.read(file);

	// Read Bitmap group
	//total += _bitmapGroup.read(file);

	// Read Layers
	std::string form;
	base::peekHeader(file, form, size, type);
	if ("LYRS" == type) {
		total += base::readFormHeader(file, "LYRS", size);
		while (total < tgenSize) {
			_layers.resize(_layers.size() + 1);

			layer& newLayer(_layers.back());
			total += newLayer.read(file);
		}
	}

	if (tgenSize == total) {
		std::cout << "Finished reading TGEN\n";
	}
	else {
		std::cout << "Failed in reading TGEN\n";
		std::cout << "Read " << total << " out of " << tgenSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Base Layer ******************************

std::size_t tgen::baseLayer::read(std::istream& file) {
	std::size_t ihdrSize;
	std::size_t total = base::readFormHeader(file, "IHDR", ihdrSize);
	ihdrSize += 8;

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	uint32_t version = base::tagToVersion(type);
	if (version > 1) {
		std::cout << "Expected type [0000...0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "Base Layer version: " << version << "\n";

	total += base::readRecordHeader(file, "DATA", size);

	int32_t temp;
	total += base::read(file, temp); _active = (0 != temp);
	total += base::read(file, _name);

	if (0 == version) {
		uint8_t unusedR, unusedG, unusedB;
		total += base::read(file, unusedR);
		total += base::read(file, unusedG);
		total += base::read(file, unusedB);
	}

	if (ihdrSize != total) {
		std::cout << "Failed in reading Base Layer\n";
		std::cout << "Read " << total << " out of " << ihdrSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Layer ******************************

std::size_t tgen::layer::read(std::istream& file) {
	std::size_t layrSize;
	std::size_t total = base::readFormHeader(file, "LAYR", layrSize);
	layrSize += 8;

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	_version = base::tagToVersion(type);

	switch (_version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	default:
		std::cout << "Unexpected LAYR version: " << _version << "\n";
		exit(0);
	}

	if (layrSize == total) {
		std::cout << "Finished reading LAYR\n";
	}
	else {
		std::cout << "Failed in reading LAYR\n";
		std::cout << "Read " << total << " out of " << layrSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readACTN(std::istream& file) {
	std::size_t actnSize;
	std::size_t total = base::readFormHeader(file, "ACTN", actnSize);

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);

	switch (version) {
	case 0: total += readACTNv0(file); break;
	case 1: total += readACTNv1(file); break;
	case 2: total += readACTNv2(file); break;
	default:
		std::cout << "Unexpected ACTN version: " << version << "\n";
		exit(0);
	}

	if (actnSize != total) {
		std::cout << "Failed in reading ACTN\n";
		std::cout << "Read " << total << " out of " << actnSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);

	total += baseLayer::read(file);

	while (total < size0000) {
		_sublayers.resize(_sublayers.size() + 1);
		total += _sublayers.back().readACTN(file);
	}

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);

	total += baseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);

	while (total < size0001) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}

	}

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);

	total += baseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);
	total += base::read(file, temp); _expanded = (0 != temp);

	while (total < size0002) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}
	}

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);

	total += baseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);
	total += base::read(file, temp); _expanded = (0 != temp);

	total += base::read(file, _notes);

	while (total < size0003) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}
	}

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readV4(std::istream& file) {
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);

	total += baseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);
	total += base::read(file, temp); // Unused
	total += base::read(file, temp); _expanded = (0 != temp);

	total += base::read(file, _notes);

	while (total < size0004) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}
	}

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readACTNv0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);

	total += baseLayer::read(file);

	while (total < size0000) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}
	}

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readACTNv1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);

	total += baseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);

	while (total < size0001) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}
	}

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgen::layer::readACTNv2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);

	total += baseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);

	while (total < size0002) {
		if (boundary::peekBoundary(file)) {
			// Read boundary
		}
		else if (filter::peekFilter(file)) {
			// Read filter
		}
		else if (affector::peekAffector(file)) {
			// Read Affector
		}
		else {
			std::cout << "Unsupported layer type\n";
			exit(0);
		}
	}

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Boundary ******************************

bool tgen::boundary::peekBoundary(std::istream& file) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	return (("BALL" == type) ||
		("BCIR" == type) ||
		("BREC" == type) ||
		("BPOL" == type) ||
		("BSPL" == type) ||
		("BPLN" == type));
}

std::size_t tgen::boundary::read(std::istream& file) {
	return 0;
}
// ****************************** Filter ******************************

bool tgen::filter::peekFilter(std::istream& file) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	return (("FHGT" == type) || // Filter Height
		("FFRA" == type) || // Filter Fractal
		("FBIT" == type) || // Filter Bitmap
		("FSLP" == type) || // Filter Slope
		("FDIR" == type) || // Filter Direction
		("FSHD" == type)); // Filter Shader
}

std::size_t tgen::filter::read(std::istream& file) {
	return 0;
}
// ****************************** Affector ******************************

bool tgen::affector::peekAffector(std::istream& file) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	return (("AHSM" == type) || // Affector Height Smoother (unused)
		("AHBM" == type) || // Affector Height Bitmap (unused)
		("ACBM" == type) || // Affector Color Bitmap (unused)
		("ASBM" == type) || // Affector Shader Bitmap (unused)
		("AFBM" == type) || // Affector Flora Bitmap (unused)
		("AENV" == type) || // Affector Environment
		("AHTR" == type) || // Affector Height Terrace
		("AHCN" == type) || // Affector Height Constant
		("AHFR" == type) || // Affector Height Fractal
		("ACCN" == type) || // Affector Color Constant
		("ACRH" == type) || // Affector Color Ramp Height
		("ACRF" == type) || // Affector Color Ramp Fractal
		("ASCN" == type) || // Affector Shader Constant
		("ASRP" == type) || // Affector Shader Replace
		("AFCN" == type) || // Affector Flora Static Collidable Constant
		("AFSC" == type) || // Affector Flora Static Collidable Constant
		("AFSN" == type) || // Affector Flora Static Non Collidable Constant
		("ARCN" == type) || // Affector Flora Dynamic Near Constant
		("AFDN" == type) || // Affector Flora Dynamic Near Constant
		("AFDF" == type) || // Affector Flora Dynamic Far Constant
		("ARIB" == type) || // Affector Ribbon
		("AEXC" == type) || // Affector Exclude
		("APAS" == type) || // Affector Passable
		("AROA" == type) || // Affector Road
		("ARIV" == type)); // Affector River
}

std::size_t tgen::affector::read(std::istream& file) {
	return 0;
}
