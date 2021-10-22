/** -*-c++-*-
 *  \class  mesh
 *  \file   mesh.hpp
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
#include <swgLib/appr.hpp>
#include <swgLib/sps.hpp>

#include <swgLib/model.hpp>
#include <swgLib/cshd.hpp>
#include <swgLib/sht.hpp>
/*
#include <swgLib/mshVertexData.hpp>
#include <swgLib/mshVertexIndex.hpp>
*/

#include <fstream>
#include <string>
#include <vector>

#ifndef MESH_HPP
#define MESH_HPP 1

namespace ml
{
	enum maxTextures {
		MAX_TEXTURES = 8
	};

	class mesh //: public model
	{
	public:
		mesh();
		~mesh();

		std::size_t readMESH(std::istream& file, bool skipSIDX=false);

		const sps& getSPS() const;

		/*
		uint32_t getNumIndexTables() const
		{
			return uint32_t(indexData.size());
		}

		uint32_t getNumVertexSets() const
		{
			return uint32_t(vertexData.size());
		}

		unsigned int getBytesPerVertex(const unsigned int& index) const
		{
			return vertexData[index]->getBytesPerVertex();
		}

		bool getIndex(
			unsigned int indexNum,
			mshVertexData** vData,
			mshVertexIndex** iData,
			std::string& shaderFilename
		);
		bool getMeshIndex(unsigned int indexNum,
			unsigned int& dataSet,
			mshVertexIndex** iData
		);

		bool getIndex(
			unsigned int indexNum,
			mshVertexData** vData,
			std::string& shaderFilename
		);

		uint32_t getNumshaders() const
		{
			return uint32_t(shaderList.size());
		}

		std::string getShader(unsigned int index) const
		{
			if (index < shaderList.size())
			{
				return shaderList[index];
			}
			else
			{
				return std::string("");
			}
		}
		
		void getBoundingSphere(float& centerX, float& centerY, float& centerZ,
			float& Radius)
		{
			centerX = cx;
			centerY = cy;
			centerZ = cz;
			Radius = radius;
		}

		void getBoundingBox(float& X1, float& Y1, float& Z1,
			float& X2, float& Y2, float& Z2)
		{
			X1 = x1;
			Y1 = y1;
			Z1 = z1;
			X2 = x2;
			Y2 = y2;
			Z2 = z2;
		}
		*/
	protected:
		int8_t _version;

		appr _appearance;
		sps _sps;

		unsigned int readD3DFVF(std::istream& file,
			unsigned int& codes,
			unsigned int& numTextures);

	private:
		/*
		std::vector<bool> isSHT;
		std::vector<std::string> shaderList;

		std::vector<mshVertexData*> vertexData;
		std::vector<mshVertexIndex*> indexData;

		// Bounding sphere center and radius
		float cx, cy, cz, radius;

		// 2 xyz points defining bounding box
		float x1, y1, z1;
		float x2, y2, z2;
		*/
	};
}
#endif
