/** -*-c++-*-
 *  \class  ptex
 *  \file   ptex.cpp
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
#include <swgLib/ptex.hpp>
#include <swgLib/base.hpp>

#include <iostream>

using namespace ml;

ptex::ptex()
{
}

ptex::~ptex()
{
}

std::size_t ptex::read(std::istream& file)
{
	std::size_t ptexSize;
	std::size_t total = base::readFormHeader(file, "PTEX", ptexSize);
	ptexSize += 8;
	std::cout << "Found PTEX: " << ptexSize - 12 << " bytes\n";

	std::string type;
	std::size_t size;
	total += base::readRecordHeader(file, type, size);
	_version = base::tagToVersion(type);
	if (_version > 0) {
		std::cout << "Expected type [0000]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Texture Version: " << _version << "\n";

	total += base::read(file, _shaderPath);
	total += base::read(file, _frameCount);
	total += base::read(file, _frameStart);
	total += base::read(file, _frameEnd);
	total += base::read(file, _frameUVSize);
	total += base::read(file, _framesPerColumn);
	total += base::read(file, _framesPerSecond);
	total += base::read(file, _textureVisible);

	if (ptexSize == total) {
		std::cout << "Finished reading PTEX\n";
	}
	else {
		std::cout << "FAILED in reading PTEX\n"
			<< "Read " << total << " out of " << ptexSize << "\n";
		exit(0);
	}

	return total;
}

const std::string& ptex::getShaderPath() const {
	return _shaderPath;
}

const int32_t& ptex::getFrameCount() const {
	return _frameCount;
}

const int32_t& ptex::getFrameStart() const {
	return _frameStart;
}

const int32_t& ptex::getFrameEnd() const {
	return _frameEnd;
}

const float& ptex::getFrameUVSize() const{
	return _frameUVSize;
}

const int32_t& ptex::getFramesPerColumn() const {
	return _framesPerColumn;
}

const float& ptex::getFramesPerSecond() const {
	return _framesPerSecond;
}

bool ptex::isTextureVisible() const {
	return _textureVisible;
}
