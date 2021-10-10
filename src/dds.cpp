/** -*-c++-*-
 *  \class  dds
 *  \file   dds.hpp
 *  \author Ken Sewell

 swgLib is used for the parsing and exporting SWG models.
 Copyright (C) 2021 Ken Sewell

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

#include "dds.hpp"

dds::dds() :
	_containsAlpha(false),
	_compressedRGB(false),
	_uncompressedRGB(false),
	_uncompressedYUV(false),
	_uncompressedSingleChannel(false)
{}

dds::~dds() {}

bool dds::isDDS() const { return (DDS_MAGIC == _magicNumber); }
bool dds::is32bit() const { return (32 == _pfdwRGBBitCount); }
bool dds::is24bit() const { return (24 == _pfdwRGBBitCount); }
bool dds::isUncompressedRGBA() const { return _uncompressedRGB; }
bool dds::isDXT1() const {
	if (_compressedRGB && (FCC_DXT1 == _pfdwFourCC)) {
		return true;
	}
	else {
		return false;
	}
}

bool dds::isDXT5() const {
	if (_compressedRGB && (FCC_DXT5 == _pfdwFourCC)) {
		return true;
	}
	else {
		return false;
	}
}

bool dds::readMagicNumber(std::istream& is) {
	is.read((char*)&_magicNumber, 4);

	return true;
}

bool dds::readHeader(std::istream& is) {
	is.read((char*)&_dwSize, 4);
	is.read((char*)&_dwFlags, 4);
	is.read((char*)&_dwHeight, 4);
	is.read((char*)&_dwWidth, 4);
	is.read((char*)&_dwPitchOrLinearSize, 4);
	is.read((char*)&_dwDepth, 4);
	is.read((char*)&_dwMipMapCount, 4);
	is.read((char*)_dwReserved1, 4 * 11);

	is.read((char*)&_pfdwSize, 4);
	is.read((char*)&_pfdwFlags, 4);
	is.read((char*)&_pfdwFourCC, 4);
	is.read((char*)&_pfdwRGBBitCount, 4);
	is.read((char*)&_pfdwRBitMask, 4);
	is.read((char*)&_pfdwGBitMask, 4);
	is.read((char*)&_pfdwBBitMask, 4);
	is.read((char*)&_pfdwABitMask, 4);

	is.read((char*)&_dwCaps, 4);
	is.read((char*)&_dwCaps2, 4);
	is.read((char*)&_dwCaps3, 4);
	is.read((char*)&_dwCaps4, 4);
	is.read((char*)&_dwReserved2, 4);

	if (FCC_DX10 == _pfdwFourCC) {
		is.read((char*)&_dxgiFormat, 4);
		is.read((char*)&_resourceDimension, 4);
		is.read((char*)&_miscFlag, 4);
		is.read((char*)&_arraySize, 4);
		is.read((char*)&_miscFlags2, 4);
	}

	_containsAlpha = ((_pfdwFlags & 0x1) == 0x1);
	_compressedRGB = ((_pfdwFlags & 0x4) == 0x4);
	_uncompressedRGB = ((_pfdwFlags & 0x40) == 0x40);
	if (_compressedRGB && _uncompressedRGB) {
		std::cout << "Header error: flags indicate data is compressed and uncompressed\n";
		return false;
	}

	_uncompressedYUV = ((_pfdwFlags & 0x200) == 0x200);
	_uncompressedSingleChannel = ((_pfdwFlags & 0x20000) == 0x20000);

	return true;
}

void dds::print(std::ostream& os) const {
	std::cout
		<< "Size: " << _dwSize << "\n"
		<< "Flags: 0x" << std::hex << _dwFlags << std::dec << "\n";
	if (_dwFlags & 0x1) { std::cout << "\tCaps data is valid\n"; }
	if (_dwFlags & 0x2) { std::cout << "\tHeight data is valid\n"; }
	if (_dwFlags & 0x4) { std::cout << "\tWidth data is valid\n"; }
	if (_dwFlags & 0x8) { std::cout << "\tPitch data is valid\n"; }
	if (_dwFlags & 0x1000) { std::cout << "\tPixel format data is valid\n"; }
	if (_dwFlags & 0x20000) { std::cout << "\tMipMap count data is valid\n"; }
	if (_dwFlags & 0x80000) { std::cout << "\tLinear size data is valid\n"; }
	if (_dwFlags & 0x800000) { std::cout << "\tDepth data is valid\n"; }

	std::cout
		<< "Height: " << _dwHeight << "\n"
		<< "Width: " << _dwWidth << "\n"
		<< "Pitch/Linear Size: " << _dwPitchOrLinearSize << "\n"
		<< "Depth: " << _dwDepth << "\n"
		<< "MipMapCount: " << _dwMipMapCount << "\n";

	std::cout
		<< "Pixel format data:\n"
		<< "\tPixel format: Size: " << _pfdwSize << "\n"
		<< "\tPixel format: Flags: 0x" << std::hex << _pfdwFlags << std::dec << "\n";
	if (_pfdwFlags & 0x01) { std::cout << "\tTexture contains alpha data\n"; }
	if (_pfdwFlags & 0x02) { std::cout << "\tAlpha channel only uncompressed data\n"; }
	if (_pfdwFlags & 0x04) { std::cout << "\tTexture contains compressed RGB data. FourCC is valid\n"; }
	if (_pfdwFlags & 0x40) { std::cout << "\tTexture contains uncompressed RGB data. RGB Bitcount/masks are valid\n"; }
	if (_pfdwFlags & 0x200) { std::cout << "\tTexture contains YUV uncompressed data\n"; }
	if (_pfdwFlags & 0x20000) { std::cout << "\tTexture contains single channel uncompressed data\n"; }

	if (_compressedRGB) {
		std::cout << "\tTexture contains compressed RGB data\n"
			<< "\tPixel format: FourCC: ";
		switch (_pfdwFourCC) {
		case FCC_DXT1: std::cout << "DXT1\n"; break;
		case FCC_DXT2: std::cout << "DXT2\n"; break;
		case FCC_DXT3: std::cout << "DXT3\n"; break;
		case FCC_DXT4: std::cout << "DXT4\n"; break;
		case FCC_DXT5: std::cout << "DXT5\n"; break;
		case FCC_DX10: std::cout << "DXT10\n"; break;
		default:
			std::cout << "Unknown\n"; break;
		}
	}

	if (!_compressedRGB) {
		std::cout
			<< "\tTexture contains uncompresssed RGB data.\n"
			<< "\tPixel format: RGB Bit Count: " << _pfdwRGBBitCount << "\n"
			<< "\tPixel format: R Bit Mask: 0x" << std::hex << _pfdwRBitMask << "\n"
			<< "\tPixel format: G Bit Mask: 0x" << _pfdwGBitMask << "\n"
			<< "\tPixel format: B Bit Mask: 0x" << _pfdwBBitMask << "\n"
			<< "\tPixel format: A Bit Mask: 0x" << _pfdwABitMask << std::dec << "\n";
	}

	if (_uncompressedYUV) {
		std::cout
			<< "\tTexture contains uncompressed YUV data.\n"
			<< "\tPixel format: YUV Bit Count: " << _pfdwRGBBitCount << "\n"
			<< "\tPixel format: Y Bit Mask: 0x" << std::hex << _pfdwRBitMask << "\n"
			<< "\tPixel format: U Bit Mask: 0x" << _pfdwGBitMask << "\n"
			<< "\tPixel format: V Bit Mask: 0x" << _pfdwBBitMask << std::dec << "\n";
	}

	if (_uncompressedSingleChannel) {
		std::cout
			<< "\tTexture contains uncompressed luminance channel data.\n"
			<< "\tPixel format: Luminance channel Bit Count: " << _pfdwRGBBitCount << "\n"
			<< "\tPixel format: Luminance Bit Mask: " << std::hex << _pfdwRBitMask << std::dec << "\n";
	}

	if (_pfdwFlags & 0x1000) { std::cout << "\tPixel format data is valid\n"; }
	if (_pfdwFlags & 0x20000) { std::cout << "\tMipMap count data is valid\n"; }
	if (_pfdwFlags & 0x80000) { std::cout << "\tLinear size data is valid\n"; }
	if (_pfdwFlags & 0x800000) { std::cout << "\tDepth data is valid\n"; }

	std::cout
		<< "Caps  : 0x" << std::hex << _dwCaps << "\n"
		<< "Caps 2: 0x" << _dwCaps2 << "\n"
		<< "Caps 3: 0x" << _dwCaps3 << "\n"
		<< "Caps 4: 0x" << _dwCaps4 << std::dec << "\n";
}

void dds::readColor(const uint32_t& pixel, uint32_t bitmask, uint32_t& color) {
	color = (pixel & bitmask);
	while ((bitmask & 0x1) != 0x1) {
		bitmask >>= 1;
		color >>= 1;
	}
}

void dds::read24bit(std::istream& is) {
	std::cout << "Reading 24-bits per pixel\n";

	// Setup final pixel array...
	const uint32_t numPixels(_dwWidth * _dwHeight);
	_pixels.resize(numPixels, 0x000000ff);

	for (uint32_t i = 0; i < numPixels; ++i) {
		// Read RGB into upper 24-bits...
		is.read((char*)&_pixels[i], 3);
	}
}

void dds::read32bit(std::istream& is) {
	std::cout << "Reading 32-bits per pixel\n";

	// Setup final pixel array...
	const uint32_t numPixels(_dwWidth * _dwHeight);
	_pixels.resize(numPixels, 0);

	// Read uncompressed data into pixel array...
	is.read((char*)_pixels.data(), numPixels * 4);

	// Decode rgba data and order it...
	uint32_t r, g, b, a;
	for (auto& p : _pixels) {
		readColor(p, _pfdwRBitMask, r);
		readColor(p, _pfdwGBitMask, g);
		readColor(p, _pfdwBBitMask, b);
		readColor(p, _pfdwABitMask, a);
		p = (r << 24) | (g << 16) | (b << 8) | a;
	}
}

void dds::setPixel(const uint32_t& row, const uint32_t& col,
	const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) {
	const uint32_t offset((row * _dwWidth) + col);

	_pixels[offset] = (r << 24) | (g << 16) | (b << 8) | a;
}

void dds::setPixelAlpha(const uint32_t& row, const uint32_t& col, const uint8_t& a) {
	const uint32_t offset((row * _dwWidth) + col);

	_pixels[offset] &= 0xfffffff0; // Zero out current alpha...
	_pixels[offset] |= a; // Set new alpha...
}

void dds::setPixel(const uint32_t& row, const uint32_t& col,
	const float& r, const float& g, const float& b, const float& a) {

	setPixel(row, col, uint8_t(r * 255), uint8_t(g * 255), uint8_t(b * 255), uint8_t(a * 255));
}

void dds::setPixel(const uint32_t& row, const uint32_t& col, const uint16_t& r5g6b5) {
	const uint32_t offset((row * _dwWidth) + col);

	uint8_t r = uint8_t(((r5g6b5 >> 11) / 31.0f) * 255);
	uint8_t g = uint8_t((((r5g6b5 >> 5) & 0x3f) / 63.0f) * 255);
	uint8_t b = uint8_t(((r5g6b5 & 0x1f) / 31.0f) * 255);

	setPixel(row, col, r, g, b, 255);
}

void dds::readDXT1(std::istream& is) {
	std::cout << "Reading DXT1\n";

	// Setup final pixel array...
	const uint32_t numPixels(_dwWidth * _dwHeight);
	_pixels.resize(numPixels, 0);

	const uint32_t blocksWide(_dwWidth / 4);
	const uint32_t blocksHigh(_dwHeight / 4);

	for (uint32_t bRow = 0; bRow < blocksHigh; ++bRow) {
		for (uint32_t bCol = 0; bCol < blocksWide; ++bCol) {
			readBC1Block(is, bRow, bCol);
		}
	}
}


void dds::readDXT5(std::istream& is) {
	std::cout << "Reading DXT5\n";

	// Setup final pixel array...
	const uint32_t numPixels(_dwWidth * _dwHeight);
	_pixels.resize(numPixels, 0);

	const uint32_t blocksWide(_dwWidth / 4);
	const uint32_t blocksHigh(_dwHeight / 4);

	for (uint32_t bRow = 0; bRow < blocksHigh; ++bRow) {
		for (uint32_t bCol = 0; bCol < blocksWide; ++bCol) {
			readBC3Block(is, bRow, bCol);
		}
	}
}

void dds::writePPM(std::ostream& os) const {
	os << "P3\n"
		<< _dwWidth << " " << _dwHeight << " " << 255 << "\n";

	for (auto p : _pixels) {
		os << (p >> 24) << " " << ((p >> 16) & 0xff) << " " << ((p >> 8) & 0xff) << "\n";
	}
}

void dds::colorToFloat(const uint16_t& c, float& r, float& g, float& b) {
	r = ((c >> 11) / (float)MAX_RED);
	g = (((c >> 5) & 0x3f) / (float)MAX_GREEN);
	b = ((c & 0x1f) / (float)MAX_RED);
}

void dds::floatToColor(const float& r, const float& g, const float& b, uint16_t& c) {
	c = 0;
	c |= (uint16_t(r * MAX_RED) << 11);
	c |= ((uint16_t(g * MAX_GREEN) & 0x3f) << 5);
	c |= (uint16_t(b * MAX_BLUE) & 0x1f);
}

void dds::interpColors(const uint16_t& c0, const uint16_t& c1,
	uint16_t& c2, uint16_t& c3) {

	float rgb0[3], rgb1[3], rgb2[3], rgb3[3];
	colorToFloat(c0, rgb0[0], rgb0[1], rgb0[2]);
	colorToFloat(c1, rgb1[0], rgb1[1], rgb1[2]);
	for (uint8_t i = 0; i < 3; ++i) {
		rgb2[i] = ((2.0f * rgb0[i]) + rgb1[i]) / 3.0f;
		rgb3[i] = (rgb0[i] + (2.0f * rgb1[i])) / 3.0f;
	}
	floatToColor(rgb2[0], rgb2[1], rgb2[2], c2);
	floatToColor(rgb3[0], rgb3[1], rgb3[2], c3);
}

void dds::readBC1Block(std::istream& is, const uint32_t& blockRow, const uint32_t& blockCol) {
	static uint16_t color[4];
	static uint32_t cData;

	is.read((char*)(&color[0]), 2);
	is.read((char*)(&color[1]), 2);
	is.read((char*)(&cData), 4);

	interpColors(color[0], color[1], color[2], color[3]);

	uint32_t shift = 0;
	for (uint32_t row = 0; row < 4; ++row) {
		for (uint32_t col = 0; col < 4; ++col) {
			uint32_t index = (cData >> shift) & 0x03;
			setPixel((blockRow * 4) + row, (blockCol * 4) + col, color[index]);
			shift += 2;
		}
	}
}

void dds::readBC3Block(std::istream& is, const uint32_t& blockRow, const uint32_t& blockCol) {
	static uint8_t alpha[8];
	static uint64_t aData;

	is.read((char*)alpha, 2); // Read alpha 0 and 1
	is.read((char*)&aData, 6); // Read alpha data in to upper 6 bytes
	aData >>= 16; // Right shift so data is in lower 6 bytes

	// Read and decode BC1 block...
	readBC1Block(is, blockRow, blockCol);

	if (alpha[0] > alpha[1]) {
		// Interpolate 6 values...
		for (uint32_t i = 1; i < 7; ++i) {
			alpha[i + 1] = uint8_t((((7.0f - i) * alpha[0]) + (i * alpha[1])) / 7.0f);
			//alpha[i + 1] = uint8_t((((7.0f - i) / 7.0f) * alpha[0]) + ((i / 7.0f) * alpha[1]));
		}
	}
	else {
		// Interpolate 4 values...
		for (uint32_t i = 1; i < 5; ++i) {
			alpha[i + 1] = uint8_t((((5.0f - i) * alpha[0]) + (i * alpha[1])) / 5.0f);
		}
		alpha[6] = 0;
		alpha[7] = 255;
	}

	uint32_t shift = 0;
	for (uint32_t row = 0; row < 4; ++row) {
		for (uint32_t col = 0; col < 4; ++col) {
			uint32_t index = (aData >> shift) & 0x07;
			setPixelAlpha((blockRow * 4) + row, (blockCol * 4) + col, alpha[index]);
			shift += 3;
		}
	}
}
