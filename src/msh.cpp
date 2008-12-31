/** -*-c++-*-
 *  \class  msh
 *  \file   msh.cpp
 *  \author Kenneth R. Sewell III

 meshLib is used for the parsing and exporting .msh models.
 Copyright (C) 2006,2007 Kenneth R. Sewell III

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

#include <meshLib/msh.hpp>
#include <meshLib/cshd.hpp>
#include <meshLib/sht.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>

using namespace ml;

msh::msh()
{
}

msh::~msh()
{
  while( !vertexData.empty() )
    {
      delete vertexData.back();
      vertexData.pop_back();
    }

  while( !indexData.empty() )
    {
      delete indexData.back();
      indexData.pop_back();
    }
}

bool msh::getIndex( unsigned int indexNum,
		    mshVertexData **vData,
		    mshVertexIndex **iData,
		    std::string &shaderFilename
		    )
{
  if( indexNum > indexData.size() )
    {
      std::cout << "Index out of range" << std::endl;
      return false;
    }

  *iData = indexData[indexNum];

  *vData = vertexData[(*iData)->getDataIndex()];

  shaderFilename = shaderList[(*iData)->getDataIndex()];

  return true;
}

bool msh::getMeshIndex( unsigned int indexNum,
			unsigned int &dataSet,
			mshVertexIndex **iData
			)
{
  if( indexNum > indexData.size() )
    {
      std::cout << "Index out of range" << std::endl;
      return false;
    }

  *iData = indexData[indexNum];

  dataSet = (*iData)->getDataIndex();

  return true;
}

bool msh::getIndex( unsigned int indexNum,
		    mshVertexData **vData,
		    std::string &shaderFilename
		    )
{
  if( indexNum > indexData.size() )
    {
      std::cout << "Index out of range" << std::endl;
      return false;
    }

  *vData = vertexData[indexNum];
  shaderFilename = shaderList[indexNum];

  return true;
}

// Some of this works...  Needs more investigating
unsigned int msh::readD3DFVF(
    std::istream &file,
    unsigned int &codes,
    unsigned int &numTex
    )
{
#define D3DFVF_RESERVED0        0x001
#define D3DFVF_XYZRHW           0x004
#define D3DFVF_LASTBETA_UBYTE4   0x1000
#define D3DFVF_LASTBETA_D3DCOLOR 0x8000
    
    file.read( (char*)&codes, sizeof( codes ) );
    std::bitset <32> bs( codes );

    std::cout << "D3D Flexible Vertex Format Bits: ";
    std::cout << bs << std::endl;

    numTex = (codes >> 8) & 0x0f;
    std::cout << " - Num textures: " << numTex << std::endl;

    if( (codes & D3DFVF_XYZRHW) == D3DFVF_XYZRHW )
    {
	std::cout << " - Vertex format includes the position of a transformed"
		  << " vertex." << std::endl;
    }

    if( (codes & D3DFVF_LASTBETA_UBYTE4) == D3DFVF_LASTBETA_UBYTE4 )
    {
	std::cout << " - The last beta field in the vertex position data"
		  << " will be of type UBYTE4." << std::endl;
    }

    if( (codes & D3DFVF_LASTBETA_D3DCOLOR) == D3DFVF_LASTBETA_D3DCOLOR )
    {
	std::cout << " - The last beta field in the vertex position data "
		  << "will be of type D3DCOLOR." << std::endl;
    }

    return (sizeof( codes ));
}

unsigned int msh::readMSH( std::istream &file, std::string path )
{
  basePath = path;
  unsigned int total = 0;
  std::string form;
  unsigned int meshSize;
  std::string type;
  
  total += readFormHeader( file, form, meshSize, type );
  if( form != "FORM" || type != "MESH" )
    {
      std::cout << "Expected Form of type MESH: " << type << std::endl;
      exit( 0 );
    }
  std::cout << "Found MESH form" << std::endl;
  
  unsigned int size;
  total += readFormHeader( file, form, size, type );
  total += readAPPR( file );
  total += readSPS( file );
  
  if( meshSize == (total-8) )
    {
      std::cout << "Finished reading MESH" << std::endl;
    }
  else
    {
      std::cout << "Failed in reading MESH" << std::endl;
    }
  
  return total;
}

unsigned int msh::readAPPR( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int apprSize;
    std::string type;

    total += readFormHeader( file, form, apprSize, type );
    apprSize += 8;
    if( form != "FORM" || type != "APPR" )
    {
	std::cout << "Expected Form of type APPR: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found APPR form" << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0003" )
    {
	std::cout << "Expected Form of type 0003: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found 0003 form" << std::endl;

    total += base::readEXBX( file, cx, cy, cz, radius,
			     x1, y1, z1, x2, y2, z2
			     );

    // Skip next form, size and type...
    file.seekg( apprSize-total, std::ios_base::cur );
    total += apprSize - total;

    if( total == apprSize )
    {
	std::cout << "Finished reading APPR." << std::endl;
    }
    else
    {
	std::cout << "Error reading APPR!" << std::endl;
	std::cout << "Read " << total << " out of " << apprSize
		  << std::endl;
    }
    
    return total;
}

unsigned int msh::readSPS( std::istream &file )
{
    unsigned int total = 0;
    std::string form, type;
    unsigned int spsSize;

    total += readFormHeader( file, form, spsSize, type );
    spsSize += 8; // Size of geode including initial FORM and size.
    if( form != "FORM" || type != "SPS " )
    {
	std::cout << "Expected Form of type SPS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found SPS form" << std::endl;

    // Skip next form, size and type...
    file.seekg( 12, std::ios_base::cur );
    total += 12;

    // Read CNT record
    unsigned int cntSize;
    total += readRecordHeader( file, type, cntSize );
    if( type != "CNT " || cntSize != 4 )
    {
	std::cout << "Expected CNT record with size 4" << std::endl;
	exit( 0 );
    }

    unsigned int numGeode;
    file.read( (char *)&numGeode, sizeof( numGeode ) );
    total += sizeof( numGeode );
    std::cout << "CNT: " << numGeode << std::endl;

    for( unsigned int i = 0; i < numGeode; ++i )
    {
	total += readGeode( file, i+1 );
    }

    if( total == spsSize )
    {
	std::cout << "Finished reading SPS." << std::endl;
    }
    else
    {
	std::cout << "Error reading SPS!" << std::endl;
	std::cout << "Read " << total << " out of " << spsSize
		  << std::endl;
    }

    return total;
}

unsigned int msh::readGeode( std::istream &file,
			     const unsigned int nodeNumber )
{
    unsigned int total = 0;

    std::string type;
    unsigned int geodeSize;
    unsigned int size;
    unsigned int number;

    // Read top level FORM record.
    total += readRecordHeader( file, type, geodeSize );
    geodeSize += 8; // Size of geode including initial FORM and size.
    file.width( 4 );
    file >> number;
    total += sizeof( number );
    if( type != "FORM" || number != nodeNumber )
    {
	std::cout << "Geometry id. Expected: " << nodeNumber
		  << " found: " << number << std::endl;
	exit( 0 );
    }

    // Read Shader file record
    total += readRecordHeader( file, type, size );
    if( type != "NAME" )
    {
	std::cout << "Expected record of type NAME: " << type << std::endl;
	exit( 0 );
    }
    char temp[255];
    std::string shaderName;
    file.getline( temp, size, 0 );
    shaderName = temp;
    total += size;
    for( unsigned int i = 0; i < shaderName.size(); ++i )
    {
	if( shaderName[i] == '\\' )
	{
	    shaderName[i] = '/';
	}
    }
    std::cout << "Shader file: " << shaderName << std::endl;


    // Read INFO record
    total += readRecordHeader( file, type, size );
    if( type != "INFO" || size != 4 )
    {
	std::cout << "Expected record of type INFO: " << type << std::endl;
	exit( 0 );
    }
    unsigned int infoNumber;
    file.read( (char *)&infoNumber, sizeof( infoNumber ) );
    total += sizeof( infoNumber );
    std::cout << "Info: " << infoNumber << std::endl;

    // Load shader and textures...
    std::string fullShaderName = basePath;
    fullShaderName += shaderName;
    shaderList.push_back( shaderName );
#if 0
    std::istream shaderFile( fullShaderName.c_str() );
    if( shaderFile.is_open() )
    {
	cshd *cshader = new cshd;
	sht *shader = new sht;
	
	if( shader->readSHT( shaderFile, basePath ) > 0 )
	{
	  delete cshader;
	  shader->setMaterialName( shaderName );
	  isSHT.push_back( true );
	  shtData.push_back( shader );
	}
	else if( cshader->readCSHD( shaderFile, basePath ) >= 0 )
	{
	  delete shader;
	  cshader->setMaterialName( shaderName );
	  isSHT.push_back( false );
	  cshdData.push_back( cshader );
	}
	shaderFile.close();
    }
    else
    {
        std::cout << "Unable to open file: " << fullShaderName << std::endl;

	// Drop a NULL pointer to keep indexing with vertex vector
	isSHT.push_back( true );
	shtData.push_back( NULL );
    }
#endif

    while( total < geodeSize )
    {
	total += readGeometry( file );
    }

#if 0
    if( UINT_MAX != shader.getNormalMapTextureUnit() )
    {
	osgFX::BumpMapping *bmap = new osgFX::BumpMapping;
	bmap->addChild( geode );
	bmap->setLightNumber( 0 );
	bmap->setEnabled( false );
        bmap->setNormalMapTextureUnit( shader.getNormalMapTextureUnit() );
	bmap->setDiffuseTextureUnit( shader.getMainTextureUnit() );
	group->addChild( bmap );
    }
    else
    {
	group->addChild( geode );
    }
#endif

    if( total == geodeSize )
    {
	std::cout << "Finished reading Geode." << std::endl;
    }
    else
    {
	std::cout << "Error reading Geode!" << std::endl;
	std::cout << "Read " << total << " out of " << geodeSize
		  << std::endl;
    }

    return total;
}

unsigned int msh::readGeometry( std::istream &file )
{
    unsigned int total = 0;

    std::string type;
    unsigned int geometrySize;
    unsigned int size;

    // Read top level FORM record.
    std::string form;
    total += readFormHeader( file, form, geometrySize, type );
    geometrySize += 8; // Size of geometry including initial FORM and size.
    if( form != "FORM" )
    {
	std::cout << "Expected record of type FORM: " << form << std::endl;
	exit( 0 );
    }

    // Read INFO record
    total += readRecordHeader( file, type, size );
    if( type != "INFO" || size != 6 )
    {
	std::cout << "Expected INFO record of size 8: " << size << std::endl;
	exit( 0 );
    }
    file.seekg( size, std::ios_base::cur );
    total += size;

    // Read VTXA FORM record.
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "VTXA" )
    {
	std::cout << "Expected FORM of type VTXA: " << type << std::endl;
	exit( 0 );
    }

    // Read next level FORM record.
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected record of type FORM: " << form << std::endl;
	exit( 0 );
    }

    // Read INFO record
    total += readRecordHeader( file, type, size );
    if( type != "INFO" || size != 8 )
    {
	std::cout << "Expected INFO record of size 8: " << size << std::endl;
	exit( 0 );
    }

    unsigned int d3dfvfCodes;
    unsigned int numTextures;
    total += readD3DFVF( file, d3dfvfCodes, numTextures );

    unsigned int numVerts;
    file.read( (char *)&numVerts, sizeof( numVerts ) );
    total += sizeof( numVerts );
    std::cout << "Num Vertices: " << numVerts << std::endl;
    
    total += readGeometryDATA( file, numVerts );

    unsigned int bytesPerIndex = 0;
    total += readGeometryINDX( file, bytesPerIndex );

    if( total != geometrySize )
    {
	total += readGeometrySIDX( file, bytesPerIndex );
    }

    if( total == geometrySize )
    {
	std::cout << "Finished reading Geometry." << std::endl;
    }
    else
    {
	std::cout << "Error reading Geometry!" << std::endl;
	std::cout << "Read " << total << " out of " << geometrySize
		  << std::endl;
    }


    return total;
}

unsigned int msh::readGeometrySIDX( std::istream &file, unsigned int bytesPerIndex )
{
    std::string type;
    unsigned int size;
    // Read SIDX record
    readRecordHeader( file, type, size );
    if( type != "SIDX" )
    {
	std::cout << "Expected SIDX record not: " << type << std::endl;
	return 0;
    }
    std::cout << "Reading SIDX record." << std::endl;
    std::cout << "size: " << size << std::endl;

    unsigned int num;
    file.read( (char *)&num, sizeof( num ) );
    std::cout << "Num matrix/index/triangle sets: " << num << std::endl;
    
    for( unsigned int j = 0; j < num; ++j )
    {
      float rx, ry, rz;
      file.read( (char *)&rx, sizeof( rx ) );
      std::cout << "X rotation?: " << rx << std::endl;
      file.read( (char *)&ry, sizeof( ry ) );
      std::cout << "Y rotation?: " << ry << std::endl;
      file.read( (char *)&rz, sizeof( rz ) );
      std::cout << "Z rotation?: " << rz << std::endl;
      
      unsigned int numIndex;
      file.read( (char *)&numIndex, sizeof( numIndex ) );
      std::cout << "Num index: " << numIndex << std::endl;
      std::cout << "Num triangles: " << numIndex/3 << std::endl;
      
      std::cout << "Bytes per index: " << bytesPerIndex << std::endl;
      
      mshVertexIndex *mvi = new mshVertexIndex( bytesPerIndex );
      
      mvi->read( file, numIndex );
      
      if( vertexData.size() > 0 )
	{
	  std::cout << "Index data: " << (vertexData.size()-1) << std::endl;
	  mvi->setShaderIndex( shaderList.size() -1 );
	  mvi->setDataIndex( vertexData.size()-1 );
	}
      
      indexData.push_back( mvi );
    }
    
    return size+8;
}

unsigned int msh::readGeometryINDX( std::istream &file, unsigned int &bytesPerIndex )
{
    std::string type;
    unsigned int size;

    // Read INDX record
    readRecordHeader( file, type, size );
    if( type != "INDX" )
    {
	std::cout << "Expected INDX record not: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Reading INDX record." << std::endl;

    std::cout << "size: " << size << std::endl;

#if 1
    unsigned int numIndex;
    file.read( (char *)&numIndex, sizeof( numIndex ) );
    std::cout << "Num index: " << numIndex << std::endl;
    std::cout << "Num triangles: " << numIndex/3 << std::endl;

    bytesPerIndex = (size-4)/numIndex;
    std::cout << "Bytes per index: " << bytesPerIndex << std::endl;

    mshVertexIndex *mvi = new mshVertexIndex( bytesPerIndex );

    mvi->read( file, numIndex );

    if( vertexData.size() > 0 )
      {
	std::cout << "Index data: " << (vertexData.size()-1) << std::endl;
	mvi->setShaderIndex( shaderList.size() -1 );
	mvi->setDataIndex( vertexData.size()-1 );
      }

    indexData.push_back( mvi );

#else
    file.seekg( size, std::ios_base::cur );
#endif

    // Return size + size of FORM/size data.
    return size+8;
}


unsigned int msh::readGeometryDATA( std::istream &file,
				    unsigned int numVerts )
{
    std::string type;
    unsigned int size;

    // Read DATA record
    readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cout << "Expected DATA record not: " << type << std::endl;
	exit( 0 );
    }

    unsigned int bytesPerVertex = size/numVerts;

    std::cout << std::fixed;
    std::cout << "Bytes per vertex: " << bytesPerVertex << std::endl;

    if( mshVertexData::isSupportedSize( bytesPerVertex ) )
      {
	std::cout << "Vertex size is supported" << std::endl;
	ml::mshVertexData *vData = new mshVertexData;
	vData->setBytesPerVertex( bytesPerVertex );
	vData->read( file, numVerts );
	//vData->print();
	vertexData.push_back( vData );
      }
    else
      {
	std::cerr << "Unsupported vertex size: "
		  << bytesPerVertex
		  << std::endl;
	file.seekg( size, std::ios_base::cur );
      }

#if 0
    if( 32 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec3Array* normals = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x, y, z;
	float nx, ny, nz;
	float s, t;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    std::cout << x << " ";
	    file.read( (char *)&y, sizeof( y ) );
	    std::cout << y << " ";
	    file.read( (char *)&z, sizeof( z ) );
	    std::cout << z << " ";
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    std::cout << nx << " ";
	    file.read( (char *)&ny, sizeof( ny ) );
	    std::cout << ny << " ";
	    file.read( (char *)&nz, sizeof( nz ) );
	    std::cout << nz << " ";
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << std::endl;
	    texcoords->push_back(osg::Vec2(s, t));

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);

	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( unsigned int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    else if( 36 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec3Array* normals = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec4Array* colors = new osg::Vec4Array;

	float x, y, z;
	float nx, ny, nz;
	float s, t;
	unsigned char r, g, b, a;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    std::cout << x << " ";
	    file.read( (char *)&y, sizeof( y ) );
	    std::cout << y << " ";
	    file.read( (char *)&z, sizeof( z ) );
	    std::cout << z << " ";
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    std::cout << nx << " ";
	    file.read( (char *)&ny, sizeof( ny ) );
	    std::cout << ny << " ";
	    file.read( (char *)&nz, sizeof( nz ) );
	    std::cout << nz << " ";
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&r, sizeof( r ) );
	    std::cout << (unsigned int)r << " ";
	    file.read( (char *)&g, sizeof( g ) );
	    std::cout << (unsigned int)g << " ";
	    file.read( (char *)&b, sizeof( b ) );
	    std::cout << (unsigned int)b << " ";
	    file.read( (char *)&a, sizeof( a ) );
	    std::cout << (unsigned int)a << " ";
	    colors->push_back(
		osg::Vec4( r/255.0, g/255.0, b/255.0, a/255.0 )
		);

	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << " " << std::endl;
	    texcoords->push_back( osg::Vec2( s, t ) );
	}

	geom->setVertexArray(coords);

	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX );

	for( unsigned int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    else if( 40 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec3Array* normals = new osg::Vec3Array;
	osg::Vec2Array* detacoords = new osg::Vec2Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x, y, z;
	float nx, ny, nz;
	float s, t;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    std::cout << x << " ";
	    file.read( (char *)&y, sizeof( y ) );
	    std::cout << y << " ";
	    file.read( (char *)&z, sizeof( z ) );
	    std::cout << z << " ";
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    std::cout << nx << " ";
	    file.read( (char *)&ny, sizeof( ny ) );
	    std::cout << ny << " ";
	    file.read( (char *)&nz, sizeof( nz ) );
	    std::cout << nz << " ";
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << " ";
	    texcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << std::endl;
	    detacoords->push_back(osg::Vec2(s, t));

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);
	
	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( unsigned int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 1 )
	    {
		geom->setTexCoordArray(i, detacoords);
		std::cout << "Assigned tex coords 1 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    else if( 44 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec3Array* normals = new osg::Vec3Array;
	osg::Vec2Array* detacoords = new osg::Vec2Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x, y, z;
	float nx, ny, nz;
	float s, t;
	unsigned char r, g, b, a;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    std::cout << x << " ";
	    file.read( (char *)&y, sizeof( y ) );
	    std::cout << y << " ";
	    file.read( (char *)&z, sizeof( z ) );
	    std::cout << z << " ";
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    std::cout << nx << " ";
	    file.read( (char *)&ny, sizeof( ny ) );
	    std::cout << ny << " ";
	    file.read( (char *)&nz, sizeof( nz ) );
	    std::cout << nz << " ";
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&r, sizeof( r ) );
	    std::cout << (unsigned int)r << " ";
	    file.read( (char *)&g, sizeof( g ) );
	    std::cout << (unsigned int)g << " ";
	    file.read( (char *)&b, sizeof( b ) );
	    std::cout << (unsigned int)b << " ";
	    file.read( (char *)&a, sizeof( a ) );
	    std::cout << (unsigned int)a << " ";
	    colors->push_back(
		osg::Vec4( r/255.0, g/255.0, b/255.0, a/255.0 )
		);

	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << " ";
	    texcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << std::endl;
	    detacoords->push_back(osg::Vec2(s, t));

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);
	
	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( unsigned int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 1 )
	    {
		geom->setTexCoordArray(i, detacoords);
		std::cout << "Assigned tex coords 1 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    else if( 48 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec3Array* normals = new osg::Vec3Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x, y, z;
	float nx, ny, nz;
	float s, t;
	float tx, ty, tz;
	float junk;
	std::cout.width( 8 );
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    // Vertex xyz
	    file.read( (char *)&x, sizeof( x ) );
	    std::cout << x << " ";
	    file.read( (char *)&y, sizeof( y ) );
	    std::cout << y << " ";
	    file.read( (char *)&z, sizeof( z ) );
	    std::cout << z << " ";
	    coords->push_back(osg::Vec3(x, y, z));

	    // Normal vector xyz
	    file.read( (char *)&nx, sizeof( nx ) );
	    std::cout << nx << " ";
	    file.read( (char *)&ny, sizeof( ny ) );
	    std::cout << ny << " ";
	    file.read( (char *)&nz, sizeof( nz ) );
	    std::cout << nz << " ";
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    // Texture coords st
	    file.read( (char *)&s, sizeof( s ) );
	    std::cout << s << " ";
	    file.read( (char *)&t, sizeof( t ) );
	    std::cout << t << " ";
	    texcoords->push_back(osg::Vec2(s, t));

	    // Tangent vector xyz?
	    file.read( (char *)&tx, sizeof( tx ) );
	    std::cout << tx << " ";
	    file.read( (char *)&ty, sizeof( ty ) );
	    std::cout << ty << " ";
	    file.read( (char *)&tz, sizeof( tz ) );
	    std::cout << tz << " ";

	    // Unknown
	    file.read( (char *)&junk, sizeof( junk ) );
	    std::cout << junk << std::endl;

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);
	
	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( unsigned int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    else if( 56 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec2Array* detacoords = new osg::Vec2Array;
	osg::Vec2Array* detbcoords = new osg::Vec2Array;
	osg::Vec3Array* normals = new osg::Vec3Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x1;
	float x, y, z;
	float nx, ny, nz;
	float s, t;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    file.read( (char *)&y, sizeof( y ) );
	    file.read( (char *)&z, sizeof( z ) );
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    file.read( (char *)&ny, sizeof( ny ) );
	    file.read( (char *)&nz, sizeof( nz ) );
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    texcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detacoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detbcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&x1, sizeof( x1 ) );
	    std::cout << x1 << " ";
	    file.read( (char *)&x1, sizeof( x1 ) );
	    std::cout << x1 << std::endl;

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);

	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 1 )
	    {
		geom->setTexCoordArray(i, detacoords);
		std::cout << "Assigned tex coords 1 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 2 )
	    {
		geom->setTexCoordArray(i, detbcoords);
		std::cout << "Assigned tex coords 2 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    // Found in appearance/mesh/ply_nboo_garden_lrg_s01_l1.msh
    else if( 64 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec2Array* detacoords = new osg::Vec2Array;
	osg::Vec2Array* detbcoords = new osg::Vec2Array;
	osg::Vec2Array* detccoords = new osg::Vec2Array;
	osg::Vec3Array* normals = new osg::Vec3Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x1;
	float x, y, z;
	float nx, ny, nz;
	float s, t;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    file.read( (char *)&y, sizeof( y ) );
	    file.read( (char *)&z, sizeof( z ) );
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    file.read( (char *)&ny, sizeof( ny ) );
	    file.read( (char *)&nz, sizeof( nz ) );
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    texcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detacoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detbcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detccoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&x1, sizeof( x1 ) );
	    std::cout << x1 << " ";
	    file.read( (char *)&x1, sizeof( x1 ) );
	    std::cout << x1 << std::endl;

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);

	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 1 )
	    {
		geom->setTexCoordArray(i, detacoords);
		std::cout << "Assigned tex coords 1 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 2 )
	    {
		geom->setTexCoordArray(i, detbcoords);
		std::cout << "Assigned tex coords 2 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 3 )
	    {
		geom->setTexCoordArray(i, detccoords);
		std::cout << "Assigned tex coords 3 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    // Found in appearance/mesh/thm_dant_jedi_main_l0.msh
    else if( 72 == bytesPerVertex )
    {
	osg::Vec3Array* coords = new osg::Vec3Array;
	osg::Vec2Array* texcoords = new osg::Vec2Array;
	osg::Vec2Array* detacoords = new osg::Vec2Array;
	osg::Vec2Array* detbcoords = new osg::Vec2Array;
	osg::Vec2Array* detccoords = new osg::Vec2Array;
	osg::Vec2Array* detdcoords = new osg::Vec2Array;
	osg::Vec3Array* normals = new osg::Vec3Array;

	osg::Vec4Array* colors = new osg::Vec4Array;

	float x1;
	float x, y, z;
	float nx, ny, nz;
	float s, t;
	for( unsigned int i = 0; i < numVerts; ++i )
	{
	    file.read( (char *)&x, sizeof( x ) );
	    file.read( (char *)&y, sizeof( y ) );
	    file.read( (char *)&z, sizeof( z ) );
	    coords->push_back(osg::Vec3(x, y, z));

	    file.read( (char *)&nx, sizeof( nx ) );
	    file.read( (char *)&ny, sizeof( ny ) );
	    file.read( (char *)&nz, sizeof( nz ) );
	    normals->push_back(osg::Vec3(nx, ny, nz));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    texcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detacoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detbcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detccoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&s, sizeof( s ) );
	    file.read( (char *)&t, sizeof( t ) );
	    detdcoords->push_back(osg::Vec2(s, t));

	    file.read( (char *)&x1, sizeof( x1 ) );
	    std::cout << x1 << " ";
	    file.read( (char *)&x1, sizeof( x1 ) );
	    std::cout << x1 << std::endl;

	    colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	}

	geom->setVertexArray(coords);

	geom->setNormalArray(normals);
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->setColorArray( colors );
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	for( int i = 0; i < 10; ++i )
	{
	    if( shader.coordMapping[i] == 0 )
	    {
		geom->setTexCoordArray(i, texcoords);
		std::cout << "Assigned tex coords 0 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 1 )
	    {
		geom->setTexCoordArray(i, detacoords);
		std::cout << "Assigned tex coords 1 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 2 )
	    {
		geom->setTexCoordArray(i, detbcoords);
		std::cout << "Assigned tex coords 2 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 3 )
	    {
		geom->setTexCoordArray(i, detccoords);
		std::cout << "Assigned tex coords 3 to tex unit "
			  << i << std::endl;
	    }
	    else if( shader.coordMapping[i] == 4 )
	    {
		geom->setTexCoordArray(i, detdcoords);
		std::cout << "Assigned tex coords 4 to tex unit "
			  << i << std::endl;
	    }
	}
    }
    else
    {
	file.seekg( size, std::ios_base::cur );
    }
#endif

    // Return size + size of FORM/size data.
    return size+8;
}

