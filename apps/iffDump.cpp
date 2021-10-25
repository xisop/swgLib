/** -*-c++-*-
 *  \file   iffDump.cpp
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

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <cstdlib>
#include <cassert>

#include <swgLib/base.hpp>
#include <swgLib/model.hpp>

unsigned int numCols = 0;
unsigned int numRows = 0;

// Temp variable
unsigned int geodeCount = 0;

std::size_t readGeometryDATA(std::ifstream& file,
	std::size_t size,
	unsigned int numVerts
)
{
	uint32_t bytesPerVertex = uint32_t(size / numVerts);

	std::cout << std::fixed;
	std::cout << "Bytes per vertex: " << bytesPerVertex << std::endl;
	if (32 == bytesPerVertex)
	{
		uint32_t numData = uint32_t(size / 32);
		std::cout << "Num vertices: " << numData << std::endl;

		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numData; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			// Tex coords 0
			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (36 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			unsigned char color[4];
			file.read((char*)color, 4);
			std::cout << (unsigned int)color[0] << " "
				<< (unsigned int)color[1] << " "
				<< (unsigned int)color[2] << " "
				<< (unsigned int)color[3] << " ";


			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (40 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (bytesPerVertex == 44)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			unsigned char color[4];
			file.read((char*)color, 4);
			std::cout << (unsigned int)color[0] << " "
				<< (unsigned int)color[1] << " "
				<< (unsigned int)color[2] << " "
				<< (unsigned int)color[3] << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (48 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (52 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			unsigned char color[4];
			file.read((char*)color, 4);
			std::cout << (unsigned int)color[0] << " "
				<< (unsigned int)color[1] << " "
				<< (unsigned int)color[2] << " "
				<< (unsigned int)color[3] << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (56 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 3
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (60 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			unsigned char color[4];
			file.read((char*)color, 4);
			std::cout << (unsigned int)color[0] << " "
				<< (unsigned int)color[1] << " "
				<< (unsigned int)color[2] << " "
				<< (unsigned int)color[3] << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 3
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (64 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 3
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 4
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (68 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			unsigned char color[4];
			file.read((char*)color, 4);
			std::cout << (unsigned int)color[0] << " "
				<< (unsigned int)color[1] << " "
				<< (unsigned int)color[2] << " "
				<< (unsigned int)color[3] << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 3
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 4
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	if (72 == bytesPerVertex)
	{
		float x, y, z;
		float nx, ny, nz;
		float s, t;
		for (unsigned int i = 0; i < numVerts; ++i)
		{
			file.read((char*)&x, sizeof(x));
			file.read((char*)&y, sizeof(y));
			file.read((char*)&z, sizeof(z));
			std::cout << x << " " << y << " " << z << " ";

			file.read((char*)&nx, sizeof(nx));
			file.read((char*)&ny, sizeof(ny));
			file.read((char*)&nz, sizeof(nz));
			std::cout << nx << " " << ny << " " << nz << " ";

			// Tex coords 0
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 1
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 2
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 3
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << " ";

			// Tex coords 4
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;

			// Tex coords 4
			file.read((char*)&s, sizeof(s));
			std::cout << s << " ";
			file.read((char*)&t, sizeof(t));
			std::cout << t << std::endl;
		}
	}
	else
	{
		file.seekg(size, std::ios_base::cur);
	}

	return size;
}

int readGeometryINDX(std::ifstream& file, unsigned int size)
{
	unsigned int numIndex;
	file.read((char*)&numIndex, sizeof(numIndex));
	std::cout << "Num index: " << numIndex << std::endl;
	std::cout << "Num triangles: " << numIndex / 3 << std::endl;

	unsigned int bytesPerIndex = (size - 4) / numIndex;

	if (bytesPerIndex == 2)
	{
		unsigned short index;
		for (unsigned int i = 0; i < numIndex; ++i)
		{
			file.read((char*)&index, sizeof(index));
		}
	}
	else
	{
		unsigned int index;
		for (unsigned int i = 0; i < numIndex; ++i)
		{
			file.read((char*)&index, sizeof(index));
		}
	}

	return size;
}


std::size_t readRecord(
	std::ifstream& file,
	unsigned int level,
	std::deque<std::string> parentForms
)
{
	std::cout << std::endl;
	char buffer[10000];

	std::string group, type;
	std::size_t size = 0;
	ml::base::peekHeader(file, group, size, type);
	if (file.eof()) {
		exit(0);
	}

	std::cout << "***** LEVEL " << level << " *****\n";

	// ********** FORMS **********
	if ("FORM" == group)
	{
		std::cout << "Group: '" << group << "'\tsize: " << size << std::endl;

		if (type == "0000")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0001")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0002")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0003")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0004")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0005")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0006")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0007")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0008")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0009")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "0010")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << "\n";
		}
		else if (type == "EXBX")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Bounding Box)" << std::endl;
		}
		else if (type == "EXSP")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Bounding Sphere)" << std::endl;
		}
		else if (type == "HPNT")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Hardpoint)" << std::endl;
		}
		else if (type == "CPIT")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Cockpit)" << std::endl;
		}
		else if (type == "FCHS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Free chase camera)"
				<< std::endl;
		}
		else if (type == "TXMS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Texture maps)" << std::endl;
		}
		else if (type == "TXM ")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Texture map)" << std::endl;
		}
		else if (type == "CACH")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Cache)" << std::endl;
		}
		else if (type == "SD2D")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (2-D Sound)" << std::endl;
		}
		else if (type == "CMSH")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Collision mesh?)" << std::endl;
		}
		else if (type == "CELS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Container of CELLs)" << std::endl;
		}
		else if (type == "CELL")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Cell?)" << std::endl;
		}
		else if (type == "PRTO")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Top level .pob)" << std::endl;
		}
		else if (type == "PRTS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Portal list)" << std::endl;
		}
		/*
		else if (type == "PRTL")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (List)" << std::endl;
		}
		*/
		else if (type == "HPTS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (List of hardpoints)" << std::endl;
		}
		else if (type == "PGRF")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Portal something?)" << std::endl;
		}
		else if (type == "DTLA")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Top level for .lod files)" << std::endl;
		}
		else if (type == "MESH")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Top level for .msh files)" << std::endl;
		}
		else if (type == "VTXA")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Vertex array for mesh primitives)" << std::endl;
		}
		else if (type == "APPR")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Appearance, used in .lod files)" << std::endl;
		}
		else if (type == "SPS ")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shader Primitives Set)" << std::endl;
		}
		else if (type == "FLOR")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Floor, used in .lod files)" << std::endl;
		}
		else if (type == "RADR")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shape on RADAR, used in .lod files)" << std::endl;
		}
		else if (type == "DATA")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (?)" << std::endl;
		}
		else if (type == "TEST")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (?)" << std::endl;
		}
		else if (type == "WRIT")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (?)" << std::endl;
		}
		else if (type == "NULL")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (NULL)" << std::endl;
		}
		else if (type == "SSHT")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Static Shader)" << std::endl;
		}
		else if (type == "MATS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (List of materials)" << std::endl;
		}
		else if (type == "TCSS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Texture Coordinate Sets)" << std::endl;
		}
		else if (type == "TFNS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Texture Factors)" << std::endl;
		}
		else if (type == "ARVS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Alpha reference values)" << std::endl;
		}
		else if (type == "SRVS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Stencil reference values)" << std::endl;
		}
		else if (type == "EFCT")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shader effect)" << std::endl;
		}
		else if (type == "IMPL")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Effect implementation)" << std::endl;
		}
		else if (type == "PASS")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shader pass)" << std::endl;
		}
		else if (type == "PVSH")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Vertex shader)" << std::endl;
		}
		else if (type == "PPSH")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Pixel shader)" << std::endl;
		}
		else if (type == "PTXM")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Texture sampler)" << std::endl;
		}
		else if (type == "PFFP")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Fixed function pipeline)" << std::endl;
		}
		else if (type == "STAG")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shader stage)" << std::endl;
		}
		else if (type == "SHOT") // Shared Object Template
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shared Object Template)" << std::endl;
		}
		else if (type == "STOT") // Shared Tangible Object Template
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Shared Tangible Object Template)" << std::endl;
		}
		else if (type == "STAT") // Space Terrain Appearance Template
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Space Terrain Appearance Template)" << std::endl;
		}
		else if (type == "CLEA") // Clear color
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Clear Color)" << std::endl;
		}
		else if (type == "ENVI") // Environment texture
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Environment Texture)" << std::endl;
		}
		else if (type == "PARA") // Light parameters
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Light Parameters)" << std::endl;
		}
		else if (type == "AMBI") // Ambient light color
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Ambient Light Color)" << std::endl;
		}
		else if (type == "SKYB") // Skybox Texture
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Skybox Texture)" << std::endl;
		}
		else if (type == "STAR") // Star color/number
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Star information)" << std::endl;
		}
		else if (type == "DUST") // Dust information
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Dust information)" << std::endl;
		}
		else if (type == "DIST") // Distant appearance information
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Distant appearance information)" << std::endl;
		}
		else if (type == "PLAN") // Planet information?
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Planet information?)" << std::endl;
		}
		else if (type == "CELE") // Celestial information
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Celestial information)" << std::endl;
		}
		else if (type == "FOG ") // Fog information
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Fog information)" << std::endl;
		}
		else if (type == "IDTL")
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Type: " << type << " (Index Table?)" << std::endl;
			/*
			std::vector<ml::vector3> vertices;
			std::vector<unsigned int> indices;
			ml::model::readIDTL(file, vertices, indices);
			return size + 12;
			*/
		}
		else
		{
			ml::base::readFormHeader(file, group, size, type);
			std::cout << "Unknown Type: '" << type << "'" << std::endl;
			exit(0);
		}
		size -= 4;

		std::size_t sizeLeft = size;
		std::size_t sizeRead;
		parentForms.push_back(type);
		while (0 != sizeLeft)
		{
			sizeRead = readRecord(file, level + 1, parentForms);
			if (sizeRead > sizeLeft)
			{
				std::cout << "Error size mismatch!" << std::endl;
				exit(0);
			}
			sizeLeft -= sizeRead;
			std::cout << "Group: '" << group << "' Type: '" << type << "' sizeLeft: " << sizeLeft << std::endl;
		}
		std::cout << "Exit FORM " << type << "\n";
		// Size of data plus 12 bytes of FORM header
		return size + 12;
	}
	else {
		// ********** Records **********
		ml::base::readRecordHeader(file, group, size);
		std::cout << "Record: '" << group << "'\tsize: " << size << std::endl;

		if ("SPHR" == group)
		{
			assert(size == 16);
			float X, Y, Z, R;

			file.read((char*)&X, sizeof(X));
			file.read((char*)&Y, sizeof(Y));
			file.read((char*)&Z, sizeof(Z));
			file.read((char*)&R, sizeof(R));

			std::cout << "X: " << X << std::endl;
			std::cout << "Y: " << Y << std::endl;
			std::cout << "Z: " << Z << std::endl;
			std::cout << "R: " << R << std::endl;
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("BOX " == group)
		{
			assert(size == 24);
			float x1, x2, y1, y2, z1, z2;

			file.read((char*)&x1, sizeof(x1));
			file.read((char*)&y1, sizeof(y1));
			file.read((char*)&z1, sizeof(z1));
			file.read((char*)&x2, sizeof(x2));
			file.read((char*)&y2, sizeof(y2));
			file.read((char*)&z2, sizeof(z2));

			std::cout << "X1: " << x1 << std::endl;
			std::cout << "Y1: " << y1 << std::endl;
			std::cout << "Z1: " << z1 << std::endl;
			std::cout << "X2: " << x2 << std::endl;
			std::cout << "Y2: " << y2 << std::endl;
			std::cout << "Z2: " << z2 << std::endl;
			return size + 8; // Size of data plus 8 bytes of header
		}

		if ("PRTL" == group)
		{
			std::size_t total = 0;
			unsigned int numVerts;
			total += ml::base::read(file, numVerts);
			std::cout << "NumVerts: " << numVerts << std::endl;

			float x, y, z;
			for (unsigned int i = 0; i < numVerts; ++i)
			{
				total += ml::base::read(file, x);
				total += ml::base::read(file, y);
				total += ml::base::read(file, z);
				std::cout << "Vertex: " << x << ", "
					<< y << ", " << z
					<< std::endl;
			}

			ml::base::readUnknown(file, size - total);
			return size + 8; // Size of data plus 8 bytes of header
		}

		if ("DATA" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
/*
			for (unsigned int i = 0; i < parentForms.size(); ++i)
			{
				std::cout << parentForms[i] << "-";
			}
			std::cout << std::endl;

			if ("TXM " == parentForms[parentForms.size() - 2])
			{
				char tempType[5];
				tempType[4] = 0;
				file.read(tempType, 4);
				std::string textureType(tempType);

				//isMainTexture = false;
				if ("PUKL" == textureType)
				{
					std::cout << "Specular lookup texture ";
				}
				if ("EBUC" == textureType)
				{
					std::cout << "Cube map texture ";
				}
				if ("NIAM" == textureType)
				{
					std::cout << "Main texture ";
					//isMainTexture = true;
				}
				if ("LMRN" == textureType)
				{
					std::cout << "Normal map texture ";
				}
				else
				{
					std::cout << textureType << " ";
				}


				// Data is a filename
				unsigned char data;
				for (unsigned int i = 0; i < size - 4; ++i)
				{
					file.read((char*)&data, sizeof(data));
					std::cout << (unsigned int)data << " ";
				}
				std::cout << std::endl;
			}
			if ("0001" == parentForms.back())
			{
				ml::base::readUnknown(file, size);
				return size + 8; // Size of data plus 8 bytes of header
			}
			if ("0003" == parentForms.back())
			{
				std::size_t totalRead = 8;
				unsigned int x;
				totalRead += ml::base::read(file, x); std::cout << x << "\n";
				totalRead += ml::base::read(file, x); std::cout << x << "\n";
				//ml::base::readUnknown(file, size);
				return size + 8; // Size of data plus 8 bytes of header
			}
			if ("0004" == parentForms.back())
			{
				ml::base::readUnknown(file, size);
				return size + 8; // Size of data plus 8 bytes of header
			}
			if ("0005" == parentForms.back())
			{
				unsigned int x;
				ml::base::read(file, x); std::cout << x << "\n";

				unsigned char u1;
				ml::base::read(file, u1);
				std::cout << "?: " << (unsigned int)u1 << std::endl;

				std::string cellName;
				ml::base::read(file, cellName);
				std::cout << "Cell name: " << cellName << std::endl;

				std::string cellModel;
				ml::base::read(file, cellModel);
				std::cout << "Cell model: " << cellModel << std::endl;

				unsigned char hasFloor;
				ml::base::read(file, hasFloor);
				std::cout << "Has floor: " << (unsigned int)hasFloor << std::endl;

				if (hasFloor > 0)
				{
					std::string cellFloor;
					ml::base::read(file, cellFloor);
					std::cout << "Cell floor: " << cellFloor << std::endl;
				}

				return size + 8; // Size of data plus 8 bytes of header
			}
			if ("0008" == parentForms.back())
			{
				unsigned short data2;
				file.read((char*)&data2, sizeof(data2));
				std::cout << data2 << std::endl;
				file.read((char*)&data2, sizeof(data2));
				std::cout << data2 << std::endl;
				file.read((char*)&data2, sizeof(data2));
				std::cout << data2 << std::endl;
				file.read((char*)&data2, sizeof(data2));
				std::cout << data2 << std::endl;
				file.read((char*)&data2, sizeof(data2));
				std::cout << data2 << std::endl;
				file.read((char*)&data2, sizeof(data2));
				std::cout << data2 << std::endl;

				// Data is a filename
				unsigned char data;
				for (unsigned int i = 0; i < size - 12; ++i)
				{
					file.read((char*)&data, sizeof(data));
					if (i < 4)
					{
						std::cout << data << " ";
					}
					else
					{
						std::cout << (unsigned int)data << " ";
					}
				}
				std::cout << std::endl;
			}
			else
			{
				if (numVerts != 0)
				{
					readGeometryDATA(file, size, numVerts);
					numVerts = 0;
				}
				else
				{
					ml::base::readUnknown(file, size);
				}
			}
			return size + 8; // Size of data plus 8 bytes of header
			*/
		}

		if ("NAME" == group)
		{
			file.read(buffer, size);
			buffer[size] = 0;
			std::string name(buffer);
			std::cout << name << std::endl;
			return size + 8;
		}

		if ("INFO" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}

		if ("INDX" == group)
		{
			uint32_t numIndices;
			ml::base::read(file, numIndices);

			bool index16(((size - 4) / 2) == numIndices);

			const uint32_t numTriangles(numIndices / 3);
			if (index16) {
				// 16-bits per index...
				uint16_t t1, t2, t3;
				for (unsigned int i = 0; i < numTriangles; ++i) {
					ml::base::read(file, t1);
					ml::base::read(file, t2);
					ml::base::read(file, t3);
					std::cout << "Triangle " << i << ": "
						<< t1 << ", "
						<< t2 << ", "
						<< t3 << "\n";
				}
			}
			else {
				// 32-bits per index...
				int32_t t1, t2, t3;
				for (unsigned int i = 0; i < numTriangles; ++i) {
					ml::base::read(file, t1);
					ml::base::read(file, t2);
					ml::base::read(file, t3);
					std::cout << "Triangle " << i << ": "
						<< t1 << ", "
						<< t2 << ", "
						<< t3 << "\n";
				}
			}

			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("MATL" == group)
		{
			if (68 != size)
			{
				std::cout << "Expected MATL to be 68bytes!!!!!!!!!" << std::endl;
				exit(0);
			}

			// Data in abgr?
			float data;
			std::cout << "Ambient: ";
			for (unsigned int i = 0; i < 4; ++i)
			{
				file.read((char*)&data, sizeof(data));
				std::cout << (unsigned int)data << " ";
			}
			std::cout << std::endl;

			std::cout << "Diffuse: ";
			for (int i = 0; i < 4; ++i)
			{
				file.read((char*)&data, sizeof(data));
				std::cout << (unsigned int)data << " ";
			}
			std::cout << std::endl;

			std::cout << "Emissive: ";
			for (int i = 0; i < 4; ++i)
			{
				file.read((char*)&data, sizeof(data));
				std::cout << (unsigned int)data << " ";
			}
			std::cout << std::endl;

			std::cout << "Specular: ";
			for (int i = 0; i < 4; ++i)
			{
				file.read((char*)&data, sizeof(data));
				std::cout << (unsigned int)data << " ";
			}
			std::cout << std::endl;

			std::cout << "Power: ";
			file.read((char*)&data, sizeof(data));
			std::cout << (unsigned int)data << std::endl;

			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("0000" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("0001" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("0002" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("0003" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("0005" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SCAP" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("OPTN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("XFIN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("LINK" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("QCHN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SROT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CHNL" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("STRN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SFAM" == group)
		{
#if 0
			char temp[255];
			unsigned int num;
			file.read((char*)&num, sizeof(num));
			std::cout << "Num: " << num << std::endl;

			std::string name;
			file.getline(temp, 255, 0);
			name = temp;
			std::cout << "Name: " << name << std::endl;

			std::string name2;
			file.getline(temp, 255, 0);
			name2 = temp;
			std::cout << "Name2: " << name2 << std::endl;

			float f;
			unsigned int i;
			unsigned char c;

			file.read((char*)&c, sizeof(c));
			std::cout << (unsigned int)c << " ";

			file.read((char*)&c, sizeof(c));
			std::cout << (unsigned int)c << " ";

			file.read((char*)&c, sizeof(c));
			std::cout << (unsigned int)c << " ";

			file.read((char*)&f, sizeof(f));
			std::cout << f << " ";

			file.read((char*)&f, sizeof(f));
			std::cout << f << " ";

			file.read((char*)&i, sizeof(i));
			std::cout << i << " ";
			std::cout << std::endl;

			std::string name3;
			file.getline(temp, 255, 0);
			name3 = temp;
			std::cout << "Name3: " << name3 << std::endl;

			file.read((char*)&f, sizeof(f));
			std::cout << f << std::endl;
#else
			ml::base::readUnknown(file, size);
#endif
		}
		if ("FFAM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("RFAM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ADTA" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PARM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SGMT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("WMAP" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SMAP" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("XXXX" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PCNT" == group)
		{
			if (4 == size)
			{
				int data;
				file.read((char*)&data, sizeof(data));
				std::cout << "Sibling Nodes: " << data << std::endl;
			}
			else
			{
				ml::base::readUnknown(file, size);
			}
		}
		if ("NODE" == group)
		{
			std::string name;
			file.width(size - 52);
			file >> name;
			std::cout << name << std::endl;

			std::string r;
			file.width(4);
			file >> r;
			std::cout << r << std::endl;

			float data;
			std::cout << std::fixed;
			for (unsigned int i = 0; i < 12; ++i)
			{
				file.read((char*)&data, sizeof(data));
				std::cout << data << " ";
				if (((i + 1) % 4) == 0)
				{
					std::cout << std::endl;
				}
			}
			std::cout << std::endl;
		}
		if ("CSND" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CEFT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("MESH" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PALV" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("VERT" == group)
		{
			uint32_t numVerts = uint32_t(size / 12); // Number of x,y,z floats
			float x, y, z;
			for (unsigned int i = 0; i < numVerts; ++i) {
				ml::base::read(file, x);
				ml::base::read(file, y);
				ml::base::read(file, z);
				std::cout << "Vertex " << i << ": "
					<< x << ", "
					<< y << ", "
					<< z << "\n";
			}
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TRIS" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("NODS" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("BEDG" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PEDG" == group)
		{
			unsigned int count;
			ml::base::read(file, count);
			std::cout << "Entries: " << count << "\n";

			unsigned int x, y, z, w;
			for (unsigned int i = 0; i < count; ++i) {
				ml::base::read(file, x);
				ml::base::read(file, y);
				ml::base::read(file, z);
				ml::base::read(file, w);
				std::cout << i << ": "
					<< x << ", "
					<< y << ", "
					<< z << ", "
					<< w << "\n";
			}
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("META" == group)
		{
			unsigned int x;
			ml::base::read(file, x);
			std::cout << x << "\n";
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PNOD" == group)
		{
			unsigned int count;
			ml::base::read(file, count);
			std::cout << "Entries: " << count << "\n";

			int x, y, z, w;
			float a, b, c, d;
			for (unsigned int i = 0; i < count; ++i) {
				ml::base::read(file, x);
				ml::base::read(file, y);
				ml::base::read(file, z);
				ml::base::read(file, w);
				ml::base::read(file, a);
				ml::base::read(file, b);
				ml::base::read(file, c);
				ml::base::read(file, d);
				std::cout << i << ": "
					<< x << ", "
					<< y << ", "
					<< z << ", "
					<< w << ", "
					<< a << ", "
					<< b << ", "
					<< c << ", "
					<< d << "\n";
			}
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ECNT" == group)
		{
			unsigned int count;
			ml::base::read(file, count);
			std::cout << "Entries: " << count << "\n";

			unsigned int x;
			for (unsigned int i = 0; i < count; ++i) {
				ml::base::read(file, x);
				std::cout << i << ": " << x << "\n";
			}
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ESTR" == group)
		{
			unsigned int count;
			ml::base::read(file, count);
			std::cout << "Entries: " << count << "\n";

			unsigned int x;
			for (unsigned int i = 0; i < count; ++i) {
				ml::base::read(file, x);
				std::cout << i << ": " << x << "\n";
			}
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("HPNT" == group) // Lightsource?
		{
			float data1;
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << std::endl;
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << std::endl;
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << " ";
			file.read((char*)&data1, sizeof(data1));
			std::cout << data1 << std::endl;

			ml::base::readUnknown(file, size - 48);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SIDX" == group)
		{
#if 0
			unsigned int num;
			file.read((char*)&num, sizeof(num));
			std::cout << "Num: " << num << std::endl;

			for (unsigned int j = 0; j < num; ++j)
			{
				float x1;
				file.read((char*)&x1, sizeof(x1));
				std::cout << "Unknown: " << x1 << std::endl;
				file.read((char*)&x1, sizeof(x1));
				std::cout << "Unknown: " << x1 << std::endl;
				file.read((char*)&x1, sizeof(x1));
				std::cout << "Unknown: " << x1 << std::endl;
				unsigned int numIndex;
				file.read((char*)&numIndex, sizeof(numIndex));
				std::cout << "Num index: " << numIndex << std::endl;

				unsigned short data;
				for (unsigned int i = 0; i < numIndex; ++i)
				{
					file.read((char*)&data, sizeof(data));
					std::cout << data << " ";
				}
				std::cout << std::endl;
			}
#endif
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("MSGN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SKTI" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PIVT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		/*
		if ("CHLD" == group)
		{
#if 0
			unsigned int lodNum;
			file.read((char*)&lodNum, sizeof(lodNum));
			std::cout << "LOD" << lodNum << ": ";
			unsigned char data;
			for (int i = 0; i < size - 4; ++i)
			{
				file.read((char*)&data, sizeof(data));
				std::cout << data;
			}
			std::cout << std::endl;
#else
			ml::base::readUnknown(file, size);
#endif
		}
		*/
		if ("PAL " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ASND" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("HOBJ" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TEXT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TYPS" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("VOLT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PRVT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SKCT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PSND" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("FRAM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ZOOM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("FRST" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("3OFF" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("1OFF" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DEFH" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PUNF" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ACTN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PRNT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("RPRE" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("RPST" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("BPTR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("BPRO" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("JROR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("KEY " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DISP" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("POST" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("INT " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("FLOT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PROB" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SKIL" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ITEM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CODE" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("COLS" == group)
		{
			file.read((char*)&numCols, sizeof(numCols));

			std::cout << "Num columns: " << numCols << std::endl;

			char temp[255];
			for (unsigned int i = 0; i < numCols; ++i)
			{
				file.getline(temp, 255, 0);
				std::string name(temp);
				std::cout << i << ": " << name << std::endl;
			}
		}
		if ("TYPE" == group)
		{
			char temp[255];
			for (unsigned int i = 0; i < numCols; ++i)
			{
				file.getline(temp, 255, 0);
				std::string name(temp);
				std::cout << i << ": " << name << std::endl;
			}
		}
		if ("ROWS" == group)
		{
#if 0
			file.read((char*)&numRows, sizeof(numRows));

			std::cout << "Num rows: " << numRows << std::endl;

			char temp[255];
			for (unsigned int i = 0; i < numRows; ++i)
			{
				file.getline(temp, 255, 0);
				std::string name = temp;
				std::cout << i << ": " << name << std::endl;
			}
#else
			ml::base::readUnknown(file, size);
#endif
		}
		if ("PSTR" == group)
		{
			ml::base::readUnknown(file, size);
		}
		if ("OTNL" == group)
		{
			unsigned int num;
			file.read((char*)&num, sizeof(num));
			std::cout << num << std::endl;
			char temp[512];

			for (unsigned int i = 0; i < num; ++i)
			{
				file.getline(temp, 512, 0);
				std::cout << temp << std::endl;
			}
			//ml::base::readUnknown( file, size-4 );
		}
		if ("NSKY" == group) // Night sky?
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("STAR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SUN " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SSUN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("MOON" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SMOO" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DEST" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PCAM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("RTFC" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SSTC" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("STFP" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SST1" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SKTM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("XFNM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("POSN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TWHD" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TWDT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("NORM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DOT3" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("OZN " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("OZC " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("FOZC" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ZTO " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PIDX" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("NIDX" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TXCI" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TCSD" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("OITL" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("LATX" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CRCT" == group)
		{
			uint32_t numData = (uint32_t)(size / 4);
			for (unsigned int i = 0; i < numData; ++i)
			{
				unsigned int data;
				file.read((char*)&data, sizeof(data));
				std::cout << i << ": " << data << std::endl;
			}
		}
		if ("STRT" == group)
		{
			uint32_t numData = (uint32_t)(size / 4);
			for (unsigned int i = 0; i < numData; ++i)
			{
				unsigned int data;
				file.read((char*)&data, sizeof(data));
				std::cout << i << ": " << data << std::endl;
			}
		}
		if ("STNG" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TX1D" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("LOCT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("0004" == group)
		{
			std::size_t totalRead = 0;

			unsigned int x;
			unsigned short y;
			totalRead += ml::base::read(file, x); std::cout << x << "\n";
			totalRead += ml::base::read(file, y); std::cout << y << "\n";
			totalRead += ml::base::read(file, x); std::cout << x << "\n";

			std::string name;
			totalRead += ml::base::read(file, name);
			std::cout << "?: " << name << std::endl;

			std::cout << "Bytes left: " << size - totalRead << "\n";
			ml::base::readUnknown(file, size - totalRead);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("LGHT" == group)
		{
			std::size_t total = 0;
			unsigned int numLights;
			total = ml::base::read(file, numLights); std::cout << "Num Lights: " << numLights << "\n";

			unsigned char x;
			float z;
			for (unsigned i = 0; i < numLights; ++i) {
				std::cout << "Light " << i << "\n";

				total += ml::base::read(file, x);
				std::cout << (unsigned int)x << "\n";

				//
				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << std::endl;

				//
				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << std::endl;

				ml::matrix3x4 mat;
				total += ml::model::readTransformMatrix(file, mat);
				std::cout << "Matrix: \n" << mat << "\n";

				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << " ";

				total += ml::base::read(file, z);
				std::cout << z << std::endl;
				std::cout << "---\n";
			}
			ml::base::readUnknown(file, size - total);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CRC " == group)
		{
			unsigned int crc;
			ml::base::read(file, crc);
			std::cout << "Cyclic redundancy check: " << crc << "\n";
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CYLN" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ITL " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("INGR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("VAL " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PPTR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SKMG" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DFLT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PSRC" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PEXE" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PART" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("SHDR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("ENTR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("EXIT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DETH" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("INCP" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("WATR" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PSHM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PSTM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CNCF" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("MESG" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("DYN " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TRTS" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TRT " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("HPTS" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CHLD" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("CNT " == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("PCNT" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("NIAM" == group)
		{
			ml::base::readUnknown(file, size);
			return size + 8; // Size of data plus 8 bytes of header
		}
		if ("TAG " == group)
		{
			ml::tag tagName;
			ml::base::read(file, tagName);
			std::cout << tagName << "\n";
			return size + 8; // Size of data plus 8 bytes of header
		}
	}

	std::cout << "Unknown KEYWORD: " << group << std::endl;
	exit(0);
}


int main(int argc, char** argv)
{

	if (2 > argc)
	{
		std::cout << "iffDump <file>" << std::endl;
		return 0;
	}

	for (int i = 1; i < argc; ++i)
	{
		std::ifstream swgFile(argv[i], std::ios_base::binary);

		if (!swgFile.is_open())
		{
			std::cout << "Unable to open file: " << argv[1] << std::endl;
			exit(0);
		}

		std::deque<std::string> parentForms;
		readRecord(swgFile, 0, parentForms);
		swgFile.close();
	}

	return 0;
}
