/** -*-c++-*-
 *  \class  sht
 *  \file   sht.hpp
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

#include <meshLib/sht.hpp>
#include <meshLib/eft.hpp>
#include <iostream>
#include <cstdlib>
#include <climits>

using namespace ml;

sht::sht()
{
}

sht::~sht()
{
}

unsigned int sht::readSHT( std::istream &file, std::string path )
{
  basePath = path;
    unsigned int total = 0;
    std::string form;
    unsigned int sshtSize;
    std::string type;

    // Reset class
    normalMapUnit = UINT_MAX;
    texTag.clear();
    coordMapping.clear();
    texTag.reserve( 10 );
    coordMapping.reserve( 10 );
    for( unsigned int i = 0; i < 10; ++i )
    {
	coordMapping[i] = UINT_MAX;
    }

    unsigned int position = file.tellg();
    total += readFormHeader( file, form, sshtSize, type );
    sshtSize += 8;
    if( form != "FORM" || type != "SSHT" )
    {
	std::cout << "SSHT not found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found SSHT form"
	      << ": " << sshtSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected FORM: " << form << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    while( total < sshtSize )
    {
	// Peek at next record, but keep file at same place.
	position = file.tellg();
	readFormHeader( file, form, size, type );
	file.seekg( position, std::ios_base::beg );

	if( form == "FORM" )
	{
	    if( type == "MATS" )
	    { total += readMATS( file ); }
	    else if( type == "TXMS" )
	    { total += readTXMS( file ); }
	    else if( type == "TCSS" )
	    { total += readTCSS( file ); }
	    else if( type == "TFNS" )
	    { total += readTFNS( file ); }
	    else if( type == "TSNS" )
	    { total += readTSNS( file ); }
	    else if( type == "ARVS" )
	    { total += readARVS( file ); }
	    else if( type == "EFCT" )
	    {
		eft effect;
		total += effect.readEFT( file, basePath );
	    }
	    else
	    {
		std::cout << "Unexpected form: " << type << std::endl;
		exit( 0 );
	    }
	}
	else if ( form == "NAME" )
	{
	    total += readNAME( file );
	}
	else
	{
	    std::cout << "Unexpected record: " << form << std::endl;
	    exit( 0 );
	}
    }

    if( sshtSize == total )
    {
	std::cout << "Finished reading SSHT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading SSHT" << std::endl;
	std::cout << "Read " << total << " out of " << sshtSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readMATS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int matsSize;
    std::string type;

    total += readFormHeader( file, form, matsSize, type );
    matsSize += 8;
    if( form != "FORM" || type != "MATS" )
    {
	std::cout << "Expected Form of type MATS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << matsSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0000")
    {
	std::cout << "Expected Form of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    unsigned int matsFound = 0;
    while( total < matsSize )
    {
	++matsFound;
	// Read TAG record
	total += readRecordHeader( file, type, size );
	if( type != "TAG " )
	{
	    std::cout << "Expected record of type TAG: " << type << std::endl;
	    exit( 0 );
	}
	std::cout << "Found " << type
		  << ": " << size << " bytes"
		  << std::endl;
	
	file.width( size );
	file >> diffuseTextureTag;
	total += size;
	std::cout << "Material texture tag: " << diffuseTextureTag
		  << std::endl;
	
	// Read MATL record
	total += readRecordHeader( file, type, size );
	if( type != "MATL" )
	{
	    std::cout << "Expected record of type MATL: " << type << std::endl;
	    exit( 0 );
	}
	std::cout << "Found record " << type
		  << ": " << size << " bytes"
		  << std::endl;
	
	if( 68 != size )
	{
	    std::cout << "Expected MATL size 68: " << size << std::endl;
	    exit( 0 );
	}

	file.read( (char *)ambient, sizeof( float ) * 4 );
	file.read( (char *)diffuse, sizeof( float ) * 4 );
	file.read( (char *)specular, sizeof( float ) * 4 );
	file.read( (char *)emissive, sizeof( float ) * 4 );
	file.read( (char *)&shininess, sizeof( float ) );
	total += 17 * sizeof( float );
	
	std::cout << "Ambient: "
		  << ambient[0] << " "
		  << ambient[1] << " "
		  << ambient[2] << " "
		  << ambient[3] << std::endl;
	
	std::cout << "Diffuse: "
		  << diffuse[0] << " "
		  << diffuse[1] << " "
		  << diffuse[2] << " "
		  << diffuse[3] << std::endl;
	
	std::cout << "Specular: "
		  << specular[0] << " "
		  << specular[1] << " "
		  << specular[2] << " "
		  << specular[3] << std::endl;
	
	std::cout << "Emissive: "
		  << emissive[0] << " "
		  << emissive[1] << " "
		  << emissive[2] << " "
		  << emissive[3] << std::endl;
	
	std::cout << "Shininess: " << shininess <<std::endl;
    }

    if( matsFound > 1 ){ std::cout << "*************************"<<std::endl;}
    if( matsSize == total )
    {
	std::cout << "Finished reading MATS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading MATS" << std::endl;
	std::cout << "Read " << total << " out of " << matsSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readTXMS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int txmsSize;
    std::string type;

    total += readFormHeader( file, form, txmsSize, type );
    txmsSize += 8; // Add size of FORM and size fields.
    if( form != "FORM" || type != "TXMS" )
    {
	std::cout << "Expected Form of type TXMS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << txmsSize-12 << " bytes"
	      << std::endl;

    while( total < txmsSize )
    {
	total += readTXM( file );
    }

    if( txmsSize == total )
    {
	std::cout << "Finished reading TXMS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TXMS" << std::endl;
	std::cout << "Read " << total << " out of " << txmsSize
                  << std::endl;
    }


    return total;
}

unsigned int sht::readTXM( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int txmSize;
    std::string type;

    // Read FORM TXM record
    total += readFormHeader( file, form, txmSize, type );
    txmSize += 8;
    if( form != "FORM" || type != "TXM " )
    {
	std::cout << "Expected Form of type TXM: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << txmSize-12 << " bytes"
	      << std::endl;

    // Read FORM 0001 record
    unsigned int size;
    total += readFormHeader( file, form, size, type );
    //if( form != "FORM" || type != "0001" )
    if( form != "FORM" )
    {
	std::cout << "Expected Form of type 0001: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;


    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
        std::cout << "Expected record of type DATA: " << type << std::endl;
        exit( 0 );
    }

    std::string textureTag;
    file.width( 4 );
    file >> textureTag;
    total += 4;

    std::cout << "Texture tag: " << textureTag << std::endl;

#if 0
    // Skip data for now
    for( int i = 0; i < size-4; ++i )
    {
	unsigned char data;
	file.read( (char*)&data, sizeof( data ) );
	std::cout<<std::hex<<"0x"<<(unsigned int)data<<" "<< std::dec;
    }
    std::cout << std::endl;
    total += size-4;
#else
    total += readUnknown( file, size-4 );
#endif
    if( total < txmSize )
    {
	total += readRecordHeader( file, type, size );
	if( type != "NAME" )
	{
	    std::cout << "Expected record of type NAME: " << type << std::endl;
	    exit( 0 );
	}

        char temp[255];
	std::string textureName;
	file.getline( temp, size, 0 );
	textureName = temp;
	total += size;
	for( unsigned int i = 0; i < textureName.size(); ++i )
	{
	    if( textureName[i] == '\\' )
	    {
		textureName[i] = '/';
	    }
	}
	std::string fullTextureName = basePath + textureName;
	std::cout << "Texture name: " << fullTextureName << std::endl;

	if( textureTag == diffuseTextureTag )
	  {
	    diffuseTextureName = fullTextureName;
	  }

	if( textureTag == "NIAM" )
	  {
	    mainTextureName = fullTextureName;
	  }
	else if( textureTag == "LMRN" )
	  {
	    normalTextureName = fullTextureName;
	  }
	else if( textureTag == "3TOD" )
	  {
	    dot3TextureName = fullTextureName;
	  }
	else if( textureTag == "PUKL" )
	  {
	    lookupTextureName = fullTextureName;
	  }
	else if( textureTag == "MVNE" )
	  {
	    environmentTextureName = fullTextureName;
	  }
	else if( textureTag == "KSAM" )
	  {
	    maskTextureName = fullTextureName;
	  }
	else if( textureTag == "BEUH" )
	  {
	    hueTextureName = fullTextureName;
	  }
	else if( textureTag == "CEPS" )
	  {
	    specularTextureName = fullTextureName;
	  }
	else if( textureTag == "MRNC" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "SIME" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "1PLA" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "2PLA" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "3PLA" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "ATED" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "ALTD" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "BLTD" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "TRID" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "DIRI" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "NRCS" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "0PER" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "LACD" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "YKS_" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "EBUC" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else if( textureTag == "PFFM" )
	  {
	    //specularTextureName = fullTextureName;
	  }
	else
	  {
	    std::cout << "Unknown texture tag: " << textureTag << std::endl;
	    exit( 0 );
	  }

#if USE_OSG
	unsigned int currentTexture =
	    static_cast<unsigned int>( texTag.size() );

	if( NULL != geode )
	{

	    osg::Texture2D* texture = new osg::Texture2D;
	    texture->setImage( osgDB::readImageFile( textureName.c_str() ) );
	    texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
	    texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
	    
	    std::cout << "Assigning to texture unit: "
		      << currentTexture
		      << std::endl;

	    // Disable these until I know how to properly implement them.
	    if( textureType == "3TOD" // Dot3
		|| textureType == "LMRN" // Normal
		|| textureType == "PUKL" // Lookup
		|| textureType == "MVNE" // Environment map
		|| textureType == "KSAM" // Mask
		|| textureType == "BEUH" // Hue
		|| textureType == "CEPS" // Specular
		)
	    {
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setTextureAttributeAndModes(
		    currentTexture,
		    texture,
		    osg::StateAttribute::OFF
		    );
	    }
	    else
	    {
		osg::TexEnv *texEnv = new osg::TexEnv;
		texEnv->setMode(osg::TexEnv::MODULATE);

		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setTextureAttributeAndModes(
		    currentTexture,
		    texture,
		    osg::StateAttribute::ON
		    );
		stateset->setTextureAttribute(currentTexture, texEnv );

	    }
	}
#endif
#if 0
	if( textureTag == "LMRN" )
	{
	    std::cout << "Normal texture: " << currentTexture << std::endl;
	    normalMapUnit = currentTexture;
	}
	else if( textureTag == "NIAM" )
	{
	    std::cout << "Diffuse texture: " << currentTexture << std::endl;
	    mainMapUnit = currentTexture;
	}
#endif
	texTag.push_back( textureTag );
    }

    if( txmSize == total )
    {
	std::cout << "Finished reading TXM" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TXM" << std::endl;
	std::cout << "Read " << total << " out of " << txmSize
                  << std::endl;
    }


    return total;
}

unsigned int sht::readTCSS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int tcssSize;
    std::string type;

    total += readFormHeader( file, form, tcssSize, type );
    tcssSize += 8;
    if( form != "FORM" || type != "TCSS" )
    {
	std::cout << "Expected Form of type TCSS: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << tcssSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    unsigned int num = size/5;

    for( unsigned int i = 0; i < num; ++i )
    {
	std::string texName;
	unsigned char texUnit;
	file.width( 4 );
	file >> texName;
	total += 4;
	file.read( (char*)&texUnit, 1 );
	total += 1;

	std::cout << "Texture type: " << texName << " "
		  << "Texture unit: " << (unsigned int)texUnit << std::endl;

	if( texName == "NIAM" )
	  {
	    mainTextureUnit = texUnit;
	  }
	else if( texName == "LMRN" )
	  {
	    normalTextureUnit = texUnit;
	  }
	else if( texName == "3TOD" )
	  {
	    dot3TextureUnit = texUnit;
	  }
	else if( texName == "PUKL" )
	  {
	    lookupTextureUnit = texUnit;
	  }
	else if( texName == "MVNE" )
	  {
	    environmentTextureUnit = texUnit;
	  }
	else if( texName == "KSAM" )
	  {
	    maskTextureUnit = texUnit;
	  }
	else if( texName == "BEUH" )
	  {
	    hueTextureUnit = texUnit;
	  }
	else if( texName == "CEPS" )
	  {
	    specularTextureUnit = texUnit;
	  }
	else if( texName == "MRNC" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "SIME" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "1PLA" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "2PLA" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "3PLA" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "ATED" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "ALTD" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "BLTD" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "TRID" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "DIRI" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "NRCS" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "0PER" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "LACD" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "YKS_" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "EBUC" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else if( texName == "PFFM" )
	  {
	    //specularTextureUnit = texUnit;
	  }
	else
	  {
	    std::cout << "Unknown texture tag: " << texName << std::endl;
	    exit( 0 );
	  }
    }
    
    if( tcssSize == total )
    {
	std::cout << "Finished reading TCSS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TCSS" << std::endl;
	std::cout << "Read " << total << " out of " << tcssSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readTFNS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int tfnsSize;
    std::string type;

    unsigned int position = file.tellg();

    total += readFormHeader( file, form, tfnsSize, type );
    tfnsSize += 8;
    if( form != "FORM" || type != "TFNS" )
    {
	std::cout << "No FORM TFNS found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found " << form << " " << type
	      << ": " << tfnsSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    total += readUnknown( file, size );

    if( tfnsSize == total )
    {
	std::cout << "Finished reading TFNS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TFNS" << std::endl;
	std::cout << "Read " << total << " out of " << tfnsSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readTSNS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int tsnsSize;
    std::string type;

    unsigned int position = file.tellg();

    total += readFormHeader( file, form, tsnsSize, type );
    tsnsSize += 8;
    if( form != "FORM" || type != "TSNS" )
    {
	std::cout << "No FORM TSNS found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found " << form << " " << type
	      << ": " << tsnsSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;

    total += readUnknown( file, size );

    if( tsnsSize == total )
    {
	std::cout << "Finished reading TSNS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading TSNS" << std::endl;
	std::cout << "Read " << total << " out of " << tsnsSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readARVS( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int arvsSize;
    std::string type;

    unsigned int position = file.tellg();

    total += readFormHeader( file, form, arvsSize, type );
    arvsSize += 8;
    if( form != "FORM" || type != "ARVS" )
    {
	std::cout << "No FORM ARVS found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found " << form << " " << type
	      << ": " << arvsSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readRecordHeader( file, type, size );
    if( type != "0000" )
    {
	std::cout << "Expected record of type 0000: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found record " << type
	      << ": " << size << " bytes"
	      << std::endl;


    total += readUnknown( file, size );
    
    if( arvsSize == total )
    {
	std::cout << "Finished reading ARVS" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading ARVS" << std::endl;
	std::cout << "Read " << total << " out of " << arvsSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readEFCT( std::istream &file )
{
    unsigned int total = 0;

    std::string form;
    unsigned int efctSize;
    std::string type;

    unsigned int position = file.tellg();

    total += readFormHeader( file, form, efctSize, type );
    efctSize += 8;
    if( form != "FORM" || type != "EFCT" )
    {
	std::cout << "No FORM EFCT found." << std::endl;
	file.seekg( position, std::ios_base::beg );
	return 0;
    }
    std::cout << "Found " << form << " " << type
	      << ": " << efctSize-12 << " bytes"
	      << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" || type != "0001" )
    {
	std::cout << "Expected FORM of type 0001: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cout << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
    std::cout << "Found record " << type 
	      << ": " << size << " bytes" 
	      << std::endl;

    total += readUnknown( file, size );
    
    if( efctSize == total )
    {
	std::cout << "Finished reading EFCT" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading EFCT" << std::endl;
	std::cout << "Read " << total << " out of " << efctSize
                  << std::endl;
    }

    return total;
}

unsigned int sht::readNAME( std::istream &file )
{
    unsigned int total = 0;

    unsigned int nameSize;
    std::string type;
    unsigned int position = file.tellg();

    // Read Effect file record
    total += readRecordHeader( file, type, nameSize );
    if( type != "NAME" )
    {
        std::cout << "Expected record of type NAME: " << type << std::endl;
	file.seekg( position, std::ios_base::beg );
        return 0;
    }

    char temp[255];
    file.getline( temp, nameSize, 0 );
    effectName = temp;
    total += nameSize;
    for( unsigned int i = 0; i < effectName.size(); ++i )
    {
	if( effectName[i] == '\\' )
	{
	    effectName[i] = '/';
	}
    }

    std::string fullEffectName = basePath + effectName;
    std::cout << "Effect file: " << fullEffectName << std::endl;

#if 0
    std::istream effectFile( fullEffectName.c_str() );
    if( effectFile.is_open() )
    {
        eft effect;
        effect.readEFT( effectFile, basePath );
        effectFile.close();
    }
    else
    {
        std::cout << "Unable to open file: " << fullEffectName << std::endl;
    }
#endif

    if( nameSize == (total-8) )
    {
	std::cout << "Finished reading NAME" << std::endl;
    }
    else
    {
	std::cout << "FAILED in reading NAMESIZE" << std::endl;
	std::cout << "Read " << total << " out of " << nameSize
                  << std::endl;
    }

    return total;
}

void sht::getAmbient( float &r, float &g, float &b, float &a ) const
{
  r = ambient[0];
  g = ambient[1];
  b = ambient[2];
  a = ambient[3];
}

void sht::getDiffuse( float &r, float &g, float &b, float &a ) const
{
  r = diffuse[0];
  g = diffuse[1];
  b = diffuse[2];
  a = diffuse[3];
}

void sht::getSpecular( float &r, float &g, float &b, float &a ) const
{
  r = specular[0];
  g = specular[1];
  b = specular[2];
  a = specular[3];
}

void sht::getEmissive( float &r, float &g, float &b, float &a ) const
{
  r = emissive[0];
  g = emissive[1];
  b = emissive[2];
  a = emissive[3];
}

void sht::getShininess( float &shiny ) const
{
  shiny = shininess;
}

std::string sht::getMaterialName() const
{
  return materialName;
}

void sht::setMaterialName( const std::string newName )
{
  materialName = newName;
}

