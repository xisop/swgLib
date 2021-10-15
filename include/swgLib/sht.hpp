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
#include <swgLib/matl.hpp>
#include <swgLib/txm.hpp>
#include <swgLib/color4.hpp>

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

		std::size_t readSHT(std::istream& file, std::string path = "");

	  const std::vector<matl> &getMaterials() const;
	  const std::vector<txm> &getTextures() const;
	  const std::string &getEffectName() const;
	  const uint8_t &getTexCoordSet( const txm& tex ) const;
	protected:
		std::size_t readMATS(std::istream& file);
		std::size_t readTXMS(std::istream& file);
		std::size_t readTCSS(std::istream& file);
		std::size_t readTFNS(std::istream& file);
		std::size_t readARVS(std::istream& file);
		std::size_t readSRVS(std::istream& file);

		uint8_t _version;

		std::vector<matl> _material;
		std::vector<txm>  _texture;

		std::map<tag, uint8_t> _texCoordSet;
		std::map<tag, color4>  _texFactor;
		std::map<tag, uint8_t> _alphaReferenceValue;
		std::map<tag, uint32_t> _stencilReferenceValue;

		std::string _effectName;




		/*
				unsigned int readTSNS(std::istream& file, const unsigned short& depth = 0);
				unsigned int readNAME(std::istream& file, const unsigned short& depth = 0);
				unsigned int readEFCT(std::istream& file, const unsigned short& depth = 0);

				std::vector<std::string> texTag;

				std::string materialName;

				std::string diffuseTextureTag;
				std::string diffuseTextureName;

				std::string mainTextureName;
				unsigned int mainTextureUnit;

				std::string normalTextureName;
				unsigned int normalTextureUnit;

				std::string specularTextureName;
				unsigned int specularTextureUnit;

				std::string hueTextureName;
				unsigned int hueTextureUnit;

				std::string environmentTextureName;
				unsigned int environmentTextureUnit;

				std::string dot3TextureName;
				unsigned int dot3TextureUnit;

				std::string lookupTextureName;
				unsigned int lookupTextureUnit;

				std::string maskTextureName;
				unsigned int maskTextureUnit;

				float ambient[4];
				float diffuse[4];
				float specular[4];
				float emissive[4];
				float shininess;

				bool normalMap;
				std::string effectName;

				unsigned int normalMapUnit;
				unsigned int mainMapUnit;
				*/
	private:

	};
}
#endif
