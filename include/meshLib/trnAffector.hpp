/** -*-c++-*-
 *  \class  trnAffector
 *  \file   trnAffector.hpp
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
#include <meshLib/base.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <map>

#ifndef TRNAFFECTOR_HPP
#define TRNAFFECTOR_HPP

namespace ml
{
  class trnAffector
  {
  public:
    trnAffector();
    virtual ~trnAffector();
    virtual void apply( const float &currentX,
			const float &currentY,
			float &data) const;
  };

  class affectorHeightFractal : public trnAffector
  {
  public:
    affectorHeightFractal();
    ~affectorHeightFractal();
    void apply( const float &currentX,
		const float &currentY,
		float &data) const;
    
    void setFractalIndex( const unsigned int &index )
    {
      fractalIndex = index;
    }

    unsigned int &getFractalIndex()
    {
      return fractalIndex;
    }

    void setUnknown1( const unsigned int &unknown )
    {
      u1 = unknown;
    }

    unsigned int &getUnknown1()
    {
      return u1;
    }

    void setHeight( const float &h )
    {
      height = h;
    }

    float & getHeight()
    {
      return height;
    }

  protected:
    unsigned int fractalIndex;
    unsigned int u1;
    float height;
    
  };


}
#endif
