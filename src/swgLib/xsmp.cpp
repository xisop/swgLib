/** -*-c++-*-
 *  \class  xsmp
 *  \file   xsmp.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2009-2021 Ken Sewell

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

#include <swgLib/xsmp.hpp>
#include <swgLib/base.hpp>

using namespace ml;

xsmp::xsmp() :
	_center(0.0f, 0.0f, 0.0f),
	_xAxis(1.0f, 0.0f, 0.0f),
	_yAxis(0.0f, 1.0f, 0.0f),
	_zAxis(0.0f, 0.0f, 1.0f),
	_xExtent(0.0f),
	_yExtent(0.0f),
	_zExtent(0.0f)
{
}

xsmp::~xsmp() {}

std::size_t xsmp::read(std::istream& file) {
	std::size_t xsmpSize;
	std::size_t total = base::readFormHeader(file, "XSMP", xsmpSize);
	xsmpSize += 8;
	std::cout << "Found XSMP form: " << xsmpSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);

	total += base::readFormHeader(file, "MSHP", size);

	total += base::read(file, _center);
	total += base::read(file, _xAxis);
	total += base::read(file, _yAxis);
	total += base::read(file, _zAxis);

	total += base::read(file, _xExtent);
	total += base::read(file, _yExtent);
	total += base::read(file, _zExtent);

	std::cout << "Center: " << _center << "\n"
		<< "X-Axis: " << _xAxis << "\n"
		<< "Y-Axis: " << _yAxis << "\n"
		<< "Z-Axis: " << _zAxis << "\n"
		<< "X Extent: " << _xExtent << "\n"
		<< "Y Extent: " << _yExtent << "\n"
		<< "Z Extent: " << _zExtent << "\n";

	if (total == xsmpSize)
	{
		std::cout << "Finished reading XSMP.\n";
	}
	else
	{
		std::cout << "Error reading XSMP!\n"
			<< "Read " << total << " out of " << xsmpSize << "\n";
		exit(0);
	}

	return total;
}
