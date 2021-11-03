/** -*-c++-*-
 *  \class  tgen
 *  \file   tgen.hpp
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

#include <istream>
#include <vector>

#include <swgLib/sgrp.hpp>
#include <swgLib/fgrp.hpp>
#include <swgLib/rgrp.hpp>
#include <swgLib/egrp.hpp>
#include <swgLib/mgrp.hpp>
//#include <swgLib/bgrp.hpp>


#ifndef TGEN_HPP
#define TGEN_HPP 1

namespace ml
{
	class tgen
	{
	public:
		class baseLayer {
		public:
			std::size_t read(std::istream& file);

		protected:
			std::string _tag;
			bool        _active;
			bool        _pruned;
			std::string _name;
		};

		class layer : public baseLayer {
		public:
			std::size_t read(std::istream& file);
			std::size_t readACTN(std::istream& file);

		protected:
			std::size_t readV0(std::istream& file);
			std::size_t readV1(std::istream& file);
			std::size_t readV2(std::istream& file);
			std::size_t readV3(std::istream& file);
			std::size_t readV4(std::istream& file);

			std::size_t readACTNv0(std::istream& file);
			std::size_t readACTNv1(std::istream& file);
			std::size_t readACTNv2(std::istream& file);

		protected:
			uint32_t _version;

			bool _invertBoundaries;
			bool _invertFilters;
			bool _expanded;
			std::string _notes;

			std::vector<layer> _sublayers;
		};

		class boundary : public baseLayer {
		public:
			static bool peekBoundary(std::istream& file);
			std::size_t read(std::istream& file);
		};

		class filter : public baseLayer {
		public:
			static bool peekFilter(std::istream& file);
			std::size_t read(std::istream& file);
		};

		class affector : public baseLayer {
		public:
			static bool peekAffector(std::istream& file);
			std::size_t read(std::istream& file);
		};

	public:
		tgen();
		~tgen();

		std::size_t read(std::istream& file);

	protected:
		uint32_t _tgenVersion;

		sgrp _shaderGroup;
		fgrp _floraGroup;
		rgrp _radialGroup;
		egrp _environmentGroup;
		mgrp _fractalGroup;
		//bgrp _bitmapGroup;

		std::vector<layer> _layers;
	private:
	};
}

#endif
