/** -*-c++-*-
 *  \class  trn
 *  \file   trn.hpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .trn models.
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
#include <map>
#include <cmath>

#include <meshLib/trnAffector.hpp>

#ifndef TRN_HPP
#define TRN_HPP

namespace ml
{
  class trn : public base
  {
  public:
    // Boundary Polygon
    class bpol
    {
    public:
      std::vector< float > x;
      std::vector< float > y;

      unsigned int u1;
      float u2;
      unsigned int u3;
      float altitude;
      float u5;
      std::string name;
    };

    // Boundary Polyline
    class bpln
    {
    public:
      std::vector< float > x;
      std::vector< float > y;

      unsigned int u1;
      float u2;
      float u3;
    };

    // Boundary Circle.
    class bcir
    {
    public:

      bool isInBounds( const float &X,
		       const float &Y ) const
      {
	return ( radiusSqrd > (pow( X-x, 2.0 ) + pow( Y-y, 2.0 )));
      }

      float x;
      float y;
      float radius;
      float radiusSqrd;
      
      unsigned int featherType;
      float featherWidth;
    };

    // Boundary Rectangle.
    class brec
    {
    public:
      bool isInBounds( const float &X,
		       const float &Y ) const
      {
	return( (X >= x1) & (X <= x2) && (Y >= y1) && (Y <= y2) );
      }

      float x1;
      float y1;
      float x2;
      float y2;
      
      unsigned int u1;
      float u2;
      unsigned int u3;
      unsigned int u4;
      float u5;
      float u6;
      std::string name;
    };

    class layer
    {
    public:
      ~layer(){}

      bool isInBounds( const float &X, const float &Y ) const;

      bool apply( const float &originX,
		  const float &originY,
		  const float &spacingX,
		  const float &spacingY,
		  const unsigned int &numRows,
		  const unsigned int &numCols,
		  float *data) const;

      const std::vector<bpol> &getBPOL() const
      {
	return bpolList;
      }
      
      const std::vector<bcir> &getBCIR() const
      {
	return bcirList;
      }
      
      const std::vector<brec> &getBREC() const
      {
	return brecList;
      }
      
      const std::vector<bpln> &getBPLN() const
      {
	return bplnList;
      }

      bool isBounded;
      unsigned int u1;
      std::string name;

      std::vector< trnAffector > affectors;

      std::vector<layer> layerList;

      std::vector<bpol> bpolList;
      std::vector<bpln> bplnList;
      std::vector<bcir> bcirList;
      std::vector<brec> brecList;

    };

    class sfam
    {
    public:
      unsigned int familyNumber;
      std::string name;
      std::string abstract;
      unsigned char rgb[3];
      float u1;
      float u2;

      std::map< std::string, float > shaderMap;
      
    };

    trn();
    ~trn();
    bool isRightType( std::istream &file )
    {
      return isOfType( file, "PTAT" );
    }
    unsigned int readTRN( std::istream &file,
			  const std::string &debugString = "" );

    const float &getTerrainSize() const
    {
      return terrainSize;
    }

  protected:
    unsigned int readTGEN( std::istream &file, const std::string & );
    unsigned int readTRNDATA( std::istream &file, const std::string & );
    unsigned int readMapDATA( std::istream &file, const std::string & );
    unsigned int readWMAP( std::istream &file, const std::string & );
    unsigned int readSMAP( std::istream &file, const std::string & );
    unsigned int readSGRP( std::istream &file, const std::string & );
    unsigned int readFGRP( std::istream &file, const std::string & );
    unsigned int readRGRP( std::istream &file, const std::string & );
    unsigned int readEGRP( std::istream &file, const std::string & );
    unsigned int readMGRP( std::istream &file, const std::string & );
    unsigned int readLYRS( std::istream &file, const std::string & );
    unsigned int readLAYR( std::istream &file,
			   const std::string &,
			   layer &newLayer );

    unsigned int readFFAM( std::istream &file, const std::string & );
    unsigned int readRFAM( std::istream &file, const std::string & );
    unsigned int readEFAM( std::istream &file, const std::string & );
    unsigned int readMFAM( std::istream &file, const std::string & );
    unsigned int readMFRC( std::istream &file, const std::string & );

    unsigned int readACCN( std::istream &file, const std::string & );
    unsigned int readACRF( std::istream &file, const std::string & );
    unsigned int readACRH( std::istream &file, const std::string & );
    unsigned int readADTA( std::istream &file, const std::string & );
    unsigned int readAENV( std::istream &file, const std::string & );
    unsigned int readAEXC( std::istream &file, const std::string & );
    unsigned int readAFDF( std::istream &file, const std::string & );
    unsigned int readAFDN( std::istream &file, const std::string & );
    unsigned int readAFSC( std::istream &file, const std::string & );
    unsigned int readAFSN( std::istream &file, const std::string & );
    unsigned int readAHCN( std::istream &file, const std::string & );
    unsigned int readAHFR( std::istream &file, const std::string &,
			   affectorHeightFractal &newAHFR );
    unsigned int readAHTR( std::istream &file, const std::string & );
    unsigned int readAROA( std::istream &file, const std::string & );
    unsigned int readASCN( std::istream &file, const std::string & );
    unsigned int readASRP( std::istream &file, const std::string & );
    unsigned int readFDIR( std::istream &file, const std::string & );
    unsigned int readFFRA( std::istream &file, const std::string & );
    unsigned int readFHGT( std::istream &file, const std::string & );
    unsigned int readFSHD( std::istream &file, const std::string & );
    unsigned int readFSLP( std::istream &file, const std::string & );
    unsigned int readIHDR( std::istream &file, const std::string &,
			   unsigned int &u1, std::string &name );
    unsigned int readSFAM( std::istream &file, const std::string &,
			   sfam &newSFAM );

    unsigned int readBCIR( std::istream &file, const std::string &, bcir & );
    unsigned int readBPLN( std::istream &file, const std::string &, bpln & );
    unsigned int readBPOL( std::istream &file, const std::string &, bpol & );
    unsigned int readBREC( std::istream &file, const std::string &, brec & );
					  
  private:
    float terrainSize;
    float chunkWidth;
    unsigned int tilesPerChunk;

    float globalWaterTableHeight;
    float globalWaterTableShaderSize;
    std::string globalWaterTableShader;

    float tileSize;

    float collidableFloraMinDistance;
    float collidableFloraMaxDistance;
    float collidableFloraTileSize;
    float collidableFloraTileBorder;
    float collidableFloraSeed;

    float nonCollidableFloraMinDistance;
    float nonCollidableFloraMaxDistance;
    float nonCollidableFloraTileSize;
    float nonCollidableFloraTileBorder;
    float nonCollidableFloraSeed;

    float nearRadialFloraMinDistance;
    float nearRadialFloraMaxDistance;
    float nearRadialFloraTileSize;
    float nearRadialFloraTileBorder;
    float nearRadialFloraSeed;

    float farRadialFloraMinDistance;
    float farRadialFloraMaxDistance;
    float farRadialFloraTileSize;
    float farRadialFloraTileBorder;
    float farRadialFloraSeed;


    unsigned int mapHeight;
    unsigned int mapWidth;

    std::map<unsigned int, sfam> sfamMap;
    std::vector<layer> layerList;

  };
}
#endif
