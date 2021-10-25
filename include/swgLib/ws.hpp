/** -*-c++-*-
 *  \class  ws
 *  \file   ws.hpp
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
#include <vector>
#include <list>
#include <string>

#ifndef WS_HPP
#define WS_HPP 1

namespace ml
{

	class ws
	{
	public:
		class node
		{
		public:
			node();
			~node();

			std::size_t read(std::istream& file);
			//std::size_t write(std::ofstream& file) const;
			void print(std::ostream& os) const;

			std::string getObjectFilename() const;
			void setObjectFilename(const std::string& name);

			const float& getPositionX() const;
			const float& getPositionY() const;
			const float& getPositionZ() const;

			const float& getQuatW() const;
			const float& getQuatX() const;
			const float& getQuatY() const;
			const float& getQuatZ() const;

			const int32_t& getID() const;
			const int32_t& getParentID() const;
			const int32_t& getObjectNameIndex() const;
			const int32_t& getPositionInParent() const;

			protected:
			int32_t _nodeID;
			int32_t _parentNodeID;
			int32_t _objectNameIndex;
			int32_t _cellIndex;

			float _qw;
			float _qx;
			float _qy;
			float _qz;

			float _positionX;
			float _positionY;
			float _positionZ;

			float _radius;

			unsigned int _crc;

			// Object filename
			std::string _objectFilename;
		};

	public:
		ws();
		~ws();

		std::size_t read(std::istream& file);
		//std::size_t create(std::ofstream& outfile);
//		std::size_t readMetaFile(std::istream& infile);

		bool canWrite() const { return true; }

		uint32_t getNumNodes() const;
		const ws::node& getNode(const uint32_t& i) const;

		const std::string& getName(const uint32_t& i) const;
	protected:
		std::size_t readNODS(std::istream& file);
		std::size_t readOTNL(std::istream& file);

#if 0
		std::size_t writeNODS(std::ofstream& outfile);
		std::size_t writeNODE(std::ofstream& outfile);
		std::size_t writeOTNL(std::ofstream& outfile);
#endif

	private:
		uint8_t _version;
		std::vector< node > _nodes;
		std::vector< std::string > _names;
	};
}
#endif
