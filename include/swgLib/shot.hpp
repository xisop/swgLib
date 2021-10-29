/** -*-c++-*-
 *  \class  shot
 *  \file   shot.hpp
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
#include <ostream>
#include <string>

#ifndef SHOT_HPP
#define SHOT_HPP 1

namespace ml
{
	class shot
	{
	public:
		shot();
		virtual ~shot();

		std::size_t readSHOT(std::istream& file);
		void print(std::ostream& os) const;

		const std::string& getBaseObjectFilename() const;

		const std::string& getObjectName() const;
		const std::string& getDetailedDescription() const;
		const std::string& getLookAtText() const;
		bool isSnapToTerrain() const;
		const int32_t& getContainerType() const;
		const int32_t& getContainerVolumeLimit() const;
		const std::string& getTintPalette() const;
		const std::string& getSlotDescriptorFilename() const;
		const std::string& getArrangementDescriptorFilename() const;
		const std::string& getAppearanceFilename() const;
		const std::string& getPortalLayoutFilename() const;
		const std::string& getClientDataFile() const;
		const int32_t& getCollisionMaterialFlags() const;
		const int32_t& getCollisionMaterialPassFlags() const;
		const int32_t& getCollisionMaterialBlockFlags() const;
		const int32_t& getCollisionActionFlags() const;
		const int32_t& getCollisionActionPassFlags() const;
		const int32_t& getCollisionActionBlockFlags() const;
		const float& getScale() const;
		const int32_t& getGameObjectType() const;
		bool isSendToClient() const;
		const float& getScaleThresholdBeforeExtentTest() const;
		const float& getClearFloraRadius() const;
		const int32_t& getSurfaceType() const;
		const float& getNoBuildRadius() const;
		bool isOnlyVisibleInTools() const;
		const float& getLocationReservationRadius() const;
		bool isForceNoCollision() const;

	protected:
		std::size_t readDERV(std::istream& file, std::string& filename);
		static std::size_t readPCNT(std::istream& file, int32_t& numParameters);
		std::size_t readSHOTParameter(std::istream& file);

		std::string _baseObjectFilename;

		int32_t      _shotVersion;

		std::string _objectName;
		std::string _detailedDescription;
		std::string _lookAtText;
		bool        _snapToTerrain;
		int32_t     _containerType;
		int32_t     _containerVolumeLimit;
		std::string _tintPalette;
		std::string _slotDescriptorFilename;
		std::string _arrangementDescriptorFilename;
		std::string _appearanceFilename;
		std::string _portalLayoutFilename;
		std::string _clientDataFile;
		int32_t     _collisionMaterialFlags;
		int32_t     _collisionMaterialPassFlags;
		int32_t     _collisionMaterialBlockFlags;
		int32_t     _collisionActionFlags;
		int32_t     _collisionActionPassFlags;
		int32_t     _collisionActionBlockFlags;
		float       _scale;
		int32_t     _gameObjectType;
		bool        _sendToClient;
		float       _scaleThresholdBeforeExtentTest;
		float       _clearFloraRadius;
		int32_t     _surfaceType;
		float       _noBuildRadius;
		bool        _onlyVisibleInTools;
		float       _locationReservationRadius;
		bool        _forceNoCollision;
	};
}
#endif
