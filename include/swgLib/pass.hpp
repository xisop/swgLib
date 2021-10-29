/** -*-c++-*-
 *  \class  pass
 *  \file   pass.hpp
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

#ifndef PASS_HPP
#define PASS_HPP 1

#include <swgLib/base.hpp>
#include <swgLib/ptxm.hpp>
#include <swgLib/pffp.hpp>
#include <swgLib/stag.hpp>

#include<istream>
#include<string>
#include<vector>

namespace ml
{
	class pass
	{
	public:
		pass();
		~pass();

		std::size_t read(std::istream& file);

	protected:
		uint32_t _version; // 0000..0010

		bool   _hasPixelShader;

		int8_t _numStages;

		tag    _pixelShaderTag;
		int8_t _shadeMode;
		int8_t _fogMode;

		bool   _ditherEnable;
		bool   _heat;

		bool   _zEnable;
		bool   _zWrite;
		int8_t _zCompare;

		bool   _alphaBlendEnable;
		int8_t _alphaBlendOperation;
		int8_t _alphaBlendSource;
		int8_t _alphaBlendDestination;

		bool   _alphaTestEnable;
		tag    _alphaReferenceValueTag;
		int8_t _alphaTestFunction;

		uint8_t _writeEnable;

		tag _textureFactorTag;
		tag _textureFactorTag2;
		tag _textureScrollTag;

		// fixed function pipeline ?
		tag _materialTag;

		bool _stencilEnable;
		bool _stencilTwoSidedMode;
		tag  _stencilReferenceValueTag;
		int8_t _stencilCompareFunction;
		int8_t _stencilPassOperation;
		int8_t _stencilZFailOperation;
		int8_t _stencilFailOperation;
		int8_t _stencilCCWCompareFunction;
		int8_t _stencilCCWPassOperation;
		int8_t _stencilCCWZFailOperation;
		int8_t _stencilCCWFailOperation;
		uint32_t _stencilWriteMask;
		uint32_t _stencilMask;

		std::string _vertexShader;

		int8_t _numTextureSamplers;
		std::string _pixelShader;
		std::vector<ptxm> _textureSampler;

		pffp _fixedFunctionPipeline;

		std::vector<stag> _stage;

	private:
	};
}

#endif
