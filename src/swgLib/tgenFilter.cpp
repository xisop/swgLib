/** -*-c++-*-
 *  \class  tgenFilter
 *  \file   tgenFilter.cpp
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

#include <swgLib/tgenFilter.hpp>
#include <swgLib/base.hpp>

using namespace ml;

tgenFilter::tgenFilter() {
}

tgenFilter::~tgenFilter() {
}

bool tgenFilter::peekFilter(std::istream& file) {
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

std::size_t tgenFilter::read(std::istream& file) {
	return 0;
}

std::size_t tgenFilter::read(std::istream& file, tgenFilterPtr& filterPtr) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	if ("FORM" == form) {
		if ("FHGT" == type) {
			filterPtr = filterHeightPtr(new ml::filterHeight);
		}
		else if ("FFRA" == type) {
			filterPtr = filterFractalPtr(new ml::filterFractal);
		}
		else if ("FSLP" == type) {
			filterPtr = filterSlopePtr(new ml::filterSlope);
		}
		else if ("FDIR" == type) {
			filterPtr = filterDirectionPtr(new ml::filterDirection);
		}
		else if ("FSHD" == type) {
			filterPtr = filterShaderPtr(new ml::filterShader);
		}

		if (filterPtr) {
			return filterPtr->read(file);
		}
	}
	else {
		std::cout << "Expected FORM instead of record: " << type << "\n";
		return 0;
	}

	return 0;
}

// ****************************** Height ******************************

filterHeight::filterHeight() :
	tgenFilter() {
}

filterHeight::~filterHeight() {
}

std::size_t filterHeight::read(std::istream& file) {
	std::size_t fhgtSize;
	std::size_t total = base::readFormHeader(file, "FHGT", fhgtSize);
	fhgtSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 2) {
		std::cout << "Unexpected FHGT version: " << version << "\n";
		exit(0);
	}
	std::cout << "FHGT version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (1 == version) { total += base::skip(file, sizeof(float)); }
	total += base::read(file, _low);
	total += base::read(file, _high);

	std::cout
		<< "Filter Height:\n"
		<< "               Low: " << _low << "\n"
		<< "              High: " << _high << "\n";

	if (2 == version) {
		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
		std::cout
			<< "  Feather Function: " << _featherFunction << "\n"
			<< "  Feather Distance: " << _featherDistance << "\n";
	}

	if (fhgtSize != total) {
		std::cout << "Failed in reading FHGT\n";
		std::cout << "Read " << total << " out of " << fhgtSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Fractal ******************************

filterFractal::filterFractal() :
	tgenFilter() {
}

filterFractal::~filterFractal() {
}

std::size_t filterFractal::read(std::istream& file) {
	std::size_t ffraSize;
	std::size_t total = base::readFormHeader(file, "FFRA", ffraSize);
	ffraSize += 8;

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);
	if (version > 5) {
		std::cout << "Unexpected FFRA version: " << version << "\n";
		exit(0);
	}
	std::cout << "Filter Fractal version: " << version << "\n";

	switch (version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	case 5: total += readV5(file); break;
	}

	if (ffraSize != total) {
		std::cout << "Failed in reading FFRA\n";
		std::cout << "Read " << total << " out of " << ffraSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t filterFractal::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	int32_t fractalType;
	total += base::read(file, fractalType);
	_multifractal.setCombinationRule(fractalType);

	uint32_t seed;
	total += base::read(file, seed);
	_multifractal.setSeed(seed);

	float scaleX, scaleZ;
	total += base::read(file, scaleX);
	total += base::read(file, _scaleVertical);
	total += base::read(file, scaleZ);
	_multifractal.setScale(scaleX, scaleZ);

	total += base::read(file, _lowLimit);
	total += base::read(file, _highLimit);

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t filterFractal::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _lowLimit);
	total += base::read(file, _highLimit);

	int32_t fractalType;
	total += base::read(file, fractalType);
	_multifractal.setCombinationRule(fractalType);

	int32_t numOctaves;
	total += base::read(file, numOctaves);
	_multifractal.setNumOctaves(numOctaves);

	float frequency;
	total += base::read(file, frequency);
	_multifractal.setFrequency(frequency);

	int32_t count;
	float scaleX, scaleZ;
	total += base::read(file, count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::read(file, scaleX);
		total += base::read(file, scaleZ);
	}

	uint32_t seed;
	total += base::read(file, seed);
	_multifractal.setSeed(seed);

	for (int32_t i = 0; i < numOctaves; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::skip(file, sizeof(int32_t));
	}

	total += base::read(file, _scaleVertical);

	_multifractal.setScale(scaleX, scaleZ);

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t filterFractal::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	total += base::skip(file, sizeof(float));

	total += base::read(file, _lowLimit);
	total += base::read(file, _highLimit);

	int32_t fractalType;
	total += base::read(file, fractalType);
	_multifractal.setCombinationRule(fractalType);

	int32_t numOctaves;
	total += base::read(file, numOctaves);
	_multifractal.setNumOctaves(numOctaves);

	float frequency;
	total += base::read(file, frequency);
	_multifractal.setFrequency(frequency);

	int32_t count;
	float scaleX, scaleZ;
	total += base::read(file, count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::read(file, scaleX);
		total += base::read(file, scaleZ);
	}

	uint32_t seed;
	total += base::read(file, seed);
	_multifractal.setSeed(seed);

	for (int32_t i = 0; i < numOctaves; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::skip(file, sizeof(int32_t));
	}

	total += base::read(file, _scaleVertical);

	_multifractal.setScale(scaleX, scaleZ);

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t filterFractal::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _lowLimit);
	total += base::read(file, _highLimit);

	int32_t fractalType;
	total += base::read(file, fractalType);
	_multifractal.setCombinationRule(fractalType);

	int32_t numOctaves;
	total += base::read(file, numOctaves);
	_multifractal.setNumOctaves(numOctaves);

	float frequency;
	total += base::read(file, frequency);
	_multifractal.setFrequency(frequency);

	int32_t count;
	float scaleX, scaleZ;
	total += base::read(file, count);
	for (int32_t i = 0; i < count; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::read(file, scaleX);
		total += base::read(file, scaleZ);
	}

	uint32_t seed;
	total += base::read(file, seed);
	_multifractal.setSeed(seed);

	for (int32_t i = 0; i < numOctaves; ++i) {
		total += base::skip(file, sizeof(int32_t));
		total += base::skip(file, sizeof(int32_t));
	}

	total += base::read(file, _scaleVertical);

	_multifractal.setScale(scaleX, scaleZ);

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t filterFractal::readV4(std::istream& file) {
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += _multifractal.read(file);

	total += base::readRecordHeader(file, "PARM", size);

	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _lowLimit);
	total += base::read(file, _highLimit);

	total += base::read(file, _scaleVertical);

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t filterFractal::readV5(std::istream& file) {
	std::size_t size0005;
	std::size_t total = base::readFormHeader(file, "0005", size0005);
	size0005 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readFormHeader(file, "DATA", size);

	total += base::readRecordHeader(file, "PARM", size);

	total += base::read(file, _familyId);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	total += base::read(file, _lowLimit);
	total += base::read(file, _highLimit);

	total += base::read(file, _scaleVertical);

	if (size0005 != total) {
		std::cout << "Failed in reading 0005\n";
		std::cout << "Read " << total << " out of " << size0005 << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Slope ******************************

filterSlope::filterSlope() :
	tgenFilter() {
}

filterSlope::~filterSlope() {
}

std::size_t filterSlope::read(std::istream& file) {
	std::size_t fslpSize;
	std::size_t total = base::readFormHeader(file, "FSLP", fslpSize);
	fslpSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 2) {
		std::cout << "Unexpected FSLP version: " << version << "\n";
		exit(0);
	}
	std::cout << "FSLP version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (1 == version) { total += base::skip(file, sizeof(float)); }

	total += base::read(file, _minAngle);
	total += base::read(file, _maxAngle);

	if (2 == version) {
		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
	}

	if (fslpSize != total) {
		std::cout << "Failed in reading FSLP\n";
		std::cout << "Read " << total << " out of " << fslpSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Direction ******************************

filterDirection::filterDirection() :
	tgenFilter() {
}

filterDirection::~filterDirection() {
}

std::size_t filterDirection::read(std::istream& file) {
	std::size_t fdirSize;
	std::size_t total = base::readFormHeader(file, "FDIR", fdirSize);
	fdirSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected FDIR version: " << version << "\n";
		exit(0);
	}
	std::cout << "FDIR version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _minAngle);
	total += base::read(file, _maxAngle);
	total += base::read(file, _featherFunction);
	total += base::read(file, _featherDistance);

	if (fdirSize != total) {
		std::cout << "Failed in reading FDIR\n";
		std::cout << "Read " << total << " out of " << fdirSize << "\n";
		exit(0);
	}

	return total;
}

// ****************************** Shader ******************************

filterShader::filterShader() :
	tgenFilter() {
}

filterShader::~filterShader() {
}

std::size_t filterShader::read(std::istream& file) {
	std::size_t fshdSize;
	std::size_t total = base::readFormHeader(file, "FSHD", fshdSize);
	fshdSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 0) {
		std::cout << "Unexpected FSHD version: " << version << "\n";
		exit(0);
	}
	std::cout << "FSHD version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _familyId);

	if (fshdSize != total) {
		std::cout << "Failed in reading FSHD\n";
		std::cout << "Read " << total << " out of " << fshdSize << "\n";
		exit(0);
	}

	return total;
}

