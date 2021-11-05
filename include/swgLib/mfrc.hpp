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

		void setSeed(const uint32_t& seed);
		const uint32_t& getSeed() const;

		void enableBias(bool enable = true);
		bool usesBias() const;

		void setBias(const float& bias);
		const float& getBias() const;

		void enableGain(bool enable = true);
		bool usesGain() const;

		void setGain(const float& gain);
		const float& getGain() const;

		void setNumOctaves(const int32_t& numOctaves);
		const int32_t& getNumOctaves() const;

		void setFrequency(const float& frequency);
		const float& getFrequency() const;

		void setAmplitude(const float& amplitude);
		const float& getAmplitude() const;

		void setScale(const float& scaleX, const float& scaleY);
		void setScaleX(const float& scaleX);
		const float& getScaleX() const;

		void setScaleY(const float& scaleY);
		const float& getScaleY() const;

		void setOffset(const float& offsetX, const float& offsetY);
		void setOffsetX(const float &offsetX);
		const float& getOffsetX() const;

		void setOffsetY(const float& offsetY);
		const float& getOffsetY() const;

		void setCombinationRule(const int32_t& rule);
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
