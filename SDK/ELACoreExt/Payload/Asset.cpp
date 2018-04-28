// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cstring>

#include "Asset.h"
#include "BRInt.h"

namespace Elastos {
	namespace SDK {
		Asset::Asset() :
				_name(""),
				_description(""),
				_precision(0),
				_assetType(AssetType::Share),
				_assetRecordType(AssetRecordType::Unspent) {

		}

		Asset::~Asset() {

		}

		void Asset::setName(const std::string &name) {
			_name = name;
		}

		std::string Asset::getName() const {
			return _name;
		}

		void Asset::setDescription(const std::string &desc) {
			_description = desc;
		}

		std::string Asset::getDescription() const {
			return _description;
		}

		void Asset::setAssetType(const Asset::AssetType type) {
			_assetType = type;
		}

		Asset::AssetType Asset::getAssetType() const {
			return _assetType;
		}

		void Asset::setAssetRecordType(const Asset::AssetRecordType type) {
			_assetRecordType = type;
		}

		Asset::AssetRecordType Asset::getAssetRecordType() const {
			return _assetRecordType;
		}

		void Asset::Serialize(ByteStream &ostream) const {
			uint64_t len = _name.length();
			uint8_t nameLength[64 / 8];
			UInt64SetLE(nameLength, len);
			ostream.putBytes(nameLength, sizeof(nameLength));
			ostream.putBytes((uint8_t *) _name.c_str(), len);

			len = _description.length();
			uint8_t descLength[64 / 8];
			UInt64SetLE(descLength, len);
			ostream.putBytes(descLength, sizeof(descLength));
			ostream.putBytes((uint8_t *) _description.c_str(), len);

			ostream.put(_precision);

			ostream.put(_assetType);

			ostream.put(_assetRecordType);
		}

		void Asset::Deserialize(ByteStream &istream) {
			uint8_t nameLength[64 / 8];
			istream.getBytes(nameLength, sizeof(nameLength));
			uint64_t len = UInt64GetLE(nameLength);
			char *utfBuffer = new char[len + 1];
			istream.getBytes((uint8_t *) utfBuffer, len);
			utfBuffer[len] = '\0';
			_name = utfBuffer;

			uint8_t descLength[64 / 8];
			istream.getBytes(descLength, sizeof(descLength));
			len = UInt64GetLE(descLength);
			utfBuffer = new char[len + 1];
			memset(utfBuffer, 0, len + 1);
			istream.getBytes((uint8_t *) utfBuffer, len);
			utfBuffer[len] = '\0';
			_description = utfBuffer;

			_precision = istream.get();

			_assetType = AssetType(istream.get());

			_assetRecordType = AssetRecordType(istream.get());
		}
	}
}