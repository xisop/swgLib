/** -*-c++-*-
 *  \class  ptcl
 *  \file   ptcl.cpp
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
#include <swgLib/ptcl.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

ptcl::ptcl() :
	_version(0),
	_name(""),
	_randomRotationDirection(false),
	_numAttachments(0)
{
}

ptcl::~ptcl()
{
}

std::size_t ptcl::read(std::istream& file)
{
	std::size_t ptclSize;
	std::size_t total = base::readFormHeader(file, "PTCL", ptclSize);
	ptclSize += 8;
	std::cout << "Found PTCL: " << ptclSize - 12 << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 3) {
		std::cout << "Expected type [0000..0003]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Description Version: " << _version << "\n";

	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _name);
	total += base::read(file, _randomRotationDirection);
	if (1 == _version) {
		std::string unusedString;
		total += base::read(file, unusedString);
	}
	else if (_version > 1) {
		total += base::read(file, _numAttachments);
	}

	total += _colorRamp.read(file);
	total += _alpha.read(file);
	total += _speedScale.read(file);

	if (_version > 2) {
		total += _particleRelativeRotationX.read(file);
		total += _particleRelativeRotationY.read(file);
		total += _particleRelativeRotationZ.read(file);
		_particleUsesRelativeRotation = true;
		// TODO if constant value of 0 for all waveforms, then..
		//_particleUsesRelativeRotation = false;
	}
	else {
		_particleUsesRelativeRotation = false;
	}

	if (_version > 1) {
		_attachments.resize(_numAttachments);
		for (auto& a : _attachments) {
			a.read(file);
		}
	}


	if (ptclSize == total) {
		std::cout << "Finished reading PTCL\n";
	}
	else {
		std::cout << "FAILED in reading PTCL\n"
			<< "Read " << total << " out of " << ptclSize << "\n";
		exit(0);
	}

	return total;
}

const uint32_t& ptcl::getVersion() const { return _version; }
const std::string& ptcl::getName() const { return _name; }
const bool& ptcl::randomRotationDirection() const { return _randomRotationDirection; }
const int32_t& ptcl::getNumAttachments() const { return _numAttachments; }
const clrr& ptcl::getColorRamp() const { return _colorRamp; }
const wvfm& ptcl::getAlpha() const { return _alpha; }
const wvfm& ptcl::getSpeedScale() const { return _speedScale; }

const std::vector<patt>& ptcl::getAttachments() const { return _attachments; }

const wvfm& ptcl::getParticleRelativeRotationX() const { return _particleRelativeRotationX; }
const wvfm& ptcl::getParticleRelativeRotationY() const { return _particleRelativeRotationY; }
const wvfm& ptcl::getParticleRelativeRotationZ() const { return _particleRelativeRotationZ; }

const bool& ptcl::particleUsesRelativeRotation() const { return _particleUsesRelativeRotation; }
