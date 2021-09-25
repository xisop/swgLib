/** -*-c++-*-
 *  \class  cach
 *  \file   cach.hpp
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

#ifndef CACH_HPP
#define CACH_HPP

namespace ml
{
  
  class cach : public base
  {
  public:
    cach();
    ~cach();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "CACH" );
    }
    std::size_t readCACH( std::istream &file, std::string path="" );

    const std::vector<std::string> &getCacheVector() const
    {
      return cacheList;
    }

  protected:
    std::vector<std::string> cacheList;

  private:

  };
}
#endif
