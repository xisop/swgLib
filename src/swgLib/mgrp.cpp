/** -*-c++-*-
 *  \class  mgrp
 *  \file   mgrp.cpp
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

#include <swgLib/mgrp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

mgrp::mgrp() {
}

mgrp::~mgrp() {
}

std::size_t mgrp::read(std::istream& file) {
	std::size_t mgrpSize;
	std::size_t total = base::readFormHeader(file, "MGRP", mgrpSize);
	mgrpSize += 8;
	std::cout << "Found MGRP form: " << mgrpSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	uint32_t version = base::tagToVersion(type);
	if (0 != version) {
		std::cout << "Expected type [0000]: " << type << "\n";
		exit(0);
	}
	std::cout << "MultiFractal Group version: " << version << "\n";

	while (total < mgrpSize) {
		std::size_t mfamSize;
		total += base::readFormHeader(file, "MFAM", mfamSize);
		mfamSize += 8;
		total += base::readRecordHeader(file, "DATA", size);
		size += 8;

		family newFamily;
		total += base::read(file, newFamily.familyId);
		total += base::read(file, newFamily.name);
		std::cout << "Name: " << newFamily.name << "\n";
		total += newFamily.multifractal.read(file);

		_family.push_back(newFamily);
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

const std::vector<mgrp::family>& mgrp::getFamily() const { return _family; }
