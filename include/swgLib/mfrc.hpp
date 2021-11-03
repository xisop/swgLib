/** -*-c++-*-
 *  \class  mfrc
 *  \file   mfrc.hpp
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

#include <istream>
#include <vector>

#ifndef MFRC_HPP
#define MFRC_HPP 1

namespace ml
{
	// Multifractal
	class mfrc
	{
	public:
		mfrc();
		~mfrc();

		std::size_t read(std::istream& file);

		const uint32_t& getSeed() const;

		bool usesBias() const;
		const float& getBias() const;

		bool usesGain() const;
		const float& getGain() const;

		const int32_t& getNumOctaves() const;
		const float& getFrequency() const;
		const float& getAmplitude() const;

		const float& getScaleX() const;
		const float& getScaleY() const;

		const float& getOffsetX() const;
		const float& getOffsetY() const;

		const int32_t& getCombinationRule() const;

	protected:
		uint32_t _version;

		uint32_t _seed;
		bool  _useBias;
		float _bias;

		bool  _useGain;
		float _gain;

		int32_t _numOctaves;
		float   _frequency;
		float   _amplitude;

		// Scale
		float _scaleX;
		float _scaleY;

		// Offset
		float _offsetX;
		float _offsetY;

		int32_t _combinationRule;

	private:
	};
}

#endif
