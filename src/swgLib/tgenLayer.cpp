/** -*-c++-*-
 *  \class  tgenLayer
 *  \file   tgenLayer.cpp
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

#include <swgLib/tgenLayer.hpp>
#include <swgLib/base.hpp>

using namespace ml;

tgenLayer::tgenLayer() :
	_invertBoundaries(false),
	_invertFilters(false),
	_expanded(false),
	_notes("") {
}

tgenLayer::~tgenLayer() {

}

std::size_t tgenLayer::read(std::istream& file) {
	std::size_t layrSize;
	std::size_t total = base::readFormHeader(file, "LAYR", layrSize);
	layrSize += 8;

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);
	std::cout << "LAYR version: " << version << "\n";

	switch (version) {
	case 0: total += readV0(file); break;
	case 1: total += readV1(file); break;
	case 2: total += readV2(file); break;
	case 3: total += readV3(file); break;
	case 4: total += readV4(file); break;
	default:
		std::cout << "Unexpected LAYR version: " << version << "\n";
		exit(0);
	}

	if (layrSize == total) {
		std::cout << "Finished reading LAYR\n";
	}
	else {
		std::cout << "Failed in reading LAYR\n";
		std::cout << "Read " << total << " out of " << layrSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readACTN(std::istream& file) {
	std::size_t actnSize;
	std::size_t total = base::readFormHeader(file, "ACTN", actnSize);
	actnSize += 8;

	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	uint32_t version = base::tagToVersion(type);

	switch (version) {
	case 0: total += readACTNv0(file); break;
	case 1: total += readACTNv1(file); break;
	case 2: total += readACTNv2(file); break;
	default:
		std::cout << "Unexpected ACTN version: " << version << "\n";
		exit(0);
	}

	if (actnSize != total) {
		std::cout << "Failed in reading ACTN\n";
		std::cout << "Read " << total << " out of " << actnSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readV0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);

	while (total < size0000) {
		_sublayers.resize(_sublayers.size() + 1);
		total += _sublayers.back().readACTN(file);
	}

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readV1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);

	while (total < size0001) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}

	}

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readV2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);
	total += base::read(file, temp); _expanded = (0 != temp);

	while (total < size0002) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}
	}

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readV3(std::istream& file) {
	std::size_t size0003;
	std::size_t total = base::readFormHeader(file, "0003", size0003);
	size0003 += 8;

	total += tgenBaseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);
	total += base::read(file, temp); _expanded = (0 != temp);

	total += base::read(file, _notes);

	while (total < size0003) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}
	}

	if (size0003 != total) {
		std::cout << "Failed in reading 0003\n";
		std::cout << "Read " << total << " out of " << size0003 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readV4(std::istream& file) {
	std::size_t size0004;
	std::size_t total = base::readFormHeader(file, "0004", size0004);
	size0004 += 8;

	total += tgenBaseLayer::read(file);

	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);
	total += base::read(file, temp); // Unused
	total += base::read(file, temp); _expanded = (0 != temp);

	total += base::read(file, _notes);

	while (total < size0004) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}
	}

	if (size0004 != total) {
		std::cout << "Failed in reading 0004\n";
		std::cout << "Read " << total << " out of " << size0004 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readACTNv0(std::istream& file) {
	std::size_t size0000;
	std::size_t total = base::readFormHeader(file, "0000", size0000);
	size0000 += 8;

	total += tgenBaseLayer::read(file);

	while (total < size0000) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}
	}

	if (size0000 != total) {
		std::cout << "Failed in reading 0000\n";
		std::cout << "Read " << total << " out of " << size0000 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readACTNv1(std::istream& file) {
	std::size_t size0001;
	std::size_t total = base::readFormHeader(file, "0001", size0001);
	size0001 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);

	while (total < size0001) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}
	}

	if (size0001 != total) {
		std::cout << "Failed in reading 0001\n";
		std::cout << "Read " << total << " out of " << size0001 << "\n";
		exit(0);
	}

	return total;
}

std::size_t tgenLayer::readACTNv2(std::istream& file) {
	std::size_t size0002;
	std::size_t total = base::readFormHeader(file, "0002", size0002);
	size0002 += 8;

	total += tgenBaseLayer::read(file);
	std::size_t size;
	total += base::readRecordHeader(file, "ADTA", size);
	int32_t temp;
	total += base::read(file, temp); _invertBoundaries = (0 != temp);
	total += base::read(file, temp); _invertFilters = (0 != temp);

	while (total < size0002) {
		if (tgenBoundary::peekBoundary(file)) {
			// Read boundary
			tgenBoundaryPtr boundaryPtr;
			total += tgenBoundary::read(file, boundaryPtr);
			_boundaries.push_back(boundaryPtr);
		}
		else if (tgenFilter::peekFilter(file)) {
			// Read filter
			tgenFilterPtr filterPtr;
			total += tgenFilter::read(file, filterPtr);
			_filters.push_back(filterPtr);
		}
		else if (tgenAffector::peekAffector(file)) {
			// Read Affector
			tgenAffectorPtr affectorPtr;
			total += tgenAffector::read(file, affectorPtr);
			_affectors.push_back(affectorPtr);
		}
		else {
			std::string form, type;
			std::size_t size;
			base::peekHeader(file, form, size, type);
			if (type == "LAYR") {
				_sublayers.resize(_sublayers.size() + 1);
				total += _sublayers.back().read(file);
			}
			else {
				std::cout << "Unsupported layer type:" << type << "\n";
				exit(0);
			}
		}
	}

	if (size0002 != total) {
		std::cout << "Failed in reading 0002\n";
		std::cout << "Read " << total << " out of " << size0002 << "\n";
		exit(0);
	}

	return total;
}
