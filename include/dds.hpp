
#include <iostream>
#include <istream>
#include <vector>

#ifndef DDS_HPP
#define DDS_HPP 1

#define DDS_MAGIC 0x20534444

#define FCC_DXT1 0x31545844
#define FCC_DXT2 0x32545844
#define FCC_DXT3 0x33545844
#define FCC_DXT4 0x34545844
#define FCC_DXT5 0x35545844
#define FCC_DX10 0x30315844

#define MAX_RED 31
#define MAX_GREEN 63
#define MAX_BLUE 31

class dds {
public:
	dds();
	~dds();

	bool isDDS() const;
	bool is32bit() const;
	bool is24bit() const;
	bool isUncompressedRGBA() const;	
	bool isDXT1() const;
	bool isDXT5() const;

	bool readMagicNumber(std::istream& is);
	bool readHeader(std::istream& is);

	void print(std::ostream& os) const;

	void readColor(const uint32_t& pixel, uint32_t bitmask, uint32_t& color);

	void read24bit(std::istream& is);
	void read32bit(std::istream& is);

	void setPixel(const uint32_t& row, const uint32_t& col,
		const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a);
	void setPixelAlpha(const uint32_t& row, const uint32_t& col, const uint8_t& a);
	void setPixel(const uint32_t& row, const uint32_t& col,
		const float& r, const float& g, const float& b, const float& a);
	void setPixel(const uint32_t& row, const uint32_t& col, const uint16_t& r5g6b5);

	void readDXT1(std::istream& is);
	void readDXT5(std::istream& is);

	void writePPM(std::ostream& os) const;

private:
	void colorToFloat(const uint16_t& c, float& r, float& g, float& b);
	void floatToColor(const float& r, const float& g, const float& b, uint16_t& c);

	void interpColors(const uint16_t& c0, const uint16_t& c1, uint16_t& c2, uint16_t& c3);

	void readBC1Block(std::istream& is, const uint32_t& blockRow, const uint32_t& blockCol);
	void readBC3Block(std::istream& is, const uint32_t& blockRow, const uint32_t& blockCol);

private:
	// Magic number
	uint32_t _magicNumber;

	// Header begin
	uint32_t _dwSize; // 124
	uint32_t _dwFlags;
	uint32_t _dwHeight; // Height in pixels
	uint32_t _dwWidth; // Width in pixels
	uint32_t _dwPitchOrLinearSize; // Num bytes per scan line (uncompressed)
	uint32_t _dwDepth; // Depth of volume texture (in pixels)
	uint32_t _dwMipMapCount; // Number of mipmap levels
	uint32_t _dwReserved1[11];

	// Pixel format start
	uint32_t _pfdwSize; // Size of pixel format structure (32 bytes)
	uint32_t _pfdwFlags; // 
	uint32_t _pfdwFourCC; // DXT1, DXT2, DXT3, DXT4, DXT5, or DX10 
	uint32_t _pfdwRGBBitCount;
	uint32_t _pfdwRBitMask;
	uint32_t _pfdwGBitMask;
	uint32_t _pfdwBBitMask;
	uint32_t _pfdwABitMask;
	// Pixel format end

	uint32_t _dwCaps;
	uint32_t _dwCaps2;
	uint32_t _dwCaps3;
	uint32_t _dwCaps4;
	uint32_t _dwReserved2;
	// Header end

	// Header DXT10 (Optional) start
	uint32_t _dxgiFormat;
	uint32_t _resourceDimension;
	uint32_t _miscFlag;
	uint32_t _arraySize;
	uint32_t _miscFlags2;
	// Header DXT10 (Optional) end

private:
	bool _containsAlpha;
	bool _compressedRGB;
	bool _uncompressedRGB;
	bool _uncompressedYUV;
	bool _uncompressedSingleChannel;
	uint32_t _numBytes;

	std::vector<uint32_t> _pixels;
};

#endif