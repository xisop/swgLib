/** -*-c++-*-
 *  \class  swts
 *  \file   swts.hpp
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

#include <fstream>
#include <string>
#include <vector>

#ifndef SWTS_HPP
#define SWTS_HPP

namespace ml
{

	class swts : public base
	{
	public:
		swts();
		~swts();
		bool isRightType(std::istream& file)
		{
			return isOfType(file, "SWTS");
		}
		unsigned int readSWTS(std::istream& file, std::string path = "");

		std::string getShaderFilename() const
		{
			return shaderFilename;
		}

		uint32_t getNumTextures() const
		{
			return uint32_t(textureList.size());
		}

		bool getTextureInfo(const unsigned int& index,
			std::string& textureFilename,
			std::string& textureTag) const;

		float getSequenceTime() const
		{
			return sequenceTime;
		}

		float getDeltaTime() const
		{
			return deltaTime;
		}

		void print() const;

	protected:
		unsigned int readNAME(std::istream& file);
		unsigned int readDTST(std::istream& file);
		unsigned int readDRTS(std::istream& file);
		unsigned int readTEXT(std::istream& file);

	private:
		unsigned int numTextures;
		float sequenceTime;
		float deltaTime;
		std::string shaderFilename;
		std::vector<std::string> textureList;
		std::vector<std::string> textureTagList;
	};
}
#endif
