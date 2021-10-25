/** -*-c++-*-
 *  \class  ws
 *  \file   ws.cpp
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

#include <swgLib/ws.hpp>

#include <iostream>
#include <cstdlib>

#define RAD2DEG (180.0/3.141592653589793116)
#define DEG2RAD (3.141592653589793116/180.0)

using namespace ml;

ws::ws() :
	_version(0) {
}

ws::~ws() {
}

std::size_t ws::read(std::istream& file)
{
	std::size_t wsSize;
	std::size_t total = base::readFormHeader(file, "WSNP", wsSize);
	wsSize += 8;

	std::size_t size;
	total += base::readFormHeader(file, "0001", size);
	_version = 1;

	total += readNODS(file);
	total += readOTNL(file);

	for (auto& n : _nodes) {
		n.setObjectFilename( this->getName(n.getObjectNameIndex()));
		n.print(std::cout);
	}

	std::cout << "Num nodes: " << _nodes.size() << "\n";
	std::cout << "Num names: " << _names.size() << "\n";

	if (wsSize == total)
	{
#ifdef DEBUG
		sd::cout << "Finished reading WS" << std::endl;
#endif
	}
	else
	{
		std::cout << "FAILED in reading WS\n"
			<< "Read " << total << " out of " << wsSize << "\n";
		exit(0);
	}

	return total;
}

#if 0
std::size_t ws::create(std::ofstream& outfile)
{
	std::size_t total = 0;

	// Write form with dummy size
	const std::streampos form0Position = outfile.tellp();
	base::writeFormHeader(outfile, 0, "WSNP");
	// Write form with dummy size
	const std::streampos form1Position = outfile.tellp();
	base::writeFormHeader(outfile, 0, "0001");

	total += writeNODS(outfile);
	total += writeOTNL(outfile);

	// Rewrite form with proper size.
	outfile.seekp(form1Position, std::ios_base::beg);
	total += base::writeFormHeader(outfile, total + 4, "0001");

	// Rewrite form with proper size.
	outfile.seekp(form0Position, std::ios_base::beg);
	total += base::writeFormHeader(outfile, total + 4, "WSNP");

	return total;
}
#endif

#if 0
std::size_t ws::readMetaFile(std::istream& infile)
{
	char temp[512];
	maxObjectIndex = 0;

	while (!infile.eof())
	{
		ws::node node;

		// Node ID
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._nodeID;
		//std::cout << node.nodeID << std::endl;

		// Parent Node ID
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._parentNodeID;
		//std::cout << node.parentNodeID << std::endl;

		// Object Index
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._objectIndex;
		//std::cout << node.objectIndex << std::endl;

		// Object filename
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._objectFilename;
		//std::cout << node.objectFilename << std::endl;

		// Position in parent
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._cellIndex;
		//std::cout << node.positionInParent << std::endl;

		// qx
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._qx;
		//std::cout << node.rx << std::endl;

		// qy
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._qy;
		//std::cout << node.qy << std::endl;

		// qz
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._qz;
		//std::cout << node.qz << std::endl;

		// qw
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._qw;
		//std::cout << node.qw << std::endl;

		// x
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._x;
		//std::cout << node.x << std::endl;

		// y
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._y;
		//std::cout << node.y << std::endl;

		// z
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._z;
		//std::cout << node.z << std::endl;

		// Unknown 2
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._radius;
		//std::cout << node.u2 << std::endl;

		// Unknown 3
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._crc;
		//std::cout << node.crc << std::endl;

		// level
		infile.getline(temp, 512, ':');
		if (infile.eof()) { break; };
		infile >> node._level;
		//std::cout << node.level << std::endl;

		if (node._objectIndex > maxObjectIndex)
		{
			maxObjectIndex = node._objectIndex;
		}

		_nodes.push_back(node);
	}

	return 0;
}
#endif

#if 0
std::size_t ws::writeNODS(std::ofstream& outfile)
{
	std::size_t total = 0;

	//FORM NODS
	// Write form with dummy size
	std::size_t form0Position = outfile.tellp();
	base::writeFormHeader(outfile, 0, "NODS");

	// Start writing nodes from beginning of list
	_currentNode = _nodes.begin();
	while (_currentNode != _nodes.end())
	{
		total += writeNODE(outfile);
	}

	std::size_t nodeEndPosition = outfile.tellp();

	//FORM NODS
	outfile.seekp(form0Position, std::ios_base::beg);
	total += base::writeFormHeader(outfile, total + 4, "NODS");

	outfile.seekp(nodeEndPosition, std::ios_base::beg);

	return total;
}
#endif

std::size_t ws::readNODS(std::istream& file)
{
	std::size_t nodsSize;
	std::size_t total = base::readFormHeader(file, "NODS", nodsSize);
	nodsSize += 8;
#ifdef DEBUG
	std::cout << "Found NODS form"
		<< ": " << nodsSize - 12 << " bytes"
		<< std::endl;
#endif

	unsigned int numNodes = 0;
	_nodes.clear();
	while (total < nodsSize)
	{
		ws::node newNode;
		total += newNode.read(file);
		_nodes.push_back(newNode);
		++numNodes;
	}

#ifdef DEBUG
	std::cout << "Number of nodes found: " << numNodes << std::endl;
#endif

	if (nodsSize == total)
	{
#ifdef DEBUG
		std::cout << "Finished reading NODS" << std::endl;
#endif
	}
	else
	{
		std::cout << "FAILED in reading NODS\n"
			<< "Read " << total << " out of " << nodsSize << "\n";
		exit(0);
	}

	return total;
}

#if 0
std::size_t ws::writeNODE(std::ofstream& outfile)
{
	std::size_t total = 0;

	// FORM NODE
	// Write form with dummy size
	std::size_t form0Position = outfile.tellp();
	base::writeFormHeader(outfile, 0, "NODE");

	// FORM 0000
	// Write form with dummy size
	std::size_t form1Position = outfile.tellp();
	base::writeFormHeader(outfile, 0, "0000");

	total += base::writeRecordHeader(outfile, "DATA", 52);
	_currentNode->print(std::cout);
	total += _currentNode->write(outfile);
	unsigned int thisLevel = _currentNode->_level;
	++_currentNode;

	if (_currentNode != _nodes.end())
	{
		while (_currentNode->_level > thisLevel)
		{
			total += writeNODE(outfile);
		}
	}

	std::size_t nodeEndPosition = outfile.tellp();

	// FORM 0000
	// Rewrite form with proper size.
	outfile.seekp(form1Position, std::ios_base::beg);
	total += base::writeFormHeader(outfile, total + 4, "0000");

	// FORM NODE
	// Rewrite form with proper size.
	outfile.seekp(form0Position, std::ios_base::beg);
	total += base::writeFormHeader(outfile, total + 4, "NODE");

	outfile.seekp(nodeEndPosition, std::ios_base::beg);

	return total;
}

std::size_t ws::writeOTNL(std::ofstream& outfile)
{
	++maxObjectIndex;
	std::cout << "Resizing vector to " << maxObjectIndex << std::endl;
	_objectNames.resize(maxObjectIndex);
	for (_currentNode = _nodes.begin(); _currentNode != _nodes.end();
		++_currentNode)
	{
		_objectNames[_currentNode->_objectIndex] = _currentNode->_objectFilename;
	}

	std::size_t total = 0;

	// Write form with dummy size
	std::size_t position = outfile.tellp();
	base::writeRecordHeader(outfile, "OTNL", total);

	uint32_t numObjects = (uint32_t)_objectNames.size();
	outfile.write((char*)&numObjects, sizeof(numObjects));
	total += sizeof(numObjects);

	std::cout << "numObjects: " << numObjects << std::endl;
	for (unsigned int i = 0; i < numObjects; ++i)
	{
		outfile.write(
			_objectNames[i].c_str(),
			static_cast<unsigned int>(_objectNames[i].size() + 1)
		);
		total += _objectNames[i].size() + 1;
	}

	std::size_t nodeEndPosition = outfile.tellp();

	// Rewrite form with proper size.
	outfile.seekp(position, std::ios_base::beg);
	total += base::writeRecordHeader(outfile, "OTNL", total);

	outfile.seekp(nodeEndPosition, std::ios_base::beg);

	return total;
}
#endif

std::size_t ws::readOTNL(std::istream& file)
{
	std::size_t otnlSize;
	std::size_t total = base::readRecordHeader(file, "OTNL", otnlSize);
	otnlSize += 8;

	uint32_t numObjects;
	total += base::read(file, numObjects);
	_names.resize(numObjects);
	for (auto& name : _names) {
		total += base::read(file, name);
	}

	if (otnlSize == total)
	{
#ifdef DEBUG
		std::cout << "Finished reading OTNL" << std::endl;
#endif
	}
	else
	{
		std::cout << "FAILED in reading OTNL\n"
			<< "Read " << total << " out of " << otnlSize
			<< "\n";
		exit(0);
	}

	return total;
}

//*****************************************************************************

ws::node::node() {
}

ws::node::~node() {
}

std::size_t ws::node::read(std::istream& file)
{
	std::size_t nodeSize, size;
	std::size_t total = base::readFormHeader(file, "NODE", nodeSize);
	total += base::readFormHeader(file, "0000", size);
	total += base::readRecordHeader(file, "DATA", size);

	total += base::read(file, _nodeID);
	total += base::read(file, _parentNodeID);
	total += base::read(file, _objectNameIndex);
	total += base::read(file, _cellIndex);
	total += base::read(file, _qw);
	total += base::read(file, _qx);
	total += base::read(file, _qy);
	total += base::read(file, _qz);
	total += base::read(file, _positionX);
	total += base::read(file, _positionY);
	total += base::read(file, _positionZ);
	total += base::read(file, _radius);
	total += base::read(file, _crc);

	return total;
}

#if 0
std::size_t ws::node::write(std::ofstream& file)
{
	std::size_t total = 0;

	file.write((char*)&_nodeID, sizeof(_nodeID));
	total += sizeof(_nodeID);

	file.write((char*)&_parentNodeID, sizeof(_parentNodeID));
	total += sizeof(_parentNodeID);

	file.write((char*)&_objectIndex, sizeof(_objectIndex));
	total += sizeof(_objectIndex);

	file.write((char*)&_cellIndex, sizeof(_cellIndex));
	total += sizeof(_cellIndex);

	file.write((char*)&_qx, sizeof(_qx));
	total += sizeof(_qx);

	file.write((char*)&_qy, sizeof(_qy));
	total += sizeof(_qy);

	file.write((char*)&_qz, sizeof(_qz));
	total += sizeof(_qz);

	file.write((char*)&_qw, sizeof(_qw));
	total += sizeof(_qw);

	file.write((char*)&_x, sizeof(_x));
	total += sizeof(_x);

	file.write((char*)&_y, sizeof(_y));
	total += sizeof(_y);

	file.write((char*)&_z, sizeof(_z));
	total += sizeof(_z);

	file.write((char*)&_radius, sizeof(_radius));
	total += sizeof(_radius);

	file.write((char*)&_crc, sizeof(_crc));
	total += sizeof(_crc);

	return total;
}
#endif

void ws::node::print(std::ostream& os) const
{
	std::cout << "Node ID: " << _nodeID << "\n"
		"Parent node ID: " << _parentNodeID << "\n"
		"Object name index: " << _objectNameIndex << "\n"
		"Object filename: " << _objectFilename << "\n"
		"Position in parent: " << _cellIndex << "\n"
		"Rotation Quaternion W: " << _qw << "\n"
		"Rotation Quaternion X: " << _qx << "\n"
		"Rotation Quaternion Y: " << _qy << "\n"
		"Rotation Quaternion Z: " << _qz << "\n"
		"Position X: " << _positionX << "\n"
		"Position Y: " << _positionY << "\n"
		"Position Z: " << _positionZ << "\n"
		"Radius: " << _radius << "\n"
		"CRC: 0x" << std::hex << _crc << std::dec << "\n"
		"\n";
}

uint32_t ws::getNumNodes() const
{
	return uint32_t(_nodes.size());
}

const ws::node& ws::getNode(const uint32_t& i) const
{
	return _nodes.at(i);
}

const std::string& ws::getName(const uint32_t& i) const {
	return _names.at(i);
}

std::string ws::node::getObjectFilename() const
{
	return _objectFilename;
}

void ws::node::setObjectFilename(const std::string& name) {
	_objectFilename = name;
}

const float& ws::node::getPositionX() const { return _positionX; }
const float& ws::node::getPositionY() const { return _positionY; }
const float& ws::node::getPositionZ() const { return _positionZ; }

const float& ws::node::getQuatW() const { return _qw; }
const float& ws::node::getQuatX() const { return _qx; }
const float& ws::node::getQuatY() const { return _qy; }
const float& ws::node::getQuatZ() const { return _qz; }

const int32_t& ws::node::getID() const { return _nodeID; }
const int32_t& ws::node::getParentID() const { return _parentNodeID; }
const int32_t& ws::node::getObjectNameIndex() const { return _objectNameIndex; }
const int32_t& ws::node::getPositionInParent() const { return _cellIndex; }
