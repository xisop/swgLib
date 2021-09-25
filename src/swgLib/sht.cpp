/** -*-c++-*-
 *  \class  sht
 *  \file   sht.cpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2006-2014 Ken Sewell

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

#include <swgLib/sht.hpp>

using namespace ml;

sht::sht()
{
}

sht::~sht()
{
}

std::size_t sht::readSHT(std::istream& file, std::string path)
{
	std::size_t sshtSize;
	std::size_t total = base::readFormHeader(file, "SSHT", sshtSize);
	sshtSize += 8; // Include FORM/Size fields from header...
	std::cout << "Found SSHT form: " << sshtSize << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);

	_version = base::tagToVersion(type);
	if ((0 != _version) && (1 != _version)) {
		std::cout << "Expected version 0000 or 0001. Found " << type << "\n";
		exit(0);
	}
	std::cout << "Found form " << type << "\n";
	std::cout << "SSHT version: " << (int)_version << "\n";
	
	std::string form;

	if (1 == _version) {
		// Load effect...
		std::cout << "Not handled yet: " << __FILE__ << ": " << __LINE__ << "\n";
		exit(0);
	}

	// Load materials...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("MATS" == type)) {
		total += readMATS(file);
	}

	// Load textures...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("TXMS" == type)) {
		total += readTXMS(file);
	}

	// Load texture coordinate set...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("TCSS" == type)) {
		total += readTCSS(file);
	}

	// Load texture factor...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("TFNS" == type)) {
		total += readTFNS(file);
	}

	if (1 == _version) {
		// Load texture scroll...
		std::cout << "Not handled yet: " << __FILE__ << ": " << __LINE__ << "\n";
		exit(0);
	}

	// Load alpha reverence value...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("ARVS" == type)) {
		total += readARVS(file);
	}

	// Load stencil reference value...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("SRVS" == type)) {
		total += readSRVS(file);
	}

	if (0 == _version) {
		// Load effect...
		total += base::readRecordHeader(file, "NAME", size);
		std::cout << "Found record NAME: " << size << "\n";
		total += base::read(file, _effectName);
		base::fixSlash(_effectName);
		std::cout << "Effect: " << _effectName << "\n";
	}

	if (sshtSize == total)
	{
		std::cout << "Finished reading SSHT\n";
	}
	else
	{
		std::cout << "FAILED in reading SSHT\n";
		std::cout << "Read " << total << " out of " << sshtSize << "\n";
	}

	return total;
}

std::size_t sht::readMATS(std::istream& file)
{
	std::size_t matsSize;
	std::size_t total = base::readFormHeader(file, "MATS", matsSize);
	matsSize += 8;
	std::cout << "Found FORM MATS: " << matsSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);
	size -= 4; // Subtract off form tag "0000"
	std::cout << "Found FORM 0000: " << size << " bytes\n";

	while (size > 0) {
		std::cout << "Reading material " << _material.size() << ":\n";
		_material.resize(_material.size() + 1);

		// Read TAG record...
		std::size_t tagSize;
		std::size_t matSize = base::readRecordHeader(file, "TAG ", tagSize);
		std::cout << "Found record TAG : " << tagSize << " bytes\n";

		// Read material name...
		tag nameTag;
		matSize += base::read(file, nameTag);
		std::cout << "Material name: " << nameTag << "\n";

		// Set material name...
		_material.back().setNameTag(nameTag);

		// Read material...
		matSize += _material.back().read(file);

		size -= matSize;
		total += matSize;
	}

	if (matsSize == total)
	{
		std::cout << "Finished reading MATS\n";
	}
	else
	{
		std::cout << "FAILED in reading MATS\n";
		std::cout << "Read " << total << " out of " << matsSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t sht::readTXMS(std::istream& file)
{
	std::size_t txmsSize;
	std::size_t total = base::readFormHeader(file, "TXMS", txmsSize);
	txmsSize += 8;
	std::size_t dataSize = txmsSize - 12;
	std::cout << "Found FORM TXMS: " << dataSize << " bytes\n";

	while (dataSize > 0) {
		std::cout << "Reading texture " << _texture.size() << ":\n";
		_texture.resize(_texture.size() + 1);

		// Read texture...
		const std::size_t txmSize = _texture.back().read(file);

		dataSize -= txmSize;
		total += txmSize;
	}

	if (txmsSize == total)
	{
		std::cout << "Finished reading TXMS\n";
	}
	else
	{
		std::cout << "FAILED in reading TXMS\n";
		std::cout << "Read " << total << " out of " << txmsSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t sht::readTCSS(std::istream& file)
{
	std::size_t tcssSize;
	std::size_t total = base::readFormHeader(file, "TCSS", tcssSize);
	tcssSize += 8;
	std::cout << "Found FORM TCSS: " << tcssSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	std::cout << "Found record 0000: " << size << " bytes\n";

	const uint32_t num = uint32_t(size / 5);
	tag texTag;
	uint8_t index;
	for (uint32_t i = 0; i < num; ++i) {
		total += base::read(file, texTag);
		total += base::read(file, index);
		_texCoordSet[texTag] = index;
		std::cout << "Tex Coord Set: " << texTag << ": " << (int)index << "\n";
	}

	/*
			if (texName == "NIAM")
			{
				mainTextureUnit = texUnit;
			}
			else if (texName == "LMRN")
			{
				normalTextureUnit = texUnit;
			}
			else if (texName == "3TOD")
			{
				dot3TextureUnit = texUnit;
			}
			else if (texName == "PUKL")
			{
				lookupTextureUnit = texUnit;
			}
			else if (texName == "MVNE")
			{
				environmentTextureUnit = texUnit;
			}
			else if (texName == "KSAM")
			{
				maskTextureUnit = texUnit;
			}
			else if (texName == "BEUH")
			{
				hueTextureUnit = texUnit;
			}
			else if (texName == "CEPS")
			{
				specularTextureUnit = texUnit;
			}
			else if (texName == "MRNC")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "SIME")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "1PLA")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "2PLA")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "3PLA")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "ATED")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "ALTD")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "BLTD")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "TRID")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "DIRI")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "NRCS")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "0PER")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "LACD")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "YKS_")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "EBUC")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "PFFM")
			{
				//specularTextureUnit = texUnit;
			}
			else if (texName == "NEPO")
			{
				//specularTextureUnit = texUnit;
			}
			else
			{
				std::cout << "Unknown texture tag: " << texName << std::endl;
				exit(0);
			}
		}
		*/

	if (tcssSize == total)
	{
		std::cout << "Finished reading TCSS\n";
	}
	else
	{
		std::cout << "FAILED in reading TCSS\n";
		std::cout << "Read " << total << " out of " << tcssSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t sht::readTFNS(std::istream& file)
{
	std::size_t tfnsSize;
	std::size_t total = base::readFormHeader(file, "TFNS", tfnsSize);
	tfnsSize += 8;
	std::cout << "Found FORM TFNS: " << tfnsSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	std::cout << "Found record 0000: " << size << " bytes\n";

	const uint32_t num = uint32_t(size / 8);
	for(uint32_t i=0; i < num; ++i ) {
		tag texTag;
		total += base::read(file, texTag);

		// Texture Factor node read...
		color4 c;
		total += c.read8(file);
		_texFactor[texTag] = c;
		std::cout << "Texture factor " << i << ": " << texTag << ": " << c << "\n";
	}

	if (tfnsSize == total)
	{
		std::cout << "Finished reading TFNS\n";
	}
	else
	{
		std::cout << "FAILED in reading TFNS\n";
		std::cout << "Read " << total << " out of " << tfnsSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t sht::readARVS(std::istream& file)
{
	std::size_t arvsSize;
	std::size_t total = base::readFormHeader(file, "ARVS", arvsSize);
	arvsSize += 8;
	std::cout << "Found FORM ARVS: " << arvsSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	std::cout << "Found record 0000: " << size << " bytes\n";

	const uint32_t num = uint32_t(size / 5);

	for (uint32_t i = 0; i < num; ++i) {
		tag alphaTag;
		uint8_t alphaReferenceValue;
		total += base::read(file, alphaTag);
		total += base::read(file, alphaReferenceValue);
		_alphaReferenceValue[alphaTag] = alphaReferenceValue;
		std::cout << "Alpha reference " << i << ": " << alphaTag << ": " << (int)alphaReferenceValue << "\n";
	}

	if (arvsSize == total)
	{
		std::cout << "Finished reading ARVS\n";
	}
	else
	{
		std::cout << "FAILED in reading ARVS\n";
		std::cout << "Read " << total << " out of " << arvsSize << "\n";
		exit(0);
	}

	return total;
}

std::size_t sht::readSRVS(std::istream& file)
{
	std::size_t srvsSize;
	std::size_t total = base::readFormHeader(file, "SRVS", srvsSize);
	srvsSize += 8;
	std::cout << "Found FORM SRVS: " << srvsSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	std::cout << "Found record 0000: " << size << " bytes\n";

	const uint32_t num = uint32_t(size / 5);

	for (uint32_t i = 0; i < num; ++i) {
		tag stencilTag;
		uint32_t stencilReferenceValue;
		total += base::read(file, stencilTag);
		total += base::read(file, stencilReferenceValue);
		_stencilReferenceValue[stencilTag] = stencilReferenceValue;
		std::cout << "Stencil reference " << i << ": " << stencilTag << ": " << (int)stencilReferenceValue << "\n";
	}

	if (srvsSize == total)
	{
		std::cout << "Finished reading SRVS\n";
	}
	else
	{
		std::cout << "FAILED in reading SRVS\n";
		std::cout << "Read " << total << " out of " << srvsSize << "\n";
		exit(0);
	}

	return total;
}

/*
unsigned int sht::readTSNS(std::istream& file, const unsigned short& depth)
{
	std::size_t tsnsSize;
	std::size_t total = readFormHeader(file, "TSNS", tsnsSize);
	tsnsSize += 8;
	std::cout << std::string(depth, ' ')
		<< "Found FORM TSNS: " << tsnsSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	std::string type;
	total += readRecordHeader(file, type, size);
	if (type != "0000")
	{
		std::cout << "Expected record of type 0000: " << type << std::endl;
		exit(0);
	}
	std::cout << std::string(depth, ' ')
		<< "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	total += readUnknown(file, size);

	if (tsnsSize == total)
	{
		std::cout << std::string(depth, ' ')
			<< "Finished reading TSNS" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading TSNS" << std::endl;
		std::cout << "Read " << total << " out of " << tsnsSize
			<< std::endl;
	}

	return total;
}


unsigned int sht::readEFCT(std::istream& file, const unsigned short& depth)
{
	std::size_t efctSize;
	std::string type;

	std::size_t total = readFormHeader(file, "EFCT", efctSize);
	efctSize += 8;
	std::cout << std::string(depth, ' ')
		<< "Found FORM EFCT: " << efctSize - 12 << " bytes"
		<< std::endl;

	std::size_t size;
	total += readFormHeader(file, "0001", size);
	std::cout << std::string(depth, ' ')
		<< "Found FORM 0001: " << size - 4 << " bytes"
		<< std::endl;

	total += readRecordHeader(file, type, size);
	if (type != "DATA")
	{
		std::cout << "Expected record of type DATA: " << type << std::endl;
		exit(0);
	}
	std::cout << std::string(depth, ' ')
		<< "Found record " << type
		<< ": " << size << " bytes"
		<< std::endl;

	total += readUnknown(file, size);

	if (efctSize == total)
	{
		std::cout << std::string(depth, ' ')
			<< "Finished reading EFCT" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading EFCT" << std::endl;
		std::cout << "Read " << total << " out of " << efctSize
			<< std::endl;
	}

	return total;
}

unsigned int sht::readNAME(std::istream& file, const unsigned short& depth)
{
	std::size_t nameSize;
	std::string type;
	// Read Effect file record
	std::size_t total = readRecordHeader(file, type, nameSize);
	nameSize += 8;
	if (type != "NAME")
	{
		std::cout << "Expected record of type NAME: " << type << std::endl;
		return 0;
	}
	std::cout << std::string(depth, ' ')
		<< "Found record " << type
		<< ": " << nameSize << " bytes"
		<< std::endl;

	total += base::read(file, effectName);
	base::fixSlash(effectName);

	std::string fullEffectName = basePath + effectName;
	std::cout << std::string(depth + 1, ' ')
		<< "Effect file: " << fullEffectName << std::endl;

	if (nameSize == total)
	{
		std::cout << std::string(depth, ' ')
			<< "Finished reading NAME" << std::endl;
	}
	else
	{
		std::cout << "FAILED in reading NAME" << std::endl;
		std::cout << "Read " << total << " out of " << nameSize
			<< std::endl;
	}

	return total;
}
*/
