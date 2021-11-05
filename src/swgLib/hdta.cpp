/** -*-c++-*-
 *  \class  hdta
 *  \file   hdta.cpp
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

#include <swgLib/hdta.hpp>
#include <swgLib/base.hpp>

using namespace ml;

hdta::hdta() {
}

hdta::~hdta() {
}

std::size_t hdta::read(std::istream& file) {
	std::string hdtaForm;
	std::size_t hdtaSize;
	std::size_t total = base::readFormHeader(file, hdtaForm, hdtaSize);
	hdtaSize += 8;
	if (("HDTA" != hdtaForm) && ("ROAD" != hdtaForm)) {
		std::cout << "Expected HDTA/ROAD. Found form: " << hdtaForm << "\n";
		return 0;
	}
	std::cout << "Found HDTA/ROAD form: " << hdtaSize << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 1) {
		std::cout << "Expected type [0000..0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "Found form of type: " << type << "\n";

	if (0 == version) {
		while (total < hdtaSize) {
			_points.resize(_points.size() + 1);
			total += read(file, _points.back());
		}
	}
	else {
		while (total < hdtaSize) {
			_segments.resize(_segments.size() + 1);
			total += read(file, _segments.back());
		}
	}

	if (hdtaSize == total) {
		std::cout << "Finished reading HDTA/ROAD\n";
	}
	else {
		std::cout << "Failed in reading HDTA/ROAD\n";
		std::cout << "Read " << total << " out of " << hdtaSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t hdta::read(std::istream& file, point& newPoint) {
	std::size_t total = base::read(file, newPoint.x);
	total += base::read(file, newPoint.y);
	total += base::read(file, newPoint.z);
	return total;
}

std::size_t hdta::read(std::istream& file, segment& newSegment) {
	std::size_t sgmtSize;
	std::size_t total = base::readRecordHeader(file, "SGMT", sgmtSize);
	sgmtSize += 8;

	while (total < sgmtSize) {
		newSegment.points.resize(newSegment.points.size() + 1);
		total += read(file, newSegment.points.back());
	}

	if (total != sgmtSize) {
		std::cout << "Failed in reading SGMT\n";
		std::cout << "Read " << total << " out of " << sgmtSize << "\n";
		exit(0);
	}
	return total;
}
