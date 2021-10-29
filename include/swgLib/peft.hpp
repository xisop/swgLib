/** -*-c++-*-
 *  \class  peft
 *  \file   peft.hpp
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
#include <swgLib/emgp.hpp>
#include <swgLib/ptim.hpp>

#include <istream>
#include <string>
#include <vector>

#ifndef PEFT_HPP
#define PEFT_HPP 1

namespace ml
{
	class peft
	{
	public:
		peft();
		~peft();

		std::size_t readPEFT(std::istream& file);

	protected:
#if 0
		std::size_t read0000(std::istream& file, uint32_t& num);
		std::size_t readEMGP(std::istream& file);
		std::size_t readEMTR(std::istream& file);
		std::size_t readEMTR0000(std::istream& file);
		std::size_t readWVFM(std::istream& file);
		std::size_t readPTQD(std::istream& file);
		std::size_t readPTCL(std::istream& file);
		std::size_t readPTCL0002(std::istream& file);
		std::size_t readCLRR(std::istream& file);
		std::size_t readPTEX(std::istream& file);
#endif

	private:
		uint32_t _version;

		ptim _timing;
		int32_t _numEmitterGroups;
		float _initialPlaybackRate;
		float _initialPlaybackRateTime;
		float _playbackRate;
		float _scale;
		std::vector<emgp> _emitterGroups;

	};
}
#endif
