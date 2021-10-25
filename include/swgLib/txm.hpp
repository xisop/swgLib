/** -*-c++-*-
 *  \class  txm
 *  \file   txm.hpp
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

#ifndef TXM_HPP
#define TXM_HPP 1

#include <swgLib/base.hpp>

#include <istream>
#include <string>

namespace ml
{
	class txm
	{
	public:
		txm();
		~txm();

		enum wrapMode {
			Wrap = 1,
			Mirror = 2,
			Clamp = 3,
			Border = 4,
			MirrorOnce = 5
		};

		enum filterMode {
			None = 1,
			Point = 2,
			Linear = 3,
			Anisotropic = 4,
			FlatCubic = 5,
			GaussianCubic = 6
		};

		std::size_t read(std::istream& file);

		const tag& getNameTag() const;
		const std::string& getTextureName() const;

		static std::string getWrapModeStr(const int8_t mode);
		static std::string getFilterModeStr(const int8_t mode);

		int8_t getUWrapMode() const;
		int8_t getVWrapMode() const;
		int8_t getWWrapMode() const;

		int8_t getMipFilterMode() const;
		int8_t getMinFilterMode() const;
		int8_t getMagFilterMode() const;

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);

		uint8_t _version;

		bool _placeHolder;
		tag _nameTag;
		std::string _textureName;

		uint8_t _uWrapMode;
		uint8_t _vWrapMode;
		uint8_t _wWrapMode;

		uint8_t  _mipFilterMode;
		uint8_t  _minFilterMode;
		uint8_t  _magFilterMode;
		uint8_t _maxAnisotropy;

	private:
	};
}

#endif
