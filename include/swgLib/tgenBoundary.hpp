/** -*-c++-*-
 *  \class  tgenBoundary
 *  \file   tgenBoundary.hpp
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
#include <utility>
#include <vector>

#include <swgLib/tgenBaseLayer.hpp>

#ifndef TGENBOUNDARY_HPP
#define TGENBOUNDARY_HPP 1

namespace ml
{
	typedef std::shared_ptr<class tgenBoundary> tgenBoundaryPtr;

	class tgenBoundary : public tgenBaseLayer {
	public:
		tgenBoundary();
		virtual ~tgenBoundary();

		static bool peekBoundary(std::istream& file);
		virtual std::size_t read(std::istream& file);

		static std::size_t read(std::istream& file, tgenBoundaryPtr& boundaryPtr);

	protected:
		int32_t _featherFunction;
		float   _featherDistance;
	};

	class boundaryCircle : public tgenBoundary {
	public:
		boundaryCircle();
		~boundaryCircle();

		std::size_t read(std::istream& file) override;

		const float& getCenterNorth() const;
		const float& getCenterEast() const;
		const float& getRadius() const;

	protected:
		float _centerNorth;
		float _centerEast;
		float _radius;
	};
	typedef std::shared_ptr<ml::boundaryCircle> boundaryCirclePtr;

	// ****************************** Rectangle ******************************

	class boundaryRectangle : public tgenBoundary {
	public:
		boundaryRectangle();
		~boundaryRectangle();

		std::size_t read(std::istream& file) override;

		const float& getWest() const;
		const float& getSouth() const;
		const float& getEast() const;
		const float& getNorth() const;

		bool hasLocalWaterTable() const;
		bool hasLocalGlobalWaterTable() const;
		const float& getLocalWaterTableHeight() const;
		const float& getLocalWaterTableShaderSize() const;
		const std::string& getLocalWaterTableShaderName() const;
		const int32_t& getWaterType() const;

	protected:
		float _west;
		float _south;
		float _east;
		float _north;

		bool        _localWaterTable;
		bool        _localGlobalWaterTable;
		float       _localWaterTableHeight;
		float       _localWaterTableShaderSize;
		std::string _localWaterTableShaderName;
		int32_t     _waterType;
	};
	typedef std::shared_ptr<ml::boundaryRectangle> boundaryRectanglePtr;

	// ****************************** Polygon ******************************

	class boundaryPolygon : public tgenBoundary {
	public:
		boundaryPolygon();
		~boundaryPolygon();

		std::size_t read(std::istream& file) override;

		bool hasLocalWaterTable() const;
		const float& getLocalWaterTableHeight() const;
		const float& getLocalWaterTableShaderSize() const;
		const std::string& getLocalWaterTableShaderName() const;
		const int32_t& getWaterType() const;

		const std::vector<std::pair<float, float>>& getPoints() const;

	protected:
		bool        _localWaterTable;
		float       _localWaterTableHeight;
		float       _localWaterTableShaderSize;
		std::string _localWaterTableShaderName;
		int32_t     _waterType;

		std::vector<std::pair<float, float>> _points;
	};
	typedef std::shared_ptr<ml::boundaryPolygon> boundaryPolygonPtr;

	// ****************************** Polyline ******************************

	class boundaryPolyline : public tgenBoundary {
	public:
		boundaryPolyline();
		~boundaryPolyline();

		std::size_t read(std::istream& file) override;

		const float& getWidth() const;
		const std::vector<std::pair<float, float>>& getPoints() const;

	protected:
		float _width;
		std::vector<std::pair<float, float>> _points;
	};
	typedef std::shared_ptr<ml::boundaryPolyline> boundaryPolylinePtr;

}

#endif
