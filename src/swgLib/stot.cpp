/** -*-c++-*-
 *  \class  stot
 *  \file   stot.cpp
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
#include <swgLib/stot.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

stot::stot() :
	shot(),
	_stotBaseObjectFilename(""),
	_stotVersion(0),
	_paletteColorCustomizationVariables(),
	_rangedIntCustomizationVariables()
{
}

stot::~stot()
{
}

std::size_t stot::readSTOT(std::istream& file)
{
	std::size_t stotSize;
	std::size_t total = base::readFormHeader(file, "STOT", stotSize);
	stotSize += 8;
	std::cout << "Found STOT form" << std::endl;

	total += readDERV(file, _stotBaseObjectFilename);
	std::cout << "STOT Base object filename: " << _stotBaseObjectFilename << "\n";

	std::string type;
	std::size_t size;
	total += base::readFormHeader(file, type, size);
	_stotVersion = base::tagToVersion(type);
	std::cout << "STOT version: " << (int)_stotVersion << "\n";

	int32_t numParameters;
	total += readPCNT(file, numParameters);
	for (int32_t i = 0; i < numParameters; ++i)
	{
		total += readSTOTParameter(file);
	}

	total += readSHOT(file);

	if (stotSize == total)
	{
		std::cout << "Finished reading STOT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading STOT" << std::endl;
		std::cout << "Read " << total << " out of " << stotSize
			<< std::endl;
		exit(0);
	}

	return total;
}

void stot::print(std::ostream& os) const
{
}

std::size_t stot::readSTOTParameter(std::istream& file)
{
	std::size_t xxxxSize;
	std::size_t total = base::readRecordHeader(file, "XXXX", xxxxSize);
	xxxxSize += 8;

	std::string parameter;
	total += base::read(file, parameter);

	// 0 - None
	// 1 - Single
	// 2 - Weighted list (count(int32), weight(int32), value)
	// 3 - Range [min, max]
	// 4 - Die Roll [numDice(int32), dieSides(int32), base(int32)
	int8_t dataType;
	total += base::read(file, dataType);

#if 1
	std::cout << "Parameter: " << parameter << std::endl;
	std::cout << "Parameter data type: ";
	switch (dataType) {
	case 1: std::cout << "Single"; break;
	case 2: std::cout << "Weighted list"; break;
	case 3: std::cout << "Range"; break;
	case 4: std::cout << "Die Roll"; break;
	default: std::cout << "None:" << (int)dataType;
	}
	//std::cout << " not handled\n";
	std::cout << "\n";

	const std::size_t valueSize(xxxxSize - total);
	std::cout << "Value size: " << valueSize << "\n";
#endif
	if (parameter == "paletteColorCustomizationVariables")
	{
		int32_t numPCCV;
		total += base::read(file, numPCCV);
		std::cout << "Num PCCV records: " << numPCCV << "\n";
		if (1 == dataType) {
			for (auto i = 0; i < numPCCV; ++i) {
				pccv newPCCV;
				total += newPCCV.read(file);
				_paletteColorCustomizationVariables.push_back(newPCCV);
				std::cout << "Palette color customization variable: \n";
				newPCCV.print(std::cout);
			}
		}
	}
	else if (parameter == "rangedIntCustomizationVariables")
	{
		int32_t numRICV;
		total += base::read(file, numRICV);
		std::cout << "Num RICV records: " << numRICV << "\n";
		if (1 == dataType) {
			for (auto i = 0; i < numRICV; ++i) {
				ricv newRICV;
				total += newRICV.read(file);
				_rangedIntCustomizationVariables.push_back(newRICV);
				std::cout << "Ranged int customization variable: \n";
				newRICV.print(std::cout);
			}
		}
	}
	else if (parameter == "constStringCustomizationVariables")
	{
		int32_t numCSCV;
		total += base::read(file, numCSCV);
		std::cout << "Num CSCV records: " << numCSCV << "\n";
		if (1 == dataType) {
			for (auto i = 0; i < numCSCV; ++i) {
				cscv newCSCV;
				total += newCSCV.read(file);
				_constStringCustomizationVariables.push_back(newCSCV);
				std::cout << "Const string customization variable: \n";
				newCSCV.print(std::cout);
			}
		}
	}
	else if (parameter == "socketDestinations") {
		int32_t socketDest;
		total += base::read(file, socketDest);
		std::cout << "Socket destination: " << socketDest << "\n";
		if (1 == dataType) {
			_socketDestinations.push_back(socketDest);
		}
	}
	else if (parameter == "structureFootprintFileName") {
		if (1 == dataType) {
			total += base::read(file, _structureFootprintFileName);
			std::cout << "Structure footprint filename: " << _structureFootprintFileName << "\n";
		}
	}
	else if (parameter == "useStructureFootprintOutline") {
		if (1 == dataType) {
			total += base::read(file, _useStructureFootprintOutline);
			std::cout << "Use structure footprint outline: " << std::boolalpha << _useStructureFootprintOutline << "\n";
		}
	}
	else if (parameter == "targetable") {
		if (1 == dataType) {
			total += base::read(file, _targetable);
			std::cout << "Targetable: " << std::boolalpha << _targetable << "\n";
		}
	}
	else if (parameter == "certificationsRequired") {
		int32_t numCert;
		total += base::read(file, numCert);
		std::cout << "Num Certification records: " << numCert << "\n";
		if (1 == dataType) {
			for (auto i = 0; i < numCert; ++i) {
				std::cout << "****************** certificationsRequired not finished ******************\n";
				exit(0);

				//total += base::read(file, _);
			}
		}
	}
	else if (parameter == "customizationVariableMapping")
	{
		int32_t numCVM;
		total += base::read(file, numCVM);
		std::cout << "Num CVM records: " << numCVM << "\n";
		if (1 == dataType) {
			for (auto i = 0; i < numCVM; ++i) {
				cvm newCVM;
				total += newCVM.read(file);
				_customizationVariableMapping.push_back(newCVM);
				std::cout << "Customization variable mapping: \n";
				newCVM.print(std::cout);
			}
		}
	}
	else if (parameter == "clientVisabilityFlag") {
		if (1 == dataType) {
			total += base::read(file, _clientVisabilityFlag);
			std::cout << "Client visability flag: 0x" << std::hex << _clientVisabilityFlag << std::dec << "\n";
		}
	}
	else
	{
		std::cout << "Unknown: " << parameter << std::endl;
		exit(0);
	}

	if (xxxxSize != total) {
		std::cout << "FAILED in reading XXXX" << std::endl;
		std::cout << "Read " << total << " out of " << xxxxSize
			<< std::endl;
		exit(0);
	}

	return total;
		}

// ********** Ranged Int Customization Variable (RICV) **********
stot::ricv::ricv(const std::string& variableName,
	const int32_t& minValueInclusive,
	const int32_t& defaultValue,
	const int32_t& maxValueExclusive) :
	_variableName(variableName),
	_minValueInclusive(minValueInclusive),
	_defaultValue(defaultValue),
	_maxValueExclusive(maxValueExclusive)
{
}

stot::ricv::~ricv() {}

size_t stot::ricv::read(std::istream& file) {
	size_t total = base::read(file, _variableName);
	total += base::read(file, _minValueInclusive);
	total += base::read(file, _defaultValue);
	total += base::read(file, _maxValueExclusive);
	return total;
}

void stot::ricv::print(std::ostream& os) const {
	os << "      Variable name: " << _variableName << "\n"
		<< "Min value inclusive: " << _minValueInclusive << "\n"
		<< "      Default value: " << _defaultValue << "\n"
		<< "Max value Exclusive: " << _maxValueExclusive << "\n";
}

// ********** Palette Color Customization Variable (PCCV) **********
stot::pccv::pccv(const std::string& variableName,
	const std::string& palettePathName,
	const int32_t& defaultPaletteIndex) :
	_variableName(variableName),
	_palettePathName(palettePathName),
	_defaultPaletteIndex(defaultPaletteIndex)
{
}

stot::pccv::~pccv() {}

size_t stot::pccv::read(std::istream& file) {
	size_t total = base::read(file, _variableName);
	total += base::read(file, _palettePathName);
	total += base::read(file, _defaultPaletteIndex);
	return total;
}

void stot::pccv::print(std::ostream& os) const {
	os << "        Variable name: " << _variableName << "\n"
		<< "    Palette path name: " << _palettePathName << "\n"
		<< "Default palette index: " << _defaultPaletteIndex << "\n";
}

// ********** Const String Customization Variable (CSCV) **********
stot::cscv::cscv(const std::string& variableName,
	const std::string& constValue) :
	_variableName(variableName),
	_constValue(constValue)
{
}

stot::cscv::~cscv() {}

size_t stot::cscv::read(std::istream& file) {
	size_t total = base::read(file, _variableName);
	total += base::read(file, _constValue);
	return total;
}

void stot::cscv::print(std::ostream& os) const {
	os << "Variable name: " << _variableName << "\n"
		<< "  Const value: " << _constValue << "\n";
}

// ********** Customization Variable Mapping (CVM) **********
stot::cvm::cvm(const std::string& sourceVariable,
	const std::string& dependentVariable) :
	_sourceVariable(sourceVariable),
	_dependentVariable(dependentVariable)
{}

stot::cvm::~cvm() {}

size_t stot::cvm::read(std::istream& file) {
	size_t total = base::read(file, _sourceVariable);
	total += base::read(file, _dependentVariable);
	return total;
}
void stot::cvm::print(std::ostream& os) const {
	os << "   Source variable: " << _sourceVariable << "\n"
		<< "Dependent variable: " << _dependentVariable << "\n";
}
