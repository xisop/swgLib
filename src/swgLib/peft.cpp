/** -*-c++-*-
 *  \class  peft
 *  \file   peft.cpp
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
#include <swgLib/peft.hpp>
#include <swgLib/base.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

peft::peft()
{
}

peft::~peft()
{
}

std::size_t peft::readPEFT(std::istream& file)
{
	std::size_t peftSize;
	std::size_t total = base::readFormHeader(file, "PEFT", peftSize);
	peftSize += 8;
	std::cout << "Found PEFT form: " << peftSize - 12 << " bytes\n";

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	if (_version > 2) {
		std::cout << "Expected type [0000..0002]: " << type << "\n";
		exit(0);
	}
	std::cout << "Particle Effect Version: " << (int)_version << "\n";

	if (_version > 0) {
		total += _timing.read(file);
	}
	
	total += base::readRecordHeader(file, "0000", size);
	total += base::read(file, _numEmitterGroups);

	if (_version > 1) {
		total += base::read(file, _initialPlaybackRate);
		total += base::read(file, _initialPlaybackRateTime);
		total += base::read(file, _playbackRate);
		total += base::read(file, _scale);
	}

	std::cout << "Num Emitter Groups: " << _numEmitterGroups << "\n";



	_emitterGroups.resize(_numEmitterGroups);
	for (auto& eg : _emitterGroups) {
		total += eg.read(file);
	}

	if (peftSize == total) {
		std::cout << "Finished reading PEFT\n";
	}
	else {
		std::cout << "FAILED in reading PEFT\n"
			<< "Read " << total << " out of " << peftSize << "\n";
		exit(0);
	}

	return total;
}

#if 0

std::size_t peft::readEMGP(std::istream& file)
{
	std::size_t emgpSize;
	std::size_t total = base::readFormHeader(file, "EMGP", emgpSize);
	emgpSize += 8;
	std::cout << "Found FORM EMGP: " << emgpSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;

	total += _particleTiming.read(file);

	unsigned int numEmitters;
	total += read0000(file, numEmitters);
	for (unsigned int i = 0; i < numEmitters; ++i)
	{
		total += readEMTR(file);
	}

	if (emgpSize == total)
	{
		std::cout << "Finished reading EMGP" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading EMGP" << std::endl;
		std::cout << "Read " << total << " out of " << emgpSize
			<< std::endl;
	}

	return total;
}

std::size_t peft::readEMTR(std::istream& file)
{
	std::size_t emtrSize;
	std::size_t total = base::readFormHeader(file, "EMTR", emtrSize);
	emtrSize += 8;
	std::cout << "Found FORM EMTR: " << emtrSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string form, type;
	total += base::readFormHeader(file, form, size, type);
	if (form != "FORM")
	{
		std::cout << "Expected FORM: " << form << std::endl;
		exit(0);
	}
	std::cout << "Found " << form << " " << type
		<< ": " << size - 4 << " bytes"
		<< std::endl;

	total += readPTIM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readEMTR0000(file);
	total += readPTQD(file);

	total += base::readUnknown(file, emtrSize - total);

	if (emtrSize == total)
	{
		std::cout << "Finished reading EMTR" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading EMTR" << std::endl;
		std::cout << "Read " << total << " out of " << emtrSize
			<< std::endl;
	}

	return total;
}

// Particle Timing
std::size_t peft::readPTIM(std::istream& file)
{
	std::size_t ptimSize;
	std::size_t total = base::readFormHeader(file, "PTIM", ptimSize);
	ptimSize += 8;
	std::cout << "Found FORM PTIM: " << ptimSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);

	_version = base::tagToVersion(type);
	if (_version > 1) {
		std::cout << "Expected type [0000..0001]: " << type << "\n";
		exit(0);
	}
	std::cout << "PTIM version: " << (int)_version << "\n";


#if 1
	total += 24;
#else
	total += base::read(file, _startDelayMin);
	total += base::read(file, _startDelayMax);
	total += base::read(file, _loopDelayMin);
	total += base::read(file, _loopDelayMax);
	total += base::read(file, _loopCountMin);
	total += base::read(file, _loopCountMax);

	if (0 == _ptimVersion) {
		if (_loopCountMin <= -1) {
			_loopCountMax = -1;
		}
	}
#endif


	if (ptimSize == total)
	{
		std::cout << "Finished reading PTIM\n";
	}
	else
	{
		std::cout << "FAILED in reading PTIM\n"
			<< "Read " << total << " out of " << ptimSize << "\n";
	}

	return total;
}

std::size_t peft::readWVFM(std::istream& file)
{
	std::size_t wvfmSize;
	std::size_t total = base::readFormHeader(file, "WVFM", wvfmSize);
	wvfmSize += 8;
	std::cout << "Found FORM WVFM: " << wvfmSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string type;
	total += base::readRecordHeader(file, type, size);
	if (type != "0001")
	{
		std::cout << "Expected record of type 0001: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	int u1;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	unsigned int num;
	total += base::read(file, num);
	std::cout << "Num: " << num << std::endl;

	float u2;
	std::cout << std::fixed;
	for (unsigned int i = 0; i < num; ++i)
	{
		total += base::read(file, u2);
		std::cout << u2 << " ";

		total += base::read(file, u2);
		std::cout << u2 << " ";

		total += base::read(file, u2);
		std::cout << u2 << " ";

		total += base::read(file, u2);
		std::cout << u2 << std::endl;
	}

	if (wvfmSize == total)
	{
		std::cout << "Finished reading WVFM" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading WVFM" << std::endl;
		std::cout << "Read " << total << " out of " << wvfmSize
			<< std::endl;
	}

	return total;
}

std::size_t peft::readPTQD(std::istream& file)
{
	std::size_t ptqdSize;
	std::size_t total = base::readFormHeader(file, "PTQD", ptqdSize);
	ptqdSize += 8;
	std::cout << "Found FORM PTQD: " << ptqdSize - 12 << " bytes"
		<< std::endl;

	total += readPTCL(file);

	std::size_t size;
	total += base::readFormHeader(file, "0001", size);
	size += 8;
	std::cout << "Found FORM 0001: " << size - 12 << " bytes"
		<< std::endl;

	total += readWVFM(file);
	total += readWVFM(file);
	total += readWVFM(file);
	total += readPTEX(file);

	std::string type;
	total += base::readRecordHeader(file, type, size);
	size += 8;
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	char u3;
	total += base::read(file, u3);
	std::cout << (int)u3 << std::endl;

	if (ptqdSize == total)
	{
		std::cout << "Finished reading PTQD" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PTQD" << std::endl;
		std::cout << "Read " << total << " out of " << ptqdSize
			<< std::endl;
	}

	return total;
}

std::size_t peft::readPTEX(std::istream& file)
{
	std::size_t ptexSize;
	std::size_t total = base::readFormHeader(file, "PTEX", ptexSize);
	ptexSize += 8;
	std::cout << "Found FORM PTEX: " << ptexSize - 12 << " bytes"
		<< std::endl;

	std::string type;
	std::size_t size;
	total += base::readRecordHeader(file, type, size);
	size += 8;
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	std::string name;
	total += base::read(file, name);
	std::cout << name << std::endl;

	float u2;
	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	char u3;
	total += base::read(file, u3);
	std::cout << (int)u3 << std::endl;

	if (ptexSize == total)
	{
		std::cout << "Finished reading PTEX" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PTEX" << std::endl;
		std::cout << "Read " << total << " out of " << ptexSize
			<< std::endl;
	}

	return total;
}

std::size_t peft::readPTCL(std::istream& file)
{
	std::size_t ptclSize;
	std::size_t total = base::readFormHeader(file, "PTCL", ptclSize);
	ptclSize += 8;
	std::cout << "Found FORM PTCL: " << ptclSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	total += base::readFormHeader(file, "0002", size);
	size += 8;
	std::cout << "Found FORM 0002: " << size - 12 << " bytes"
		<< std::endl;

	std::string type;
	total += base::readRecordHeader(file, type, size);
	size += 8;
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	std::string name;
	total += base::read(file, name);
	std::cout << name << std::endl;

	unsigned int u1;
	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	unsigned char u2;
	total += base::read(file, u2);
	std::cout << (int)u2 << std::endl;

	total += readCLRR(file);
	total += readWVFM(file);
	total += readWVFM(file);

	total += base::readUnknown(file, ptclSize - total);

	if (ptclSize == total)
	{
		std::cout << "Finished reading PTCL" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading PTCL" << std::endl;
		std::cout << "Read " << total << " out of " << ptclSize
			<< std::endl;
	}

	return total;
}

std::size_t peft::readCLRR(std::istream& file)
{
	std::size_t clrrSize;
	std::size_t total = base::readFormHeader(file, "CLRR", clrrSize);
	clrrSize += 8;
	std::cout << "Found FORM CLRR: " << clrrSize - 12 << " bytes"
		<< std::endl;

	float u1;
	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	char u2, u3, u4, u5;
	total += base::read(file, u2);
	total += base::read(file, u3);
	total += base::read(file, u4);
	total += base::read(file, u5);

	std::cout << (int)u2 << ", "
		<< (int)u3 << ", "
		<< (int)u4 << ", "
		<< (int)u5 << std::endl;

	unsigned int numFloats = u5 / sizeof(float);
	for (unsigned int i = 0; i < numFloats; ++i)
	{
		total += base::read(file, u1);
		std::cout << u1 << std::endl;
	}

	if (clrrSize == total)
	{
		std::cout << "Finished reading CLRR" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading CLRR" << std::endl;
		std::cout << "Read " << total << " out of " << clrrSize
			<< std::endl;
	}

	return total;
}


std::size_t peft::read0000(std::istream& file, unsigned int& num)
{
	std::size_t size;
	std::string type;
	std::size_t total = base::readRecordHeader(file, type, size);
	size += 8;
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	total += base::read(file, num);

	if (size == total)
	{
		std::cout << "Finished reading 0000" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading 0000" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}

std::size_t peft::readEMTR0000(std::istream& file)
{
	std::size_t size;
	std::string type;
	std::size_t total = base::readRecordHeader(file, type, size);
	size += 8;
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	std::string name;
	total += base::read(file, name);
	std::cout << name << std::endl;

	int u1;
	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	float u2;
	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	total += base::read(file, u2);
	std::cout << u2 << std::endl;

	short u3;
	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u3);
	std::cout << u3 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::read(file, u1);
	std::cout << u1 << std::endl;

	total += base::readUnknown(file, size - total);

	if (size == total)
	{
		std::cout << "Finished reading 0000" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading 0000" << std::endl;
		std::cout << "Read " << total << " out of " << size
			<< std::endl;
	}

	return total;
}


#endif