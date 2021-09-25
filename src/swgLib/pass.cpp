/** -*-c++-*-
 *  \class  pass
 *  \file   pass.cpp
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

#include <swgLib/pass.hpp>
#include <swgLib/base.hpp>

using namespace ml;

pass::pass() {
}

pass::~pass() {
}

std::size_t pass::read(std::istream& file) {
	std::size_t passSize;
	std::size_t total = base::readFormHeader(file, "PASS", passSize);
	passSize += 8;
	std::cout << "Found FORM PASS: " << passSize - 12 << " bytes\n";

	std::size_t size;
	std::string type;
	total += base::readFormHeader(file, type, size);
	_version = base::tagToVersion(type);

	if (_version > 10)
	{
		std::cout << "Expected FORM of type [0000..0010]. Found: " << type << "\n";
		exit(0);
	}
	std::cout << "Found FORM " << type << ": " << size << " bytes\n";

	std::cout << "Shader pass version: " << (int)_version << "\n";

	// Versions 1+...
	if (_version > 0) {
		// Read DATA record
		total += base::readRecordHeader(file, "DATA", size);
		std::cout << "Found record DATA: " << size << "\n";
	}

	// Versions 2, 3, and 4...
	if ((1 < _version) && (_version < 4)) {
		total += base::read(file, _hasPixelShader);
		std::cout << "Has pixel shader: " << std::boolalpha << _hasPixelShader << "\n";
	}

	total += base::read(file, _numStages);
	std::cout << "Number of stages: " << (int)_numStages << "\n";

	if (1 == _version) {
		total += base::read(file, _pixelShaderTag);
		std::cout << "Pixel shader tag: " << _pixelShaderTag << "\n";
	}

	total += base::read(file, _shadeMode);
	std::cout << "Shade mode: " << (int)_shadeMode << "\n";

	// Versions 2+...
	if (_version > 1) {
		total += base::read(file, _fogMode);
		std::cout << "Fog mode: " << (int)_fogMode << "\n";
	}

	total += base::read(file, _ditherEnable);
	std::cout << "Dither enable: " << std::boolalpha << _ditherEnable << "\n";

	if (10 == _version) {
		total += base::read(file, _heat);
		std::cout << "Heat: " << std::boolalpha << _heat << "\n";
	}

	total += base::read(file, _zEnable);
	total += base::read(file, _zWrite);
	total += base::read(file, _zCompare);

	std::cout << "Z Enable: " << std::boolalpha << _zEnable << "\n";
	std::cout << "Z Write: " << std::boolalpha << _zWrite << "\n";
	std::cout << "Z Compare: " << std::boolalpha << (int)_zCompare << "\n";

	total += base::read(file, _alphaBlendEnable);
	total += base::read(file, _alphaBlendOperation);
	total += base::read(file, _alphaBlendSource);
	total += base::read(file, _alphaBlendDestination);

	std::cout << "Alpha blend enable: " << std::boolalpha << _alphaBlendEnable << "\n";
	std::cout << "Alpha blend operation: " << (int)_alphaBlendOperation << "\n";
	std::cout << "Alpha blend source: " << (int)_alphaBlendSource << "\n";
	std::cout << "Alpha blend destination: " << (int)_alphaBlendDestination << "\n";

	total += base::read(file, _alphaTestEnable);
	total += base::read(file, _alphaReferenceValueTag);
	total += base::read(file, _alphaTestFunction);

	std::cout << "Alpha test enable: " << std::boolalpha << _alphaTestEnable << "\n";
	std::cout << "Alpha reference value tag: " << _alphaReferenceValueTag << "\n";
	std::cout << "Alpha test function: " << (int)_alphaTestFunction << "\n";

	total += base::read(file, _writeEnable);
	std::cout << "Write enable: " << (int)_writeEnable << "\n";

	total += base::read(file, _textureFactorTag);
	std::cout << "Texture factor tag: " << _textureFactorTag << "\n";

	if (_version > 6) {
		total += base::read(file, _textureFactorTag2);
		std::cout << "Texture factor tag 2: " << _textureFactorTag2 << "\n";
	}
	if (_version > 7) {
		total += base::read(file, _textureScrollTag);
		std::cout << "Texture scroll tag: " << _textureScrollTag << "\n";
	}

	if (_version < 4) {
		// fixed function pipeline ?
		// material tag
		std::cout << "Not handled yet: " << __FILE__ << ": " << __LINE__ << "\n";
		exit(0);
	}

	// stencil enable (bool)
	total += base::read(file, _stencilEnable);
	std::cout << "Stencil enable: " << std::boolalpha << _stencilEnable << "\n";

	if (_version > 8) {
		total += base::read(file, _stencilTwoSidedMode);
		std::cout << "Stencil two sided mode: " << std::boolalpha << _stencilTwoSidedMode << "\n";
	}
	total += base::read(file, _stencilReferenceValueTag);
	total += base::read(file, _stencilCompareFunction);
	total += base::read(file, _stencilPassOperation);
	total += base::read(file, _stencilZFailOperation);
	total += base::read(file, _stencilFailOperation);

	std::cout << "Stencil reference value tag: " << _stencilReferenceValueTag << "\n";
	std::cout << "Stencil compare function: " << (int)_stencilCompareFunction << "\n";
	std::cout << "Stencil pass operation: " << (int)_stencilPassOperation << "\n";
	std::cout << "Stencil z fail operation: " << (int)_stencilZFailOperation << "\n";
	std::cout << "Stencil fail operation: " << (int)_stencilFailOperation << "\n";

	if (_version > 8) {
		total += base::read(file, _stencilCCWCompareFunction);
		total += base::read(file, _stencilCCWPassOperation);
		total += base::read(file, _stencilCCWZFailOperation);
		total += base::read(file, _stencilCCWFailOperation);

		std::cout << "Stencil CCW compare function: " << (int)_stencilCompareFunction << "\n";
		std::cout << "Stencil CCW pass operation: " << (int)_stencilPassOperation << "\n";
		std::cout << "Stencil CCW z fail operation: " << (int)_stencilZFailOperation << "\n";
		std::cout << "Stencil CCW fail operation: " << (int)_stencilFailOperation << "\n";
	}
	// stencil write mask (uint32)
	total += base::read(file, _stencilWriteMask);
	std::cout << "Stencil write mask: " << _stencilWriteMask << "\n";
	// stencil stencil mask (uint32)
	total += base::read(file, _stencilMask);
	std::cout << "Stencil mask: " << _stencilMask << "\n";

	if (3 == _version) {
		uint32_t unused;
		total += base::read(file, unused);
	}

	if (_version > 5) {
		total += base::read(file, _materialTag);
		std::cout << "Material tag: " << _materialTag << "\n";
	}

	if (_version > 3) {
		// PFFP or PVSH record...
		std::string form;
		base::peekHeader(file, form, size, type);
		std::cout << "Peek: " << form << " " << type << "\n";

		if ("PFFP" == type) {
			total += _fixedFunctionPipeline.read(file);
		}

		if ("PVSH" == type) {
			total += base::readFormHeader(file, "PVSH", size);
			total += base::readRecordHeader(file, "0000", size);
			total += base::read(file, _vertexShader);
			base::fixSlash(_vertexShader);
			std::cout << "Vertex shader: " << _vertexShader << "\n";
		}
	}

	if ((_version > 4) && (0 == _numStages)) {
		std::string form;
		base::peekHeader(file, form, size, type);
		std::cout << "Peek: " << form << " " << type << "\n";
		if ("PPSH" == type) {
			total += base::readFormHeader(file, "PPSH", size);
			std::cout << "Found form PPSH\n";
			total += base::readFormHeader(file, "0001", size);
			std::cout << "Found form 0001\n";
			total += base::readRecordHeader(file, "DATA", size);
			std::cout << "Found record DATA\n";

			total += base::read(file, _numTextureSamplers);
			std::cout << "Number of texture samplers: " << (int)_numTextureSamplers << "\n";

			total += base::read(file, _pixelShader);
			base::fixSlash(_pixelShader);
			std::cout << "Pixel shader: " << _pixelShader << "\n";

			// Read all the texture samplers...
			_textureSampler.resize(_numTextureSamplers);
			for (auto& ts : _textureSampler) {
				total += ts.read(file);
			}
		}
	}
	else {
		// Stages...
		_stage.resize(_numStages);
		for (auto& s : _stage) {
			total += s.read(file);
		}
	}

	// TODO...

	if (passSize == total)
	{
		std::cout << "Finished reading PASS\n";
	}
	else
	{
		std::cout << "FAILED in reading PASS\n";
		std::cout << "Read " << total << " out of " << passSize << "\n";
		exit(0);
	}

	return total;
}
