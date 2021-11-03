/** -*-c++-*-
 *  \class  bgrp
 *  \file   bgrp.cpp
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

#include <swgLib/bgrp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

bgrp::bgrp() {
}

bgrp::~bgrp() {
}

std::size_t bgrp::read(std::istream& file) {
	std::size_t mgrpSize;
	std::size_t total = base::readFormHeader(file, "MGRP", mgrpSize);
	mgrpSize += 8;
	std::cout << "Found MGRP form: " << mgrpSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (0 != _version) {
		std::cout << "Expected type [0000]: " << type << "\n";
		exit(0);
	}
	std::cout << "Bitmap Group version: " << _version << "\n";

	while (total < mgrpSize) {
		total += base::readFormHeader(file, "MFAM", size);
		total += base::readRecordHeader(file, "DATA", size);

		// Increase family vector by one...
		_family.resize(_family.size() + 1);
		family& newFamily(_family.back());

		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		total += base::read(file, newFamily.bitmapName);
		std::cout << "Name: " << newFamily.name << "\n"
		          << "Bitmap name: " << newFamily.bitmapName << "\n";
	}

	if (mgrpSize == total) {
		std::cout << "Finished reading MGRP\n";
	}
	else {
		std::cout << "Failed in reading MGRP\n";
		std::cout << "Read " << total << " out of " << mgrpSize << "\n";
		exit(0);
	}

	return total;
}
