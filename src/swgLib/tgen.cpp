/** -*-c++-*-
 *  \class  tgen
 *  \file   tgen.cpp
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

#include <swgLib/tgen.hpp>
#include <swgLib/base.hpp>

using namespace ml;

tgen::tgen() :
	_tgenVersion(0) {
}

tgen::~tgen() {
}

std::size_t tgen::read(std::istream& file) {
	std::size_t tgenSize;
	std::size_t total = base::readFormHeader(file, "TGEN", tgenSize);
	tgenSize += 8;
	std::cout << "Found TGEN form: " << tgenSize << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);
	_tgenVersion = base::tagToVersion(type);
	if (0 != _tgenVersion) {
		std::cout << "Expected type [0000]: " << type << "\n";
		exit(0);
	}
	std::cout << "Terrain Generator version: " << _tgenVersion << "\n";

	// Read Shader group
	std::string form;
	base::peekHeader(file, form, size, type);
	{
		std::string form, type;
		std::size_t size;
		base::peekHeader(file, form, size, type);
		std::cout << "Peek: " << form << ":" << type << "\n";
	}
	if ("SGRP" == type) { total += _shaderGroup.read(file); }

	// Read Flora group
	base::peekHeader(file, form, size, type);
	if ("FGRP" == type) { total += _floraGroup.read(file); }

	// Read Radial group
	base::peekHeader(file, form, size, type);
	if ("RGRP" == type) { total += _radialGroup.read(file); }

	// Read Environment group
	base::peekHeader(file, form, size, type);
	if ("EGRP" == type) { total += _environmentGroup.read(file); }

	// Read Fractal group
	base::peekHeader(file, form, size, type);
	if ("MGRP" == type) { total += _fractalGroup.read(file); }
	
	// Read Bitmap group
	//total += _bitmapGroup.read(file);

	// Read Layers
	base::peekHeader(file, form, size, type);
	if ("LYRS" == type) {
		std::size_t lyrsRead = 0;
		std::size_t lyrsSize;
		lyrsRead += base::readFormHeader(file, "LYRS", lyrsSize);
		std::cout << "lyrsSize: " << lyrsSize << "\n";
		lyrsSize += 8;
		while (lyrsRead < lyrsSize) {
			std::cout << "lyrsRead: " << lyrsRead << "\n";
			tgenLayer newLayer;
			lyrsRead += newLayer.read(file);
			_layers.push_back(newLayer);
			return 0;
		}
		total += lyrsRead;
	}

	if (tgenSize == total) {
		std::cout << "Finished reading TGEN\n";
	}
	else {
		std::cout << "Failed in reading TGEN\n";
		std::cout << "Read " << total << " out of " << tgenSize << "\n";
		exit(0);
	}

	return total;
}
