/** -*-c++-*-
 *  \class  txm
 *  \file   txm.cpp
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

#include <swgLib/txm.hpp>
#include <swgLib/base.hpp>

using namespace ml;

txm::txm() :
	_uWrapMode(0),
	_vWrapMode(0),
	_wWrapMode(0),
	_mipFilterMode(0),
	_minFilterMode(0),
	_magFilterMode(0)
{
}

txm::~txm() {
}

std::size_t txm::read(std::istream& file) {
	// Read txm record
	std::size_t txmSize;
	std::size_t total = base::readFormHeader(file, "TXM ", txmSize);
	txmSize += 8; // Include Form/Size from header...
	std::cout << "Found record TXM : " << txmSize << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);

	if (_version > 2) {
		std::cout << "Expected version 0000, 0001, or 0002. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found FORM " << type << ": " << size << " bytes\n";

	total += base::readRecordHeader(file, "DATA", size);
	std::cout << "Found record DATA: " << size << " bytes\n";

	if (0 == _version) {
		total += base::read(file, _placeHolder);
		std::cout << "Placeholder: " << std::boolalpha << _placeHolder << "\n";
	}

	total += base::read(file, _nameTag);
	std::cout << "Name: " << _nameTag << "\n";

	if (_version != 0) {
		total += base::read(file, _placeHolder);
		std::cout << "Placeholder: " << std::boolalpha << _placeHolder << "\n";

		// Read U texture wrap mode...
		total += base::read(file, _uWrapMode);

		// Read V texture wrap mode...
		total += base::read(file, _vWrapMode);

		// Read W texture wrap mode...
		total += base::read(file, _wWrapMode);

		// Read map address Mip
		total += base::read(file, _mipFilterMode);

		// Read map address Min
		total += base::read(file, _minFilterMode);

		// Read map address Mag
		total += base::read(file, _magFilterMode);
	}

	std::cout << "U texture wrap mode: " << getWrapModeStr(_uWrapMode) << "\n";
	std::cout << "V texture wrap mode: " << getWrapModeStr(_vWrapMode) << "\n";
	std::cout << "W texture wrap mode: " << getWrapModeStr(_wWrapMode) << "\n";

	std::cout << "Mip texture filter mode: " << getFilterModeStr(_mipFilterMode) << "\n";
	std::cout << "Min texture filter mode: " << getFilterModeStr(_minFilterMode) << "\n";
	std::cout << "Mag texture filter mode: " << getFilterModeStr(_magFilterMode) << "\n";

	if (!_placeHolder) {
		total += base::readRecordHeader(file, "NAME", size);
		std::cout << "Found record NAME: " << size << " bytes\n";

		total += base::read(file, _textureName);
		std::cout << "Texture name: " << _textureName << "\n";
	}

	if (txmSize == total)
	{
		std::cout << "Finished reading txm\n";
	}
	else
	{
		std::cout << "FAILED in reading txm\n";
		std::cout << "Read " << total << " out of " << txmSize << "\n";
		exit(0);
	}

	return total;
}

const tag& txm::getNameTag() const {
	return _nameTag;
}

const std::string& txm::getTextureName() const {
	return _textureName;
}

std::string txm::getWrapModeStr(const int8_t mode) {
	switch (mode) {
	case Wrap: return std::string("Wrap"); break;
	case Mirror: return std::string("Mirror"); break;
	case Clamp: return std::string("Clamp"); break;
	case Border: return std::string("Border"); break;
	case MirrorOnce: return std::string("MirrorOnce"); break;
	}

	return std::string("Invalid");
}

std::string txm::getFilterModeStr(const int8_t mode) {
	switch (mode) {
	case None: return std::string("None"); break;
	case Point: return std::string("Point"); break;
	case Linear: return std::string("Linear"); break;
	case Anisotropic: return std::string("Anisotropic"); break;
	case FlatCubic: return std::string("FlatCubic"); break;
	case GaussianCubic: return std::string("GaussianCubic"); break;
	}

	return std::string("Invalid");
}

int8_t txm::getUWrapMode() const {
	return _uWrapMode;
}

int8_t txm::getVWrapMode() const {
	return _vWrapMode;
}

int8_t txm::getWWrapMode() const {
	return _wWrapMode;
}

int8_t txm::getMipFilterMode() const {
	return _mipFilterMode;
}

int8_t txm::getMinFilterMode() const {
	return _minFilterMode;
}

int8_t txm::getMagFilterMode() const {
	return _magFilterMode;
}
