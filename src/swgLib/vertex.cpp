/** -*-c++-*-
 *  \class  vertex
 *  \file   vertex.cpp
 *  \author Ken Sewell

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

#include <swgLib/base.hpp>
#include <swgLib/vertex.hpp>
#include <memory.h>
#include <iostream>

using namespace ml;

vertex::vertex(uint32_t bpv, const char* dataPtr)
	:_bytesPerVertex(bpv),
	_dataPtr(dataPtr)
{
}

vertex::~vertex()
{
}

uint32_t vertex::getBytesPerVertex() const
{
	return _bytesPerVertex;
}

void vertex::print(std::ostream& os) const
{
	const float* x = reinterpret_cast<const float*>(_dataPtr);
	os << " ( " << x[0] << ", " << x[1] << ", " << x[2] << " ) "
		<< " ( " << x[3] << ", " << x[4] << ", " << x[5] << " ) ";

	switch (_bytesPerVertex)
	{
	case 32:
		os << " ( " << x[6] << ", " << x[7] << " ) "; // Tex coords 0
		break;

	case 36:
	{
		// ARGB Color
		const unsigned char* y =
			reinterpret_cast<const unsigned char*>(_dataPtr + (sizeof(float) * 6));
		os << " ( " << (unsigned int)(y[0]) << ", "
			<< (uint32_t)(y[1]) << " "
			<< (uint32_t)(y[2]) << " "
			<< (uint32_t)(y[3]) << ") ";
	}

	os << " ( " << x[7] << ", " << x[8] << " ) "; // Tex coords 0
	break;

	case 40:
		os << " ( " << x[6] << ", " << x[7] << " ) " // Tex coords 0
			<< " ( " << x[8] << ", " << x[9] << " ) ";// Tex coords 1
		break;

	case 44:
	{
		// ARGB Color
		const unsigned char* y =
			reinterpret_cast<const unsigned char*>(_dataPtr + (sizeof(float) * 6));
		os << " ( " << (uint32_t)(y[0]) << ", "
			<< (uint32_t)(y[1]) << " "
			<< (uint32_t)(y[2]) << " "
			<< (uint32_t)(y[3]) << ") ";
	}

	os << " ( " << x[7] << ", " << x[8] << " ) " // Tex coords 0
		<< " ( " << x[9] << ", " << x[10] << " ) "; // Tex coords 1
	break;

	case 48:
		os << " ( " << x[6] << ", " << x[7] << " ) " // Tex coords 0
			<< " ( " << x[8] << ", " << x[9] << ", " // Tex coords 1
			<< " ( " << x[10] << ", " << x[11] << " ) "; // Tex coords 2
		break;

	case 52:
	{
		// ARGB Color
		const unsigned char* y =
			reinterpret_cast<const unsigned char*>(_dataPtr + (sizeof(float) * 6));
		os << " ( " << (uint32_t)(y[0]) << ", "
			<< (uint32_t)(y[1]) << " "
			<< (uint32_t)(y[2]) << " "
			<< (uint32_t)(y[3]) << ") ";
	}

	os << " ( " << x[6] << ", " << x[7] << " ) " // Tex coords 0
		<< " ( " << x[8] << ", " << x[9] << ", " // Tex coords	1
		<< " ( " << x[10] << ", " << x[11] << " ) "; // Tex coords 2
	break;

	case 56:
		os << " ( " << x[6] << ", " << x[7] << " ) " // Tex coords 0
			<< " ( " << x[8] << ", " << x[9] << " ) " // Tex coords 1
			<< " ( " << x[10] << ", " << x[11] << " ) " // Tex coords 2
			<< " ( " << x[12] << ", " << x[13] << " ) "; // Tex coords 3
		break;

	case 60:
	{
		// ARGB Color
		const unsigned char* y =
			reinterpret_cast<const unsigned char*>(_dataPtr + (sizeof(float) * 6));
		os << " ( " << (uint32_t)(y[0]) << ", "
			<< (uint32_t)(y[1]) << " "
			<< (uint32_t)(y[2]) << " "
			<< (uint32_t)(y[3]) << ") ";
	}

	os << " ( " << x[7] << ", " << x[8] << " ) " // Tex coords 0
		<< " ( " << x[9] << ", " << x[10] << " ) " // Tex coords 1
		<< " ( " << x[11] << ", " << x[12] << " ) " // Tex coords 2
		<< " ( " << x[13] << ", " << x[14] << " ) "; // Tex coords 3

	break;

	case 64:
		os << " ( " << x[6] << ", " << x[7] << " ) " // Tex coords 0
			<< " ( " << x[8] << ", " << x[9] << " ) " // Tex coords 1
			<< " ( " << x[10] << ", " << x[11] << " ) " // Tex coords 2
			<< " ( " << x[12] << ", " << x[13] << " ) " // Tex coords 3
			<< " ( " << x[14] << ", " << x[15] << " ) "; // Tex coords 4
		break;

	case 68:
	{
		// ARGB Color
		const unsigned char* y =
			reinterpret_cast<const unsigned char*>(_dataPtr + (sizeof(float) * 6));
		os << " ( " << (uint32_t)(y[0]) << ", "
			<< (uint32_t)(y[1]) << " "
			<< (uint32_t)(y[2]) << " "
			<< (uint32_t)(y[3]) << ") ";
	}

	os << " ( " << x[6] << ", " << x[7] << " ) " // Tex coords 0
		<< " ( " << x[8] << ", " << x[9] << " ) " // Tex coords 1
		<< " ( " << x[10] << ", " << x[11] << " ) " // Tex coords 2
		<< " ( " << x[12] << ", " << x[13] << " ) " // Tex coords 3
		<< " ( " << x[14] << ", " << x[15] << " ) "; // Tex coords 4
	break;

	case 72:
		// Tex coords 0
		os << " ( " << x[6] << ", " << x[7] << " ) "
			// Tex coords 1
			<< " ( " << x[8] << ", " << x[9] << " ) "
			// Tex coords 2
			<< " ( " << x[10] << ", " << x[11] << " ) "
			// Tex coords 3
			<< " ( " << x[12] << ", " << x[13] << " ) "
			// Tex coords 4
			<< " ( " << x[14] << ", " << x[15] << " ) "
			// Tex coords 5
			<< " ( " << x[16] << ", " << x[17] << " ) ";
		break;
	};
}

void vertex::getPosition(float& x, float& y, float& z) const
{
	const float* v = reinterpret_cast<const float*>(_dataPtr);
	x = v[0];
	y = v[1];
	z = v[2];
}

void vertex::getNormal(float& nx, float& ny, float& nz) const
{
	const float* v = reinterpret_cast<const float*>(_dataPtr);
	nx = v[3];
	ny = v[4];
	nz = v[5];
}

void vertex::getTexCoords(unsigned int& numPairs, float* coords) const
{
	const float* v = reinterpret_cast<const float*>(_dataPtr);

	switch (_bytesPerVertex)
	{
	case 32:
		numPairs = 1;
		memcpy(coords, v + 6, sizeof(float) * 2 * numPairs);
		break;
	case 36:
		numPairs = 1;
		memcpy(coords, v + 7, sizeof(float) * 2 * numPairs);
		break;
	case 40:
		numPairs = 2;
		memcpy(coords, v + 6, sizeof(float) * 2 * numPairs);
		break;
	case 44:
		numPairs = 2;
		memcpy(coords, v + 7, sizeof(float) * 2 * numPairs);
		break;
	case 48:
		numPairs = 1;
		memcpy(coords, v + 6, sizeof(float) * 2 * numPairs);
		break;
	case 52:
		numPairs = 1;
		memcpy(coords, v + 7, sizeof(float) * 2 * numPairs);
		break;
	case 56:
		numPairs = 3;
		memcpy(coords, v + 6, sizeof(float) * 2 * numPairs);
		break;
	case 60:
		numPairs = 4;
		memcpy(coords, v + 7, sizeof(float) * 2 * numPairs);
		break;
	case 64:
		numPairs = 4;
		memcpy(coords, v + 6, sizeof(float) * 2 * numPairs);
		break;
	case 68:
		numPairs = 4;
		memcpy(coords, v + 7, sizeof(float) * 2 * numPairs);
		break;
	case 72:
		numPairs = 5;
		memcpy(coords, v + 6, sizeof(float) * 2 * numPairs);
		break;
	default:
		numPairs = 1;
		coords[0] = 0.0f;
		coords[1] = 0.0f;
	};
}

void vertex::getColor(unsigned char* argb) const
{
	if ((36 == _bytesPerVertex)
		|| (44 == _bytesPerVertex)
		|| (52 == _bytesPerVertex)
		|| (60 == _bytesPerVertex)
		|| (68 == _bytesPerVertex)
		)
	{
		memcpy(argb, _dataPtr + 24, sizeof(unsigned int));
	}
}

