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

#include <swgLib/base.hpp>
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

const ilf::node& ilf::getNode(const uint32_t& index) const {
	return _nodes.at(index);
}

std::size_t ilf::createILF(std::istream& infile, std::ofstream& outfile)
{
	std::size_t total = 0;
	char temp[512];

	// Write form with dummy size
	uint32_t form0Position = (uint32_t)outfile.tellp();
	base::writeFormHeader(outfile, 0, "INLY");
	// Write form with dummy size
	uint32_t form1Position = (uint32_t)outfile.tellp();
	base::writeFormHeader(outfile, 0, "0000");

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

		total += base::writeRecordHeader(outfile, "NODE", nodeSize);
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
	total += base::writeFormHeader(outfile, total + 4, "0000");

	// Rewrite form with proper size.
	outfile.seekp(form0Position, std::ios_base::beg);
	total += base::writeFormHeader(outfile, total + 4, "INLY");

	return total;
}

std::size_t ilf::readILF(std::istream& file)
{
	std::size_t ilfSize;
	std::size_t total = base::readFormHeader(file, "INLY", ilfSize);
	ilfSize += 8;
	std::cout << "Found INLY form: " << ilfSize << "\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);

	while (total < ilfSize)
	{
		total += readNODE(file);
	}

	if (ilfSize == total)
	{
		std::cout << "Finished reading ILF\n";
	}
	else
	{
		std::cout << "FAILED in reading ILF\n"
			<< "Read " << total << " out of " << ilfSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t ilf::readNODE(std::istream& file)
{
	std::size_t nodeSize;
	std::size_t total = base::readRecordHeader(file, "NODE", nodeSize);
	nodeSize += 8;

	node newNode;
	total += newNode.read(file);
	newNode.print(std::cout);
	_nodes.push_back(newNode);

	if (nodeSize == total)
	{
		std::cout << "Finished reading NODE\n";
	}
	else
	{
		std::cout << "FAILED in reading NODE\n"
			<< "Read " << total << " out of " << nodeSize << "\n";
		exit(0);
	}

	return total;
}

ilf::node::node() :
	_objectFilename(""),
	_cellName(""),
	_transform() {
}

ilf::node::~node() {
}

std::size_t ilf::node::read(std::istream& file) {
	std::size_t total = base::read(file, _objectFilename);
	total += base::read(file, _cellName);
	total += base::read(file, _transform);
	return total;
}

std::size_t ilf::node::write(std::ostream& file) const {
	std::size_t total = base::write(file, _objectFilename);
	total += base::write(file, _cellName);
	total += base::write(file, _transform);
	return total;
}

void ilf::node::print(std::ostream& os) const {
	os << "Object filename: " << _objectFilename << "\n"
		<< "Cell name: " << _cellName << "\n"
		<< "Transform:\n";
	_transform.print(os);
	os << "\n\n";
}
