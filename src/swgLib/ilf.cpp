/** -*-c++-*-
 *  \class  ilf
 *  \file   ilf.cpp
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

#include <swgLib/ilf.hpp>
#include <iostream>
#include <cstdlib>

using namespace ml;

ilf::ilf()
{
}

ilf::~ilf()
{
}

bool ilf::getNode(const unsigned int& index,
	std::string& fileName,
	std::string& zoneName,
	matrix3x4& transformMatrix,
	vector3& translateVector
) const
{
	if (index >= nodeFilename.size())
	{
		return false;
	}

	fileName = nodeFilename[index];
	zoneName = nodeZone[index];
	transformMatrix = nodeMatrix[index];
	translateVector = nodeVector[index];

	return true;
}


unsigned int ilf::createILF(std::istream& infile, std::ofstream& outfile)
{
	std::size_t total = 0;
	char temp[512];

	// Write form with dummy size
	unsigned int form0Position = outfile.tellp();
	writeFormHeader(outfile, 0, "INLY");
	// Write form with dummy size
	unsigned int form1Position = outfile.tellp();
	writeFormHeader(outfile, 0, "0000");

	while (!infile.eof())
	{
		unsigned int nodeSize = 0;

		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		std::string objectFilename;
		infile >> objectFilename;
		std::cout << objectFilename << std::endl;
		nodeSize += static_cast<unsigned int>(objectFilename.size() + 1);

		infile.getline(temp, 512, ':');
		std::string objectZone;
		infile >> objectZone;
		std::cout << objectZone << std::endl;
		nodeSize += static_cast<unsigned int>(objectZone.size() + 1);

		// 'Transform matrix:' line
		infile.getline(temp, 512, ':');

		std::cout.flags(std::ios_base::showpoint);
		std::cout << std::dec;
		std::cout.flags(std::ios_base::fixed);
		float x[12];
		for (unsigned int i = 0; i < 12; ++i)
		{
			std::cout.width(10);
			infile >> x[i];
			nodeSize += sizeof(float);
			std::cout << x[i] << " ";
		}
		std::cout << std::endl;

		// Blank line
		infile.getline(temp, 512);

		total += writeRecordHeader(outfile, "NODE", nodeSize);
		outfile.write(objectFilename.c_str(),
			static_cast<unsigned int>(objectFilename.size() + 1)
		);
		outfile.write(objectZone.c_str(),
			static_cast<unsigned int>(objectZone.size() + 1)
		);
		outfile.write((char*)x, sizeof(float) * 12);
		total += nodeSize;
	}

	// Rewrite form with proper size.
	outfile.seekp(form1Position, std::ios_base::beg);
	total += writeFormHeader(outfile, total + 4, "0000");

	// Rewrite form with proper size.
	outfile.seekp(form0Position, std::ios_base::beg);
	total += writeFormHeader(outfile, total + 4, "INLY");

	return total;
}

unsigned int ilf::readILF(std::istream& file)
{
	std::size_t ilfSize;
	std::size_t total = readFormHeader(file, "INLY", ilfSize);
	ilfSize += 8;
	std::cout << "Found INLY form"
		<< ": " << ilfSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string form, type;
	total += readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}
#ifdef DEBUG
	std::cout << "Found " << form << " " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;
#endif

	while (total < ilfSize)
	{
		total += readNODE(file);
	}

	if (ilfSize == total)
	{
#ifdef DEBUG
		sd::cout << "Finished reading ILF" << std::endl;
#endif
	}
	else
	{
		std::cout << "FAILED in reading ILF" << std::endl;
		std::cout << "Read " << total << " out of " << ilfSize
			<< std::endl;
	}

	return total;
}


unsigned int ilf::readNODE(std::istream& file)
{
	std::size_t nodeSize;
	std::string type;
	std::size_t total = readRecordHeader(file, type, nodeSize);
	nodeSize += 8;
	if (type != "NODE")
	{
		std::cout << "Expected record of type NODE: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found NODE record"
		<< ": " << nodeSize - 8 << " bytes"
		<< std::endl;

	std::string objectFilename;
	total += base::read(file, objectFilename);
	nodeFilename.push_back(objectFilename);

	std::string objectZone;
	total += base::read(file, objectZone);
	nodeZone.push_back(objectZone);

	std::cout << "Object Filename: " << objectFilename << std::endl;
	std::cout << "Object Zone: " << objectZone << std::endl;

	std::cout << "Transform matrix: " << std::endl;

	matrix3x4 mat;
	//vector3 vec;
	total += model::readTransformMatrix(file, mat);

	std::cout << "Matrix: \n" << mat << "\n";

	//std::cout << "Position: ";
	//vec.print();

	nodeMatrix.push_back(mat);
	//nodeVector.push_back( vec );


	while (total < nodeSize)
	{
		total += readNODE(file);
	}

	if (nodeSize == total)
	{
#ifdef DEBUG
		std::cout << "Finished reading NODE" << std::endl;
#endif
	}
	else
	{
		std::cout << "FAILED in reading NODE" << std::endl;
		std::cout << "Read " << total << " out of " << nodeSize
			<< std::endl;
	}

	return total;
}
