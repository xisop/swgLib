/** -*-c++-*-
 *  \class  peft
 *  \file   peft.cpp
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
#include <swgLib/peft.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

peft::peft()
{
}

peft::~peft()
{
}

std::size_t peft::readPEFT(std::istream& file)
{
	std::size_t peftSize;
	std::size_t total = base::readFormHeader(file, "PEFT", peftSize);
	peftSize += 8;
	std::cout << "Found PEFT form: " << peftSize - 12 << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (_version > 2) {
		std::cout << "Expected type [0000..0002]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Effect Version: " << (int)_version << "\n";

	if (_version > 0) {
		total += _timing.read(file);
	}

	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _numEmitterGroups);

	if (_version > 1) {
		total += base::read(file, _initialPlaybackRate);
		total += base::read(file, _initialPlaybackRateTime);
		total += base::read(file, _playbackRate);
		total += base::read(file, _scale);
	}

	std::cout << "Num Emitter Groups: " << _numEmitterGroups << "\n";



	_emitterGroups.resize(_numEmitterGroups);
	for (auto& eg : _emitterGroups) {
		total += eg.read(file);
	}

	if (peftSize == total) {
		std::cout << "Finished reading PEFT\n";
	}
	else {
		std::cout << "FAILED in reading PEFT\n"
			<< "Read " << total << " out of " << peftSize << "\n";
		exit(0);
	}

	return total;
}

const uint32_t& peft::getVersion() const {
	return _version;
}

const ptim& peft::getTiming() const {
	return _timing;
}

const int32_t& peft::getNumEmitterGroups() const {
	return _numEmitterGroups;
}

const float& peft::getInitialPlaybackRate() const {
	return _initialPlaybackRate;
}

const float& peft::getInitialPlaybackRateTime() const{
	return _initialPlaybackRateTime;
}

const float& peft::getPlaybackRate() const {
	return _playbackRate;
}

const float& peft::getScale() const {
	return _scale;
}

const std::vector<emgp>& peft::getEmitterGroups() const {
	return _emitterGroups;
}
