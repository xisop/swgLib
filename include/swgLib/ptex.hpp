/** -*-c++-*-
 *  \class  ptex
 *  \file   ptex.hpp
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

#include <swgLib/ptcl.hpp>
#include <swgLib/wvfm.hpp>

#include <istream>
#include <string>

#ifndef PTEX_HPP
#define PTEX_HPP 1

namespace ml
{
	class ptex : public ptcl
	{
	public:
		ptex();
		~ptex();

		std::size_t read(std::istream& file);

		const std::string& getShaderPath() const;
		const int32_t& getFrameCount() const;
		const int32_t& getFrameStart() const;
		const int32_t& getFrameEnd() const;
		const float& getFrameUVSize() const;
		const int32_t& getFramesPerColumn() const;
		const float& getFramesPerSecond() const;
		bool isTextureVisible() const;

	protected:
		uint32_t _version;

		std::string _shaderPath;
		int32_t _frameCount;
		int32_t _frameStart;
		int32_t _frameEnd;
		float _frameUVSize;
		int32_t _framesPerColumn;
		float _framesPerSecond;
		bool _textureVisible;

	private:
	};
}
#endif
