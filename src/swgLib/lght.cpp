/** -*-c++-*-
 *  \class  lght
 *  \file   lght.cpp
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

#include <swgLib/lght.hpp>
#include <swgLib/base.hpp>

#include <iostream>

using namespace ml;

lght::lght() {
}

lght::~lght() {
}

std::size_t lght::read(std::istream& file) {
	// Light type
	std::size_t total = base::read(file, _type);
	switch (_type) {
	case 0: std::cout << "Light type: Ambient\n"; break;
	case 1: std::cout << "Light type: Parallel\n"; break;
	case 2: std::cout << "Light type: Point\n"; break;
	};

	// Diffuse color (argb)
	total += _diffuseColor.read32(file);
	std::cout << "Diffuse color (argb): " << _diffuseColor << "\n";

	// Specular color (argb)
	total += _specularColor.read32(file);
	std::cout << "Specular color (argb): " << _specularColor << "\n";

	// Transform
	total += _transform.read(file);
	std::cout << "Matrix:\n" << _transform << "\n";

	// Constant attenuation
	total += base::read(file, _constantAttenuation);
	std::cout << "Constant Attenuation: " << _constantAttenuation << "\n";

	// Linear attenuation
	total += base::read(file, _linearAttenuation);
	std::cout << "Linear Attenuation: " << _linearAttenuation << "\n";

	// Quadratic attenuation
	total += base::read(file, _quadraticAttenuation);
	std::cout << "Quadratic Attenuation: " << _quadraticAttenuation << "\n";

	return total;
}
