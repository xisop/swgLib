/** -*-c++-*-
 *  \class  tgenFilter
 *  \file   tgenFilter.hpp
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
#include <memory>
#include <vector>

#include <swgLib/tgenBaseLayer.hpp>
#include <swgLib/mfrc.hpp>

#ifndef TGENFILTER_HPP
#define TGENFILTER_HPP 1

namespace ml
{
	typedef std::shared_ptr<class tgenFilter> tgenFilterPtr;

	class tgenFilter : public tgenBaseLayer {
	public:
		tgenFilter();
		~tgenFilter();

		static bool peekFilter(std::istream& file);
		virtual std::size_t read(std::istream& file);

		static std::size_t read(std::istream& file, tgenFilterPtr& filterPtr);

	protected:
		int32_t _featherFunction;
		float   _featherDistance;
	};

	class filterHeight : public tgenFilter {
	public:
		filterHeight();
		~filterHeight();

		std::size_t read(std::istream& file);

	protected:
		float _low;
		float _high;
	};
	typedef std::shared_ptr<ml::filterHeight> filterHeightPtr;

	class filterFractal : public tgenFilter {
	public:
		filterFractal();
		~filterFractal();

		std::size_t read(std::istream& file);
	protected:
		std::size_t readV0(std::istream& file);
		std::size_t readV1(std::istream& file);
		std::size_t readV2(std::istream& file);
		std::size_t readV3(std::istream& file);
		std::size_t readV4(std::istream& file);
		std::size_t readV5(std::istream& file);

	protected:
		mfrc _multifractal;
		int32_t _familyId;
		float   _scaleVertical;
		float   _lowLimit;
		float   _highLimit;
	};
	typedef std::shared_ptr<ml::filterFractal> filterFractalPtr;

	class filterSlope : public tgenFilter {
	public:
		filterSlope();
		~filterSlope();

		std::size_t read(std::istream& file);

	protected:
		float _minAngle;
		float _maxAngle;
	};
	typedef std::shared_ptr<ml::filterSlope> filterSlopePtr;

	class filterDirection : public tgenFilter {
	public:
		filterDirection();
		~filterDirection();

		std::size_t read(std::istream& file);

	protected:
		float _minAngle;
		float _maxAngle;
	};
	typedef std::shared_ptr<ml::filterDirection> filterDirectionPtr;

	class filterShader : public tgenFilter {
	public:
		filterShader();
		~filterShader();

		std::size_t read(std::istream& file);

	protected:
		int32_t _familyId;
	};
	typedef std::shared_ptr<ml::filterShader> filterShaderPtr;

}

#endif
