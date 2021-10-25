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

const std::vector<matl>& sht::getMaterials() const { return _material; }

const std::vector<txm>& sht::getTextures() const { return _texture; }

const std::string& sht::getEffectName() const { return _effectName; }

uint8_t sht::getTexCoordSet(const txm& tex) const {
	const auto& set = _texCoordSet.find(tex.getNameTag());
	if (_texCoordSet.end() == set) {
		return 0;
	}
	else {
		return set->second;
	}
}

std::size_t sht::readSHT(std::istream& file)
{
	std::size_t sshtSize;
	std::size_t total = base::readFormHeader(file, "SSHT", sshtSize);
	sshtSize += 8; // Include FORM/Size fields from header...
	std::cout << "Found SSHT form: " << sshtSize << " bytes\n";

	std::size_t size;
	std::string form, type;
	base::peekHeader(file, form, size, type);
	_version = base::tagToVersion(type);
	std::cout << "SSHT version: " << (int)_version << "\n";

	if (0 == _version) {
		total += readV0(file);
	}
	else if (1 == _version) {
		total += readV1(file);
	}
	else {
		std::cout << "Unsupported Static Shader (SSHT) version: " << (int)_version << "\n";
		exit(0);
	}

	if (sshtSize == total) {
		std::cout << "Finished reading SSHT\n";
	}
	else {
		std::cout << "FAILED in reading SSHT\n";
		std::cout << "Read " << total << " out of " << sshtSize << "\n";
	}

	return total;
}

std::size_t sht::readV0(std::istream& file) {
	std::size_t size;
	std::size_t total = base::readFormHeader(file, "0000", size);

	std::string form, type;
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

	// Load effect...
	base::peekHeader(file, form, size, type);
	std::cout << "Peek: " << form << ":" << type << "\n";
	if ("NAME" == form) {
		total += base::readRecordHeader(file, "NAME", size);
		std::cout << "Found record NAME: " << size << "\n";
		total += base::read(file, _effectName);
		base::fixSlash(_effectName);
		std::cout << "Effect: " << _effectName << "\n";
	}
	else if ("EFCT" == form) {
		total += _effect.readEFCT(file);
	}
	else {
		std::cout << "Unhandled effect type: " << type << "\n";
		exit(0);
	}

	return total;
}

std::size_t sht::readV1(std::istream& file) {
	std::size_t size;
	std::size_t total = base::readFormHeader(file, "0001", size);

	// Load effect...
	std::string form, type;
	base::peekHeader(file, form, size, type);
	if ("NAME" == type) {
		total += base::readRecordHeader(file, "NAME", size);
		std::cout << "Found record NAME: " << size << "\n";
		total += base::read(file, _effectName);
		base::fixSlash(_effectName);
		std::cout << "Effect: " << _effectName << "\n";
	}
	else if ("EFCT" == type) {
		total += _effect.readEFCT(file);
	}
	else {
		std::cout << "Unhandled effect type: " << type << "\n";
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

	// Load TSNS...
	base::peekHeader(file, form, size, type);
	if (("FORM" == form) && ("TSNS" == type)) {
		total += readTSNS(file);
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
	std::cout << "Found FORM TXMS: " << txmsSize << " bytes\n";

	while (total < txmsSize ) {
		std::cout << "Reading texture " << _texture.size() << ":\n";
		_texture.resize(_texture.size() + 1);

		// Read texture...
		const std::size_t txmSize = _texture.back().read(file);

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

	if (tcssSize == total) {
		std::cout << "Finished reading TCSS\n";
	}
	else {
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
	for (uint32_t i = 0; i < num; ++i) {
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

std::size_t sht::readTSNS(std::istream& file) {
	std::size_t tsnsSize;
	std::size_t total = base::readFormHeader(file, "TSNS", tsnsSize);
	tsnsSize += 8;
	std::cout << "Found FORM TSNS: " << tsnsSize - 12 << " bytes\n";

	std::size_t size;
	total += base::readRecordHeader(file, "0000", size);
	std::cout << "Found record 0000: " << size << " bytes\n";

	tag texTag;
	total += base::read(file, texTag);
	textureScroll scroll;
	total += base::read(file, scroll.u1);
	total += base::read(file, scroll.v1);
	total += base::read(file, scroll.u2);
	total += base::read(file, scroll.v2);

	std::cout << "Scroll texture: " << texTag.str() << "\n"
		<< "u1: " << scroll.u1 << "\n"
		<< "v1: " << scroll.v1 << "\n"
		<< "u2: " << scroll.u2 << "\n"
		<< "v2: " << scroll.v2 << "\n";

	_texScroll[texTag] = scroll;

	if (tsnsSize == total) {
		std::cout << "Finished reading TSNS\n";
	}
	else {
		std::cout << "FAILED in reading TSNS\n";
		std::cout << "Read " << total << " out of " << tsnsSize << "\n";
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
