/** -*-c++-*-
 *  \class  cldf
 *  \file   cldf.cpp
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

#include <swgLib/base.hpp>
#include <swgLib/cldf.hpp>

#include <iostream>
#include <cstdlib>

using namespace ml;

cldf::cldf()
{
}

cldf::~cldf()
{
}

std::size_t cldf::read(std::istream& file)
{
	std::size_t cldfSize;
	std::size_t total = base::readFormHeader(file, "CLDF", cldfSize);
	cldfSize += 8;
	std::cout << "Found form CLDF: " << cldfSize << "\n";

	std::size_t size;
	total += base::readFormHeader(file, "0000", size);

	std::string form, type;
	while (total < cldfSize) {
		base::peekHeader(file, form, size, type);

		if ("FORM" == form) {
			if ("DAMA" == type) {
				// Damage level
				damageLevel newDamage;
				total += newDamage.read(file);
				_damageLevel.push_back(newDamage);
			}
#if 0
			else if ("ONOF" == type) {
				//On/Off object
			}
			else if ("WEAR" == type) {
				// Wearable
			}
			else if ("FLAG" == type) {
				// Flag
			}
			else if ("BANR" == type) {
				// Banner
			}
			else if ("VTHR" == type) {
				// Vehicle thruster
			}
			else if ("VGEF" == type) {
				// Vehicle ground effect
			}
			else if ("VLEF" == type) {
				// Vehicle lightning effect
			}
			else if ("CONT" == type) {
				// Contrail data
			}
			else if ("WING" == type) {
			}
			else if ("GLOW" == type) {
			}
			else if ("INTS" == type) {
				// Interpolated sound
			}
			else if ("ENGS" == type) {
				// Engine sounds
			}
			else if ("BRKP" == type) {
				// Breakpoint
			}
			else if ("DSTR" == type) {
				// Destruction effect
			}
			else if ("DSEF" == type) {
				// Destruction sequence
			}
#endif
			else {
				std::cout << "Unhandled FORM type: " << type << "\n";
				exit(0);
			}
		}
		else if ("ASND" == form) {
			// Ambient Sound 
			total += base::readFormHeader(file, "ASND", size);
			total += base::read(file, _ambientSoundFilename);
		}
#if 0
		else if ("CEFT" == form) {
			// Cached Effect Template
		}
		else if ("CHLD" == form) {
			// Transform child object
		}
		else if ("CHL2" == form) {
			// Transform child object
		}
		else if ("CNCF" == form) {
			// Clear flora
		}
		else if ("CSND" == form) {
			// Cached Sound
		}
		else if ("EVNT" == form) {
			// Event
		}
		else if ("HLOB" == form) {
			// Hardpoint light object
		}
		else if ("HOBJ" == form) {
			// Hardpoint child object
		}
		else if ("IHOB" == form) {
			// Initial hardpoint child object
		}
		else if ("LOBJ" == form) {
			// Transform light object
		}
		else if ("SUNT" == form) {
			// Sun tracking object
		}
		else if ("TURR" == form) {
			// Turret object
		}
		else if ("CSSI" == form) {
			//
		}
#endif
		else {
			std::cout << "Unhandled form: " << form << "\n";
			exit(0);
		}
	}

	if (cldfSize == total) {
		std::cout << "Finished reading CLDF\n";
	}
	else {
		std::cout << "FAILED in reading CLDF\n"
			<< "Read " << total << " out of " << cldfSize << "\n";
		exit(0);
	}

	return total;
}

void cldf::print(std::ostream& os) const
{
}

//*****************************************************************************

cldf::damageLevel::damageLevel() :
	_minDamageLevel(0.0f),
	_maxDamageLevel(0.0f),
	_clientEffectName(""),
	_appearanceName(""),
	_useHardpoint(false),
	_positionX(0.0f),
	_positionY(0.0f),
	_positionZ(0.0f),
	_yaw(0.0f),
	_pitch(0.0f),
	_roll(0.0f),
	_hardpointName(""),
	_ambientSoundName("")
{
}

cldf::damageLevel::~damageLevel() {
}

std::size_t cldf::damageLevel::read(std::istream& file) {
	std::size_t total = base::read(file, _minDamageLevel);
	total += base::read(file, _maxDamageLevel);
	total += base::read(file, _clientEffectName);
	total += base::read(file, _appearanceName);
	total += base::read(file, _useHardpoint);
	total += base::read(file, _positionX);
	total += base::read(file, _positionY);
	total += base::read(file, _positionZ);
	total += base::read(file, _yaw);
	total += base::read(file, _pitch);
	total += base::read(file, _roll);
	total += base::read(file, _hardpointName);
	total += base::read(file, _ambientSoundName);
	return total;
}

void cldf::damageLevel::print(std::ostream& os) const {

}

