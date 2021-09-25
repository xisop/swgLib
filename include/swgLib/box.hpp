/** -*-c++-*-
 *  \class  box
 *  \file   box.hpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2009-2021 Ken Sewell

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

#include <iostream>
#include <cstring>

#ifndef BOX_HPP
#define BOX_HPP 1

namespace ml
{
  class box
  {
  public:
    box():
        x1(0.0f), y1(0.0f), z1(0.0f),
        x2(0.0f), y2(0.0f), z2(0.0f)
    {};
    
    std::size_t readBOX( std::istream &file );
    std::size_t writeBOX( std::ostream &file ) const;

    void getCorner1( float &x, float &y, float &z ) const
    {
      x = x1;
      y = y1;
      z = z1;
    }

    void getCorner2( float &x, float &y, float &z ) const
    {
      x = x2;
      y = y2;
      z = z2;
    }

  protected:
    float x1, y1, z1;
    float x2, y2, z2;

  private:
  };
}
#endif
