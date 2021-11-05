/** -*-c++-*-
 *  \class  tgenLayer
 *  \file   tgenLayer.hpp
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

#include <swgLib/tgenBaseLayer.hpp>
#include <swgLib/tgenBoundary.hpp>
#include <swgLib/tgenFilter.hpp>
#include <swgLib/tgenAffector.hpp>

#ifndef TGENLAYER_HPP
#define TGENLAYER_HPP 1

namespace ml
{
	class tgenLayer : public tgenBaseLayer {
	public:
		tgenLayer();
		~tgenLayer();

		std::size_t read(std::istream& file);
		std::size_t readACTN(std::istream& file);

		uint32_t getNumBoundaries() const;
		const std::vector<tgenBoundary>& getBoundaries() const;

		uint32_t getNumFilters() const;
		const std::vector<tgenFilter>& getFilters() const;

		uint32_t getNumAffectors() const;
		const std::vector<tgenAffector>& getAffectors() const;

		uint32_t getNumSublayers() const;
		const std::vector<tgenLayer>& getSublayers() const;

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
		bool _invertBoundaries;
		bool _invertFilters;
		bool _expanded;
		std::string _notes;

		std::vector<tgenBoundaryPtr> _boundaries;
		std::vector<tgenFilterPtr>   _filters;
		std::vector<tgenAffectorPtr> _affectors;
		std::vector<tgenLayer>    _sublayers;
	};
}

#endif
