/** -*-c++-*-
 *  \class  skmg
 *  \file   skmg.hpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006-2009 Kenneth R. Sewell III

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

#ifndef SKMG_HPP
#define SKMG_HPP

namespace ml
{
  class skmg : public base
  {
  public:
    skmg();
    ~skmg();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "SKMG" );
    }
    unsigned int readSKMG( std::istream &file );
    void print() const;

  protected:
    unsigned int readINFO( std::istream &file );

    unsigned int readSKTM( std::istream &file );
    unsigned int readXFNM( std::istream &file );
    unsigned int readPOSN( std::istream &file );
    unsigned int readTWHD( std::istream &file );
    unsigned int readTWDT( std::istream &file );
    unsigned int readNORM( std::istream &file );
    unsigned int readDOT3( std::istream &file );
    unsigned int readPSDT( std::istream &file );
    unsigned int readNAME( std::istream &file );
    unsigned int readBLTS( std::istream &file );
    unsigned int readBLT( std::istream &file );
    unsigned int readBLTPOSN( std::istream &file, unsigned int num );
    unsigned int readBLTNORM( std::istream &file, unsigned int num );
    unsigned int readOZN( std::istream &file );
    unsigned int readFOZC( std::istream &file );
    unsigned int readOZC( std::istream &file );
    unsigned int readZTO( std::istream &file );
    unsigned int readBLTINFO( std::istream &file,
			      unsigned int &numBLTPos,
			      unsigned int &numBLTNorm
			      );
    unsigned int readDOT3Index( std::istream &file );

    unsigned int readPIDX( std::istream &file );
    unsigned int readNIDX( std::istream &file );
    unsigned int readTXCI( std::istream &file );
    unsigned int readTCSF( std::istream &file );
    unsigned int readTCSD( std::istream &file );
    unsigned int readPRIM( std::istream &file );
    unsigned int readPRIMINFO( std::istream &file );
    unsigned int readITL( std::istream &file );
    unsigned int readOITL( std::istream &file );

  private:
    std::string skeletonFilename;
    std::string shaderFilename;

    unsigned int numBones;
    unsigned int numPoints;
    unsigned int numNorm;
    unsigned int numDot3;
    unsigned int numTwdt;
    unsigned int numIndex;
    unsigned int numBLT;

    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;

    std::vector<float> nx;
    std::vector<float> ny;
    std::vector<float> nz;
  };
}
#endif
