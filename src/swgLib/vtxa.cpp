/** -*-c++-*-
 *  \class  vtxa
 *  \file   vtxa.cpp
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

#include <swgLib/vtxa.hpp>
#include <swgLib/vertex.hpp>
#include <swgLib/base.hpp>

#include <bitset>

using namespace ml;

vtxa::vtxa() {
}

vtxa::~vtxa() {
}

std::size_t vtxa::read(std::istream& file) {
	std::size_t vtxaSize;
	std::size_t total = base::readFormHeader(file, "VTXA", vtxaSize);
	vtxaSize += 8; //Don't count FORM/size fields in data read...

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	uint8_t version = base::tagToVersion(type);
	std::cout << "VTXA version: " << (int)version << "\n";

	total += base::readRecordHeader(file, "INFO", size);

	if (1 == version) {
		total += base::read(file, _numVertices);
		total += base::read(file, _numUVs);
	}

	total += base::read(file, _flags);
	uint32_t numTex;
	processD3DFVF(_flags, numTex);

	if (version > 1) {
		// Num texture sets comes from flags...
		total += base::read(file, _numVertices);
	}

	total += base::readRecordHeader(file, "DATA", size);
	std::cout << "Num vertices: " << _numVertices << "\n";
	std::cout << "Data size: " << size << "\n";
	_bytesPerVertex = uint32_t(size / _numVertices);
	std::cout << "Bytes per vertex: " << _bytesPerVertex << "\n";

	// Read all raw vertex data into an array...
	_vertexData.resize(size);
	file.read(_vertexData.data(), size);
	total += size;

	if (total == vtxaSize)
	{
		std::cout << "Finished reading VTXA.\n";
	}
	else
	{
		std::cout << "Error reading VTXA!\n";
		std::cout << "Read " << total << " out of " << vtxaSize << "\n";
		exit(0);
	}
	return total;
}

bool vtxa::hasPosition(uint32_t flags) {
	return (std::bitset<32>(flags).test(31));
}

bool vtxa::isTransformed(uint32_t flags) {
	return (std::bitset<32>(flags).test(30));
}

bool vtxa::hasNormal(uint32_t flags) {
	return (std::bitset<32>(flags).test(29));
}

bool vtxa::hasColor0(uint32_t flags) {
	return (std::bitset<32>(flags).test(28));
}

bool vtxa::hasColor1(uint32_t flags) {
	return (std::bitset<32>(flags).test(27));
}

bool vtxa::hasPointSize(uint32_t flags) {
	return (std::bitset<32>(flags).test(26));
}

// Some of this works...  Needs more investigating
void vtxa::processD3DFVF(uint32_t& codes, uint32_t& numTex)
{
#define VTXA_POSITION    0x00000001
#define VTXA_TRANSFORMED 0x00000002
#define VTXA_NORMAL      0x00000004
#define VTXA_COLOR0      0x00000008
#define VTXA_COLOR1      0x00000010

#define VTXA_POINTSIZE   0x00000020

#define VTXA_TEXCOUNTSHIFT 8
#define VTXA_TEXCOUNTMASK  0x0000000f

#define D3DFVF_RESERVED0           0x00000001 // 0000 0000 0000 0000 0000 0000 0000 0001
#define D3DFVF_POSITION_MASK       0x0000400E // 0000 0000 0000 0000 0100 0000 0000 1110
#define D3DFVF_XYZ                 0x00000002 // 0000 0000 0000 0000 0000 0000 0000 0010
#define D3DFVF_XYZRHW              0x00000004 // 0000 0000 0000 0000 0000 0000 0000 0100
#define D3DFVF_XYZB1               0x00000006 // 0000 0000 0000 0000 0000 0000 0000 0110
#define D3DFVF_XYZB2               0x00000008 // 0000 0000 0000 0000 0000 0000 0000 1000
#define D3DFVF_XYZB3               0x0000000a // 0000 0000 0000 0000 0000 0000 0000 1010
#define D3DFVF_XYZB4               0x0000000c // 0000 0000 0000 0000 0000 0000 0000 1100
#define D3DFVF_XYZB5               0x0000000e // 0000 0000 0000 0000 0000 0000 0000 1110
#define D3DFVF_XYZW                0x00004002 // 0000 0000 0000 0000 0100 0000 0000 0010
#define D3DFVF_NORMAL              0x00000010 // 0000 0000 0000 0000 0000 0000 0001 0000
#define D3DFVF_PSIZE               0x00000020 // 0000 0000 0000 0000 0000 0000 0010 0000
#define D3DFVF_DIFFUSE             0x00000040 // 0000 0000 0000 0000 0000 0000 0100 0000
#define D3DFVF_SPECULAR            0x00000080 // 0000 0000 0000 0000 0000 0000 1000 0000
#define D3DFVF_TEXCOUNT_MASK       0x00000f00 // 0000 0000 0000 0000 0000 1111 0000 0000
#define D3DFVF_TEXCOUNT_SHIFT               8
#define D3DFVF_TEX0                0x00000000 // 0000 0000 0000 0000 0000 0000 0000 0000
#define D3DFVF_TEX1                0x00000100 // 0000 0000 0000 0000 0000 0001 0000 0000
#define D3DFVF_TEX2                0x00000200 // 0000 0000 0000 0000 0000 0010 0000 0000
#define D3DFVF_TEX3                0x00000300 // 0000 0000 0000 0000 0000 0011 0000 0000
#define D3DFVF_TEX4                0x00000400 // 0000 0000 0000 0000 0000 0100 0000 0000
#define D3DFVF_TEX5                0x00000500 // 0000 0000 0000 0000 0000 0101 0000 0000
#define D3DFVF_TEX6                0x00000600 // 0000 0000 0000 0000 0000 0110 0000 0000
#define D3DFVF_TEX7                0x00000700 // 0000 0000 0000 0000 0000 0111 0000 0000
#define D3DFVF_TEX8                0x00000800 // 0000 0000 0000 0000 0000 1000 0000 0000
#define D3DFVF_LASTBETA_UBYTE4     0x00001000 // 0000 0000 0000 0000 0001 0000 0000 0000
#define D3DFVF_LASTBETA_D3DCOLOR   0x00008000 // 0000 0000 0000 0000 1000 0000 0000 0000
#define D3DFVF_RESERVED2           0x00006000 // 0000 0000 0000 0000 0110 0000 0000 0000

	std::bitset <32> bs(codes);

	std::cout << "D3D Flexible Vertex Format Bits: ";
	std::cout << bs << "\n";

	_hasPosition = bs.test(0);
	_hasTransformed = bs.test(0);
	_hasNormal = bs.test(2);
	_hasColor0 = bs.test(3);
	_hasColor1 = bs.test(4);

	numTex = (codes >> VTXA_TEXCOUNTSHIFT) & VTXA_TEXCOUNTMASK;
	for (unsigned int i = 0; i < 8; ++i) {
		_hasTexCoord[i] = (numTex > i);
	}

	std::cout << " -   Vertex has position: " << std::boolalpha << _hasPosition << "\n";
	std::cout << " -Vertex has transformed: " << std::boolalpha << _hasTransformed << "\n";
	std::cout << " -     Vertex has normal: " << std::boolalpha << _hasNormal << "\n";
	std::cout << " -    Vertex has color 0: " << std::boolalpha << _hasColor0 << "\n";
	std::cout << " -    Vertex has color 1: " << std::boolalpha << _hasColor1 << "\n";

	for (unsigned int i = 0; i < 8; ++i) {
		std::cout << " -Vertex has Tex Coord " << i << ": " << std::boolalpha << _hasTexCoord[i] << "\n";
	}

	std::cout << "Tex 0 dim: " << ((codes >> 12) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 1 dim: " << ((codes >> 14) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 2 dim: " << ((codes >> 16) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 3 dim: " << ((codes >> 18) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 4 dim: " << ((codes >> 20) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 5 dim: " << ((codes >> 22) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 6 dim: " << ((codes >> 24) & 0x00000003) + 1 << "\n";
	std::cout << "Tex 7 dim: " << ((codes >> 26) & 0x00000003) + 1 << "\n";

	switch (codes & D3DFVF_TEXCOUNT_MASK)
	{
	case D3DFVF_TEX0:
		std::cout << " -Vertex format includes no tex coords.\n";
		break;

	case D3DFVF_TEX1:
		std::cout << " -Vertex format includes 1 set of tex coords.\n";
		break;

	case D3DFVF_TEX2:
		std::cout << " -Vertex format includes 2 sets of tex coords.\n";
		break;

	case D3DFVF_TEX3:
		std::cout << " -Vertex format includes 3 sets of tex coords.\n";
		break;

	case D3DFVF_TEX4:
		std::cout << " -Vertex format includes 4 sets of tex coords.\n";
		break;

	case D3DFVF_TEX5:
		std::cout << " -Vertex format includes 5 sets of tex coords.\n";
		break;

	case D3DFVF_TEX6:
		std::cout << " -Vertex format includes 6 sets of tex coords.\n";
		break;

	case D3DFVF_TEX7:
		std::cout << " -Vertex format includes 7 sets of tex coords.\n";
		break;

	case D3DFVF_TEX8:
		std::cout << " -Vertex format includes 8 sets of tex coords.\n";
		break;
	}

	return;
}

bool vtxa::getPosition(const uint32_t& vertexNumber, float& x, float& y, float& z) const {
	const std::size_t offset( vertexNumber * _bytesPerVertex );
	vertex v(_bytesPerVertex, _vertexData.data() + offset);
	v.getPosition(x, y, z);

	return true;
}

bool vtxa::getNormal(const uint32_t& vertexNumber, float& nx, float& ny, float& nz) const {
	const std::size_t offset(vertexNumber * _bytesPerVertex);
	vertex v(_bytesPerVertex, _vertexData.data() + offset);
	v.getNormal(nx, ny, nz);

	return true;
}

bool vtxa::getColor0(const uint32_t& vertexNumber, color4& color) const {
	const std::size_t offset(vertexNumber * _bytesPerVertex);
	vertex v(_bytesPerVertex, _vertexData.data() + offset);
	/*
	v.getColor0(nx, ny, nz);
	*/
	// TODO...
	return false;
}

bool vtxa::getColor1(const uint32_t& vertexNumber, color4& color) const {
	const std::size_t offset(vertexNumber * _bytesPerVertex);
	vertex v(_bytesPerVertex, _vertexData.data() + offset);
	/*
	v.getColor0(nx, ny, nz);
	*/
	// TODO...
	return false;
}
