/** -*-c++-*-
 *  \class  ptqd
 *  \file   ptqd.cpp
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
#include <swgLib/ptqd.hpp>
#include <swgLib/base.hpp>

#include <iostream>

using namespace ml;

ptqd::ptqd()
{
}

ptqd::~ptqd()
{
}

std::size_t ptqd::read(std::istream& file)
{
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	if ("PTQD" == type) {
		std::size_t ptqdSize;
		std::size_t total = base::readFormHeader(file, "PTQD", ptqdSize);
		ptqdSize += 8;
		std::cout << "Found PTQD: " << ptqdSize - 12 << " bytes\n";

		total += ptcl::read(file);

		total += base::readFormHeader(file, type, size);
		_version = base::tagToVersion(type);
		if (_version > 3) {
			std::cout << "Expected type [0000..0003]: " << type << "\n";
			exit(0);
		}
		std::cout << "Particle Description Quad Version: " << _version << "\n";

		total += _rotation.read(file);
		total += _length.read(file);
		total += _width.read(file);
		total += _particleTexture.read(file);
		_particleTextureLoaded = true;
		_particleTextureFilename = "";


		if (_version > 0) {
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _aspectLocked);
		}

		if (ptqdSize == total) {
			std::cout << "Finished reading PTQD\n";
		}
		else {
			std::cout << "FAILED in reading PTQD\n"
				<< "Read " << total << " out of " << ptqdSize << "\n";
			exit(0);
		}

		return total;
	}
	else if ("PTCL" == type) {
		std::size_t ptqdSize;
		std::size_t total = base::readFormHeader(file, "PTCL", ptqdSize);
		ptqdSize += 8;
		std::cout << "Found PTCL Quad: " << ptqdSize - 12 << " bytes\n";

		total += base::readFormHeader(file, type, size);
		_version = base::tagToVersion(type);
		if (_version > 1) {
			std::cout << "Expected type [0000..0001]: " << type << "\n";
			exit(0);
		}
		std::cout << "Particle Description Quad(old) Version: " << _version << "\n";

		total += base::readRecordHeader(file, "0000", size);

		total += base::read(file, _name); // Name
		if (0 == _version) {
			total += base::read(file, _name); // Shader path overwrites name
		}
		total += base::read(file, _randomRotationDirection);

		std::cout << "Name: " << _name << "\n";
		std::cout << "Random rotation direction: " << _randomRotationDirection << "\n";

		total += _rotation.read(file);
		total += _length.read(file);
		total += _width.read(file);
		total += _colorRamp.read(file);
		total += _alpha.read(file);
		total += _speedScale.read(file);
		if (0 == _version) {
			_particleTextureLoaded = false;
			_particleTextureFilename = _name;
		}
		else {
			total += _particleTexture.read(file);
			_particleTextureLoaded = true;
			_particleTextureFilename = "";
		}

		if (ptqdSize == total) {
			std::cout << "Finished reading PTCL Quad\n";
		}
		else {
			std::cout << "FAILED in reading PTCL Quad\n"
				<< "Read " << total << " out of " << ptqdSize << "\n";
			exit(0);
		}

		return total;
	}
	else {
		std::cout << "Unhandled PTQD type: " << type << "\n";
		exit(0);
	}
	return 0;
}

const wvfm& ptqd::getRotation() const { return _rotation; }
const wvfm& ptqd::getLength() const { return _length; }
const wvfm& ptqd::getWidth() const { return _width; }
const bool& ptqd::particleTextureLoaded() const { return _particleTextureLoaded; }
const std::string& ptqd::getParticleTextureFilename() const { return _particleTextureFilename; }
const ptex& ptqd::getParticleTexture() const { return _particleTexture; }
const bool& ptqd::aspectLocked() const { return _aspectLocked; }
