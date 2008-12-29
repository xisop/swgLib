/** -*-c++-*-
 *  \class  ws
 *  \file   ws.cpp
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

#include <meshLib/ws.hpp>

#include <iostream>
#include <cstdlib>

#define RAD2DEG (180.0/3.141592653589793116)
#define DEG2RAD (3.141592653589793116/180.0)

using namespace ml;

ws::ws()
{
}

ws::~ws()
{
}

unsigned int ws::readMetaFile( std::istream &infile )
{
  char temp[512];
  maxObjectIndex = 0;

  while( !infile.eof() )
    {
      wsNode node;

      // Node ID
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.nodeID;
      //std::cout << node.nodeID << std::endl;
      
      // Parent Node ID
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.parentNodeID;
      //std::cout << node.parentNodeID << std::endl;
      
      // Object Index
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.objectIndex;
      //std::cout << node.objectIndex << std::endl;

      // Object filename
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.objectFilename;
      //std::cout << node.objectFilename << std::endl;
      
      // Position in parent
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.positionInParent;
      //std::cout << node.positionInParent << std::endl;

      // rx
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.rx;
      node.rx *= DEG2RAD;
      //std::cout << node.rx << std::endl;

      // ry
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.ry;
      node.ry *= DEG2RAD;
      //std::cout << node.ry << std::endl;

      // rz
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.rz;
      node.rz *= DEG2RAD;
      //std::cout << node.rz << std::endl;

      // Unknown 1
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.u1;
      //std::cout << node.u1 << std::endl;

      // x
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.x;
      //std::cout << node.x << std::endl;

      // y
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.y;
      //std::cout << node.y << std::endl;

      // z
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.z;
      //std::cout << node.z << std::endl;

      // Unknown 2
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.u2;
      //std::cout << node.u2 << std::endl;

      // Unknown 3
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.crc;
      //std::cout << node.crc << std::endl;

      // level
      infile.getline( temp, 512, ':' );
      if( infile.eof() ) { break; };
      infile >> node.level;
      //std::cout << node.level << std::endl;

      if( node.objectIndex > maxObjectIndex )
	{
	  maxObjectIndex = node.objectIndex;
	}

      nodes.push_back( node );
    }

  return 0;
}

unsigned int ws::createWS( std::ofstream &outfile )
{

    unsigned int total = 0;

    // Write form with dummy size
    unsigned int form0Position = outfile.tellp();
    writeFormHeader( outfile, "FORM", 0, "WSNP" );
    // Write form with dummy size
    unsigned int form1Position = outfile.tellp();
    writeFormHeader( outfile, "FORM", 0, "0001" );

    total += writeNODS( outfile );
    total += writeOTNL( outfile );

    // Rewrite form with proper size.
    outfile.seekp( form1Position, std::ios_base::beg );
    total += writeFormHeader( outfile, "FORM", total+4, "0001" );

    // Rewrite form with proper size.
    outfile.seekp( form0Position, std::ios_base::beg );
    total += writeFormHeader( outfile, "FORM", total+4, "WSNP" );

    return total;
}

unsigned int ws::readWS( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int wsSize;
    std::string type;

    total += readFormHeader( file, form, wsSize, type );
    wsSize += 8;
    if( form != "FORM" || type != "WSNP" )
    {
	std::cout << "Expected Form of type WSNP: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found WSNP form"
	      << ": " << wsSize-12 << " bytes"
	      << std::endl;
#endif
    std::cout << "File type: " << type << std::endl;

    unsigned int size;
    total += readFormHeader( file, form, size, type );
    if( form != "FORM" )
    {
	std::cout << "Expected FORM: " << form << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found " << form << " " << type
	      << ": " << size-4 << " bytes"
	      << std::endl;
#endif

    total += readNODS( file );
    total += readOTNL( file );


    std::list< wsNode >::iterator node;
    for( node = nodes.begin(); node != nodes.end(); ++node )
    {
	node->objectFilename = objectNames[node->objectIndex];
	node->print();
    }


    if( wsSize == total )
    {
#if DEBUG
	sd::cout << "Finished reading WS" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading WS" << std::endl;
	std::cout << "Read " << total << " out of " << wsSize
                  << std::endl;
    }
    
    return total;
}

unsigned int ws::writeNODS( std::ofstream &outfile )
{
    unsigned int total = 0;

    //FORM NODS
    // Write form with dummy size
    unsigned int form0Position = outfile.tellp();
    writeFormHeader( outfile, "FORM", 0, "NODS" );

    // Start writing nodes from beginning of list
    currentNode = nodes.begin();
    while( currentNode != nodes.end()  )
    {
	total += writeNODE( outfile );
    }

    unsigned int nodeEndPosition = outfile.tellp();

    //FORM NODS
    outfile.seekp( form0Position, std::ios_base::beg );
    total += writeFormHeader( outfile, "FORM", total+4, "NODS" );

    outfile.seekp( nodeEndPosition, std::ios_base::beg );

    return total;
}

unsigned int ws::readNODS( std::istream &file )
{
    unsigned int total = 0;
    std::string form;
    unsigned int nodsSize;
    std::string type;

    total += readFormHeader( file, form, nodsSize, type );
    nodsSize += 8;
    if( form != "FORM" || type != "NODS" )
    {
	std::cout << "Expected Form of type NODS: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found NODS form"
	      << ": " << nodsSize-12 << " bytes"
	      << std::endl;
#endif
    
    unsigned int numNodes = 0;
    while( total < nodsSize )
    {
	total += readNODE( file, 0 );
	++numNodes;
    }

#if DEBUG
    std::cout << "Number of nodes found: " << numNodes << std::endl;
#endif

    if( nodsSize == total )
    {
#if DEBUG
	std::cout << "Finished reading NODS" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading NODS" << std::endl;
	std::cout << "Read " << total << " out of " << nodsSize
                  << std::endl;
    }
    
    return total;
}

unsigned int ws::writeNODE( std::ofstream &outfile )
{
    unsigned int total = 0;

    // FORM NODE
    // Write form with dummy size
    unsigned int form0Position = outfile.tellp();
    writeFormHeader( outfile, "FORM", 0, "NODE" );

    // FORM 0000
    // Write form with dummy size
    unsigned int form1Position = outfile.tellp();
    writeFormHeader( outfile, "FORM", 0, "0000" );

    total += writeRecordHeader( outfile, "DATA", 52 );
    currentNode->print();
    total += currentNode->write( outfile );
    unsigned int thisLevel = currentNode->level;
    ++currentNode;

    if( currentNode != nodes.end() )
      {
	while( currentNode->level > thisLevel )
	  {
	    total += writeNODE( outfile );
	  }
      }

    unsigned int nodeEndPosition = outfile.tellp();

    // FORM 0000
    // Rewrite form with proper size.
    outfile.seekp( form1Position, std::ios_base::beg );
    total += writeFormHeader( outfile, "FORM", total+4, "0000" );

    // FORM NODE
    // Rewrite form with proper size.
    outfile.seekp( form0Position, std::ios_base::beg );
    total += writeFormHeader( outfile, "FORM", total+4, "NODE" );

    outfile.seekp( nodeEndPosition, std::ios_base::beg );

    return total;
}

unsigned int ws::readNODE( std::istream &file, unsigned int level )
{
    unsigned int total = 0;
    std::string form;
    unsigned int nodeSize;
    std::string type;

    // FORM NODE
    total += readFormHeader( file, form, nodeSize, type );
    nodeSize += 8;
    if( form != "FORM" || type != "NODE" )
    {
	std::cout << "Expected Form of type NODE: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found NODE form"
	      << ": " << nodeSize-12 << " bytes"
	      << std::endl;
#endif

    // FORM 0000
    unsigned int size;
    total += readFormHeader( file, form, size, type );
    size += 8;
    if( form != "FORM" || type != "0000" )
    {
	std::cout << "Expected Form of type 0000: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found 0000 form"
	      << ": " << size-12 << " bytes"
	      << std::endl;
#endif

    total += readRecordHeader( file, type, size );
    if( type != "DATA" )
    {
	std::cout << "Expected record of type DATA: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found DATA record"
	      << ": " << size << " bytes"
	      << std::endl;
#endif

    if( size != 52 )
    {
	std::cout << "Expected size of 52: " << size << std::endl;
	exit( 0 );
    }

    wsNode node;
    total += node.read( file );
    node.level = level;
    nodes.push_back( node );

    while( total < nodeSize )
    {
	total += readNODE( file, level+1 );
    }

    if( nodeSize == total )
    {
#if DEBUG
	std::cout << "Finished reading NODE" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading NODE" << std::endl;
	std::cout << "Read " << total << " out of " << nodeSize
                  << std::endl;
    }
    
    return total;
}

unsigned int ws::writeOTNL( std::ofstream &outfile )
{
  ++maxObjectIndex;
  std::cout << "Resizing vector to " << maxObjectIndex << std::endl;
  objectNames.resize( maxObjectIndex );
  for( currentNode = nodes.begin(); currentNode != nodes.end();
       ++currentNode )
    {
      objectNames[currentNode->objectIndex] = currentNode->objectFilename;
    }
  
  unsigned int total = 0;

  // Write form with dummy size
  unsigned int position = outfile.tellp();
  writeRecordHeader( outfile, "OTNL", total );

  unsigned int numObjects = objectNames.size();
  outfile.write( (char*)&numObjects, sizeof( numObjects ) );
  total += sizeof( numObjects );

  std::cout << "numObjects: " << numObjects << std::endl;
  for( unsigned int i = 0; i < numObjects; ++i )
    {
      outfile.write(
		    objectNames[i].c_str(),
		    static_cast<unsigned int>( objectNames[i].size()+1 )
		    );
      total += objectNames[i].size()+1;
    }

  unsigned int nodeEndPosition = outfile.tellp();

  // Rewrite form with proper size.
  outfile.seekp( position, std::ios_base::beg );
  total += writeRecordHeader( outfile, "OTNL", total );

  outfile.seekp( nodeEndPosition, std::ios_base::beg );

  return total;
}

unsigned int ws::readOTNL( std::istream &file )
{
    unsigned int total = 0;
    unsigned int otnlSize;
    std::string type;

    total += readRecordHeader( file, type, otnlSize );
    otnlSize += 8;
    if( type != "OTNL" )
    {
	std::cout << "Expected record of type OTNL: " << type << std::endl;
	exit( 0 );
    }
#if DEBUG
    std::cout << "Found OTNL record"
	      << ": " << otnlSize << " bytes"
	      << std::endl;
#endif

    unsigned int numObjects;
    file.read( (char*)&numObjects, sizeof( numObjects ) );
    total += sizeof( numObjects );
#if DEBUG
    std::cout << "Num objects: " << numObjects << std::endl;
#endif

    char temp[512];
    for( unsigned int i = 0; i < numObjects; ++i )
    {
	file.getline( temp, 512, 0 );
	std::string objectName( temp );
	total += static_cast<unsigned int>( objectName.size() + 1 );
	objectNames.push_back( objectName );
    }

    if( otnlSize == total )
    {
#if DEBUG
	std::cout << "Finished reading OTNL" << std::endl;
#endif
    }
    else
    {
	std::cout << "FAILED in reading OTNL" << std::endl;
	std::cout << "Read " << total << " out of " << otnlSize
                  << std::endl;
    }
    
    return total;
}

unsigned int wsNode::read( std::istream &file )
{
    unsigned int total = 0;

    file.read( (char *)&nodeID, sizeof( nodeID ) );
    total += sizeof( nodeID );

    file.read( (char *)&parentNodeID, sizeof( parentNodeID ) );
    total += sizeof( parentNodeID );
    
    file.read( (char *)&objectIndex, sizeof( objectIndex ) );
    total += sizeof( objectIndex );
    
    file.read( (char *)&positionInParent, sizeof( positionInParent ) );
    total += sizeof( positionInParent );

    file.read( (char *)&rx, sizeof( rx ) );
    total += sizeof( rx );

    file.read( (char *)&ry, sizeof( ry ) );
    total += sizeof( ry );

    file.read( (char *)&rz, sizeof( rz ) );
    total += sizeof( rz );

    file.read( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );

    file.read( (char *)&x, sizeof( x ) );
    total += sizeof( x );
    
    file.read( (char *)&y, sizeof( y ) );
    total += sizeof( y );
    
    file.read( (char *)&z, sizeof( z ) );
    total += sizeof( z );

    file.read( (char *)&u2, sizeof( u2 ) );
    total += sizeof( u2 );

    file.read( (char *)&crc, sizeof( crc ) );
    total += sizeof( crc );

    return total;
}

unsigned int wsNode::write( std::ofstream &file )
{
    unsigned int total = 0;

    file.write( (char *)&nodeID, sizeof( nodeID ) );
    total += sizeof( nodeID );

    file.write( (char *)&parentNodeID, sizeof( parentNodeID ) );
    total += sizeof( parentNodeID );
    
    file.write( (char *)&objectIndex, sizeof( objectIndex ) );
    total += sizeof( objectIndex );
    
    file.write( (char *)&positionInParent, sizeof( positionInParent ) );
    total += sizeof( positionInParent );

    file.write( (char *)&rx, sizeof( rx ) );
    total += sizeof( rx );

    file.write( (char *)&ry, sizeof( ry ) );
    total += sizeof( ry );

    file.write( (char *)&rz, sizeof( rz ) );
    total += sizeof( rz );

    file.write( (char *)&u1, sizeof( u1 ) );
    total += sizeof( u1 );

    file.write( (char *)&x, sizeof( x ) );
    total += sizeof( x );
    
    file.write( (char *)&y, sizeof( y ) );
    total += sizeof( y );
    
    file.write( (char *)&z, sizeof( z ) );
    total += sizeof( z );

    file.write( (char *)&u2, sizeof( u2 ) );
    total += sizeof( u2 );

    file.write( (char *)&crc, sizeof( crc ) );
    total += sizeof( crc );

    return total;
}

void wsNode::print()
{
    std::cout << "Node ID: " << nodeID << std::endl;
    std::cout << "Parent node ID: " << parentNodeID << std::endl;
    std::cout << "Object index: " << objectIndex << std::endl;
    std::cout << "Object filename: " << objectFilename << std::endl;
    std::cout << "Position in parent: " << positionInParent << std::endl;
    std::cout << "Rotation about X(deg): " << rx * RAD2DEG << std::endl;
    std::cout << "Rotation about Y(deg): " << ry * RAD2DEG << std::endl;
    std::cout << "Rotation about Z(deg): " << rz * RAD2DEG << std::endl;
    std::cout << "Unknown 1: 0x" << std::hex << u1 << std::dec << std::endl;
    std::cout << "X(m): " << x << std::endl;
    std::cout << "Y(m): " << y << std::endl;
    std::cout << "Z(m): " << z << std::endl;
    std::cout << "Unknown 2: 0x" << std::hex << u2 << std::endl;
    std::cout << "CRC: 0x" << std::hex << crc << std::dec << std::endl;
    std::cout << "level: " << level << std::endl;
    std::cout << std::endl;
}
