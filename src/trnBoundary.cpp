/** -*-c++-*-
 *  \class  trnBoundary
 *  \file   trnBoundary.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .trn models.
 Copyright (C) 2009 Kenneth R. Sewell III

 This file is part of meshLib.

 meshLib is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 meshLib is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with meshLib; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <meshLib/trnBoundary.hpp>
#include <cmath>
using namespace ml;

trnBoundary::trnBoundary()
{
}

trnBoundary::~trnBoundary()
{
}

bool trnBoundary::isInBounds( const float &X,
			      const float &Y ) const
{
  return false;
}

// Boundary Polygon
boundaryPolygon::boundaryPolygon()
{
}

boundaryPolygon::~boundaryPolygon()
{
}

// Boundary Polyline
boundaryPolyline::boundaryPolyline()
{
}

boundaryPolyline::~boundaryPolyline()
{
}

// Boundary Circle.
boundaryCircle::boundaryCircle()
{
}

boundaryCircle::~boundaryCircle()
{
}

bool boundaryCircle::isInBounds( const float &X,
				 const float &Y ) const
{
  return ( radiusSqrd > (std::pow( X-x, 2.0 ) + std::pow( Y-y, 2.0 )));
}

// Boundary Rectangle.
boundaryRectangle::boundaryRectangle()
{
}

boundaryRectangle::~boundaryRectangle()
{
}

bool boundaryRectangle::isInBounds( const float &X,
				    const float &Y ) const
{
  return( (X >= x1) & (X <= x2) && (Y >= y1) && (Y <= y2) );
}
