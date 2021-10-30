/** -*-c++-*-
 *  \class  wvfm
 *  \file   wvfm.cpp
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
#include <swgLib/wvfm.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

wvfm::wvfm() :
	_version(0),
	_interpolationType(0),
	_sampleType(0),
	_valueMin(0),
	_valueMax(0),
	_controlPointCount(0) {
}

wvfm::~wvfm()
{
}

std::size_t wvfm::read(std::istream& file)
{
	std::size_t wvfmSize;
	std::size_t total = base::readFormHeader(file, "WVFM", wvfmSize);
	wvfmSize += 8;
	std::cout << "Found WVFM: " << wvfmSize - 12 << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readRecordHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000..0002]: " << type << "\n";
		exit(0);
	}
	std::cout << "Waveform Version: " << (int)_version << "\n";

	total += base::read(file, _interpolationType);
	std::cout << "Interpolation type: " << _interpolationType << "\n";

	if (_version > 0) {
		total += base::read(file, _sampleType);
		std::cout << "Sample type: " << _sampleType << "\n";
	}

	if (_version > 1) {
		total += base::read(file, _valueMin);
		total += base::read(file, _valueMax);
		std::cout << "Value range: [" << _valueMin << "..." << _valueMax << "]\n";
	}

	total += base::read(file, _controlPointCount);
	std::cout << "Num control points: " << _controlPointCount << "\n";

	_controlPoint.resize(_controlPointCount);

	uint32_t cpNum = 0;
	for (auto& cp : _controlPoint) {
		total += base::read(file, cp._percent);
		total += base::read(file, cp._value);
		total += base::read(file, cp._randomMax);
		total += base::read(file, cp._randomMin);
		std::cout
			<< "Control point " << cpNum++ << ": \n"
			<< "       Percent: " << cp._percent << "\n"
			<< "         Value: " << cp._value << "\n"
			<< "    Random min: " << cp._randomMax << "\n"
			<< "    Random max: " << cp._randomMin << "\n";
	}

	if (wvfmSize == total) {
		std::cout << "Finished reading WVFM\n";
	}
	else {
		std::cout << "FAILED in reading WVFM\n"
			<< "Read " << total << " out of " << wvfmSize << "\n";
		exit(0);
	}

	return total;
}

void wvfm::scale(const float& percent) {
	for (auto& cp : _controlPoint) {
		cp._value *= percent;
		cp._randomMax *= percent;
		cp._randomMin *= percent;
	}
}
