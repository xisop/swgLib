/** -*-c++-*-
 *  \class  sht
 *  \file   sht.hpp
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
#include <swgLib/base.hpp>
#include <swgLib/color4.hpp>
#include <swgLib/efct.hpp>
#include <swgLib/matl.hpp>
#include <swgLib/txm.hpp>

#include <istream>
#include <string>
#include <vector>
#include <map>

#ifndef SHT_HPP
#define SHT_HPP 1

namespace ml
{
	class sht
	{
	public:
		sht();
		~sht();

		std::size_t readSHT(std::istream& file);

		const std::vector<matl>& getMaterials() const;
		const std::vector<txm>& getTextures() const;
		const std::string& getEffectName() const;
		uint8_t getTexCoordSet(const txm& tex) const;

	public:
		struct textureScroll {
			float u1, v1;
			float u2, v2;
		};

	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);

	protected:
		std::size_t readMATS(std::istream& file);
		std::size_t readTXMS(std::istream& file);
		std::size_t readTCSS(std::istream& file);
		std::size_t readTFNS(std::istream& file);
		std::size_t readTSNS(std::istream& file);
		std::size_t readARVS(std::istream& file);
		std::size_t readSRVS(std::istream& file);

		uint8_t _version;

		std::vector<matl> _material;
		std::vector<txm>  _texture;

		std::map<tag, uint8_t>       _texCoordSet;
		std::map<tag, color4>        _texFactor;
		std::map<tag, uint8_t>       _alphaReferenceValue;
		std::map<tag, uint32_t>      _stencilReferenceValue;
		std::map<tag, textureScroll> _texScroll;

		std::string _effectName;
		efct        _effect;

	private:
	};
}
#endif
