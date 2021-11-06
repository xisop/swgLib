/** -*-c++-*-
 *  \class  tgenBoundary
 *  \file   tgenBoundary.cpp
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

#include <swgLib/tgenBoundary.hpp>
#include <swgLib/base.hpp>

using namespace ml;

tgenBoundary::tgenBoundary() :
	tgenBaseLayer(),
	_featherFunction(0),
	_featherDistance(0.0f) {
}

tgenBoundary::~tgenBoundary() {
}

bool tgenBoundary::peekBoundary(std::istream& file) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	return (("BALL" == type) ||
		("BCIR" == type) ||
		("BREC" == type) ||
		("BPOL" == type) ||
		("BSPL" == type) ||
		("BPLN" == type));
}

std::size_t tgenBoundary::read(std::istream& file) {
	return 0;
}

std::size_t tgenBoundary::read(std::istream& file, tgenBoundaryPtr& boundaryPtr) {
	std::string form, type;
	std::size_t size;
	base::peekHeader(file, form, size, type);

	if ("FORM" == form) {
		if ("BCIR" == type) {
			boundaryPtr = boundaryCirclePtr(new ml::boundaryCircle);
		}
		else if ("BREC" == type) {
			boundaryPtr = boundaryRectanglePtr(new ml::boundaryRectangle);
		}
		else if ("BPOL" == type) {
			boundaryPtr = boundaryPolygonPtr(new ml::boundaryPolygon);
		}
		else if ("BPLN" == type) {
			boundaryPtr = boundaryPolylinePtr(new ml::boundaryPolyline);
		}
		else {
			std::cout << "Unknown boundary primitive: " << type << "\n";
			return 0;
		}

		if (boundaryPtr) {
			return boundaryPtr->read(file);
		}
	}
	else {
		std::cout << "Expected FORM instead of record: " << type << "\n";
		return 0;
	}

	return 0;
}

// ****************************** Circle ******************************

boundaryCircle::boundaryCircle() :
	tgenBoundary(),
	_centerNorth(0.0f),
	_centerEast(0.0f),
	_radius(1.0f)
{
}

boundaryCircle::~boundaryCircle() {
}

std::size_t boundaryCircle::read(std::istream& file) {
	std::size_t bcirSize;
	std::size_t total = base::readFormHeader(file, "BCIR", bcirSize);
	bcirSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 2) {
		std::cout << "Unexpected BCIR version: " << version << "\n";
	}
	std::cout << "BCIR version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (1 == version) {
		total += base::skip(file, sizeof(float));
	}

	total += base::read(file, _centerEast);
	total += base::read(file, _centerNorth);
	total += base::read(file, _radius);

	std::cout
		<< "Boundary Circle:\n"
		<< "       Center East: " << _centerEast << "\n"
		<< "      Center North: " << _centerNorth << "\n"
		<< "            Radius: " << _radius << "\n";

	if (2 == version) {
		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
		std::cout
			<< "  Feather function: " << _featherFunction << "\n"
			<< "  Feather distance: " << _featherDistance << "\n";
	}

	if (bcirSize != total) {
		std::cout << "Failed in reading BCIR\n";
		std::cout << "Read " << total << " out of " << bcirSize << "\n";
		exit(0);
	}

	return total;
}

const float& boundaryCircle::getCenterNorth() const { return _centerNorth; }
const float& boundaryCircle::getCenterEast() const { return _centerEast; }
const float& boundaryCircle::getRadius() const { return _radius; }

// ****************************** Rectangle ******************************

boundaryRectangle::boundaryRectangle() :
	tgenBoundary(),
	_west(0.0f),
	_south(0.0f),
	_east(0.0f),
	_north(0.0f)
{
}

boundaryRectangle::~boundaryRectangle() {
}

std::size_t boundaryRectangle::read(std::istream& file) {
	std::size_t brecSize;
	std::size_t total = base::readFormHeader(file, "BREC", brecSize);
	brecSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 4) {
		std::cout << "Unexpected BREC version: " << version << "\n";
	}
	std::cout << "BREC version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	if (1 == version) {
		total += base::skip(file, sizeof(float));
	}

	total += base::read(file, _west);
	total += base::read(file, _south);
	total += base::read(file, _east);
	total += base::read(file, _north);

	std::cout
		<< "Boundary Rectangle:\n"
		<< "                          West: " << _west << "\n"
		<< "                         South: " << _south << "\n"
		<< "                          East: " << _east << "\n"
		<< "                         North: " << _north << "\n";

	if (version > 1) {
		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
		std::cout
			<< "              Feather function: " << _featherFunction << "\n"
			<< "              Feather distance: " << _featherDistance << "\n";
	}

	if (version > 2) {
		int32_t temp;
		total += base::read(file, temp); _localWaterTable = (0 != temp);
		total += base::read(file, temp); _localGlobalWaterTable = (0 != temp);
		total += base::read(file, _localWaterTableHeight);
		total += base::read(file, _localWaterTableShaderSize);
		total += base::read(file, _localWaterTableShaderName);

		std::cout
			<< "             Local water table: " << std::boolalpha << _localWaterTable << "\n"
			<< "            Global water table: " << std::boolalpha << _localGlobalWaterTable << "\n"
			<< "      Local water table height: " << _localWaterTableHeight << "\n"
			<< " Local water table shader size: " << _localWaterTableShaderSize << "\n"
			<< " Local water table shader name: " << _localWaterTableShaderName << "\n";
	}

	if (version > 3) {
		total += base::read(file, _waterType);
		std::cout
			<< "                    Water type: " << _waterType << "\n";
	}

	if (brecSize != total) {
		std::cout << "Failed in reading BREC\n";
		std::cout << "Read " << total << " out of " << brecSize << "\n";
		exit(0);
	}

	return total;
}

const float& boundaryRectangle::getWest() const { return _west; }
const float& boundaryRectangle::getSouth() const { return _south; }
const float& boundaryRectangle::getEast() const { return _east; }
const float& boundaryRectangle::getNorth() const { return _north; }

bool boundaryRectangle::hasLocalWaterTable() const { return _localWaterTable; }
bool boundaryRectangle::hasLocalGlobalWaterTable() const { return _localGlobalWaterTable; }
const float& boundaryRectangle::getLocalWaterTableHeight() const { return _localWaterTableHeight; }
const float& boundaryRectangle::getLocalWaterTableShaderSize() const { return _localWaterTableShaderSize; }
const std::string& boundaryRectangle::getLocalWaterTableShaderName() const { return _localWaterTableShaderName; }
const int32_t& boundaryRectangle::getWaterType() const { return _waterType; }

// ****************************** Polygon ******************************

boundaryPolygon::boundaryPolygon() :
	tgenBoundary()
{
}

boundaryPolygon::~boundaryPolygon() {
}

std::size_t boundaryPolygon::read(std::istream& file) {
	std::size_t bpolSize;
	std::size_t total = base::readFormHeader(file, "BPOL", bpolSize);
	bpolSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 7) {
		std::cout << "Unexpected BPOL version: " << version << "\n";
	}
	std::cout << "BPOL version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	std::cout
		<< "Boundary Polygon:\n";

	if (version < 5) {
		if (1 == version) {
			total += base::skip(file, sizeof(float));
		}

		if (version > 1) {
			total += base::read(file, _featherFunction);
			total += base::read(file, _featherDistance);
			std::cout
				<< "              Feather function: " << _featherFunction << "\n"
				<< "              Feather distance: " << _featherDistance << "\n";
		}

		if (version > 2) {
			int32_t temp;
			total += base::read(file, temp); _localWaterTable = (0 != temp);
			total += base::read(file, _localWaterTableHeight);
			std::cout
				<< "             Local water table: " << std::boolalpha << _localWaterTable << "\n"
				<< "      Local water table height: " << _localWaterTableHeight << "\n";

			if (version > 3) {
				total += base::read(file, _localWaterTableShaderSize);
				std::cout
					<< " Local water table shader size: " << _localWaterTableShaderSize << "\n";
			}

			total += base::read(file, _localWaterTableShaderName);
			std::cout
				<< " Local water table shader name: " << _localWaterTableShaderName << "\n";
		}

		float x, y;
		while (total < bpolSize) {
			total += base::read(file, x);
			total += base::read(file, y);

			_points.push_back(std::make_pair(x, y));
		}
	}
	else // Version 5+
	{
		int32_t numPoints;
		total += base::read(file, numPoints);

		float x, y;
		for (int32_t i = 0; i < numPoints; ++i) {
			total += base::read(file, x);
			total += base::read(file, y);

			_points.push_back(std::make_pair(x, y));
		}

		int32_t temp;

		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
		total += base::read(file, temp); _localWaterTable = (0 != temp);
		total += base::read(file, _localWaterTableHeight);
		total += base::read(file, _localWaterTableShaderSize);
		if (version > 6) {
			total += base::read(file, _waterType);
		}
		if (version > 5) {
			total += base::skip(file, sizeof(int32_t));
		}
		total += base::read(file, _localWaterTableShaderName);

		std::cout
			<< "              Feather function: " << _featherFunction << "\n"
			<< "              Feather distance: " << _featherDistance << "\n"
			<< "             Local water table: " << std::boolalpha << _localWaterTable << "\n"
			<< "      Local water table height: " << _localWaterTableHeight << "\n"
			<< " Local water table shader size: " << _localWaterTableShaderSize << "\n"
			<< " Local water table shader name: " << _localWaterTableShaderName << "\n";
	}

	if (bpolSize != total) {
		std::cout << "Failed in reading BPOL\n";
		std::cout << "Read " << total << " out of " << bpolSize << "\n";
		exit(0);
	}

	return total;
}

bool boundaryPolygon::hasLocalWaterTable() const { return _localWaterTable; }
const float& boundaryPolygon::getLocalWaterTableHeight() const { return _localWaterTableHeight; }
const float& boundaryPolygon::getLocalWaterTableShaderSize() const { return _localWaterTableShaderSize; }
const std::string& boundaryPolygon::getLocalWaterTableShaderName() const { return _localWaterTableShaderName; }
const int32_t& boundaryPolygon::getWaterType() const { return _waterType; }

const std::vector<std::pair<float, float>>& boundaryPolygon::getPoints() const { return _points; }

// ****************************** Polyline ******************************

boundaryPolyline::boundaryPolyline() :
	tgenBoundary()
{
}

boundaryPolyline::~boundaryPolyline() {
}

std::size_t boundaryPolyline::read(std::istream& file) {
	std::size_t bplnSize;
	std::size_t total = base::readFormHeader(file, "BPLN", bplnSize);
	bplnSize += 8;

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint32_t version = base::tagToVersion(type);
	if (version > 2) {
		std::cout << "Unexpected BPLN version: " << version << "\n";
	}
	std::cout << "BPLN version: " << version << "\n";

	total += tgenBaseLayer::read(file);
	total += base::readRecordHeader(file, "DATA", size);

	std::cout
		<< "Boundary Polyline:\n";

	if (0 == version) {
		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
		total += base::read(file, _width);
		std::cout
			<< "              Feather function: " << _featherFunction << "\n"
			<< "              Feather distance: " << _featherDistance << "\n"
			<< "                         Width: " << _width << "\n";

		float x, y;
		while (total < bplnSize) {
			total += base::read(file, x);
			total += base::read(file, y);

			_points.push_back(std::make_pair(x, y));
		}
	}
	else {
		int32_t numPoints;
		total += base::read(file, numPoints);

		float x, y;
		for (int32_t i = 0; i < numPoints; ++i) {
			total += base::read(file, x);
			total += base::read(file, y);

			_points.push_back(std::make_pair(x, y));
		}

		if (2 == version) {
			int32_t numHeightPoints;
			total += base::read(file, numHeightPoints);
			total += base::skip(file, numHeightPoints * sizeof(float));
		}

		total += base::read(file, _featherFunction);
		total += base::read(file, _featherDistance);
		total += base::read(file, _width);
		if (2 == version) {
			total += base::skip(file, sizeof(int32_t));
		}

		std::cout
			<< "              Feather function: " << _featherFunction << "\n"
			<< "              Feather distance: " << _featherDistance << "\n"
			<< "                         Width: " << _width << "\n";
	}

	if (bplnSize != total) {
		std::cout << "Failed in reading BPLN\n";
		std::cout << "Read " << total << " out of " << bplnSize << "\n";
		exit(0);
	}

	return total;
}

const float& boundaryPolyline::getWidth() const { return _width; }

const std::vector<std::pair<float, float>>& boundaryPolyline::getPoints() const { return _points; }
