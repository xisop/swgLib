/** -*-c++-*-
 *  \class  mfrc
 *  \file   mfrc.cpp
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

#include <swgLib/mfrc.hpp>
#include <swgLib/base.hpp>

using namespace ml;

mfrc::mfrc() {
}

mfrc::~mfrc() {
}

std::size_t mfrc::read(std::istream& file) {
	{
		std::string form, type;
		std::size_t size;
		base::peekHeader(file, form, size, type);
		std::cout << "mfrc::read::peek: " << form << ":" << type << "\n";
	}

	std::size_t mfrcSize;
	std::size_t total = base::readFormHeader(file, "MFRC", mfrcSize);
	mfrcSize += 8;
	std::cout << "Found MFRC form: " << mfrcSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000...0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "MultiFractal version: " << _version << "\n";

	total += base::readRecordHeader(file, "DATA", size);

	int32_t temp;
	total += base::read(file, _seed);

	total += base::read(file, temp); _useBias = (0 != temp);
	total += base::read(file, _bias);

	total += base::read(file, temp); _useGain = (0 != temp);
	total += base::read(file, _gain);

	total += base::read(file, _numOctaves);
	total += base::read(file, _frequency);
	total += base::read(file, _amplitude);

	total += base::read(file, _scaleX);
	total += base::read(file, _scaleY);

	if (_version > 0) {
		total += base::read(file, _offsetX);
		total += base::read(file, _offsetY);
	}

	total += base::read(file, _combinationRule);

	if (mfrcSize == total) {
		std::cout << "Finished reading MFRC\n";
	}
	else {
		std::cout << "Failed in reading MFRC\n";
		std::cout << "Read " << total << " out of " << mfrcSize << "\n";
		exit(0);
	}

	return total;
}

void mfrc::setSeed(const uint32_t& seed) { _seed = seed; }
const uint32_t& mfrc::getSeed() const { return _seed; }

void mfrc::enableBias(bool enable) { _useBias = enable; }
bool mfrc::usesBias() const { return _useBias; }

void mfrc::setBias(const float& bias) { _bias = bias; }
const float& mfrc::getBias() const { return _bias; }

void mfrc::enableGain(bool enable) { _useGain = enable; }
bool mfrc::usesGain() const { return _useGain; }

void mfrc::setGain(const float& gain) { _gain = gain; }
const float& mfrc::getGain() const { return _gain; }

void mfrc::setNumOctaves(const int32_t& numOctaves) { _numOctaves = numOctaves; }
const int32_t& mfrc::getNumOctaves() const { return _numOctaves; }

void mfrc::setFrequency(const float& frequency) { _frequency = frequency; }
const float& mfrc::getFrequency() const { return _frequency; }

void mfrc::setAmplitude(const float& amplitude) { _amplitude = amplitude; }
const float& mfrc::getAmplitude() const { return _amplitude; }

void mfrc::setScale(const float& scaleX, const float& scaleY) {
	_scaleX = scaleX;
	_scaleY = scaleY;
}

void mfrc::setScaleX(const float& scaleX) { _scaleX = scaleX; }
const float& mfrc::getScaleX() const { return _scaleX; }

void mfrc::setScaleY(const float& scaleY) { _scaleY = scaleY; }
const float& mfrc::getScaleY() const { return _scaleY; }

void mfrc::setOffset(const float& offsetX, const float& offsetY) {
	_offsetX = offsetX;
	_offsetY = offsetY;
}

void mfrc::setOffsetX(const float& offsetX) { _offsetX = offsetX; }
const float& mfrc::getOffsetX() const { return _offsetX; }

void mfrc::setOffsetY(const float& offsetY) { _offsetY = offsetY; }
const float& mfrc::getOffsetY() const { return _offsetY; }

void mfrc::setCombinationRule(const int32_t& rule) { _combinationRule = rule; }
const int32_t& mfrc::getCombinationRule() const { return _combinationRule; }
