// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "PayloadTransferAsset.h"

namespace Elastos {
	namespace ElaWallet {

		PayloadTransferAsset::PayloadTransferAsset() {

		}

		PayloadTransferAsset::~PayloadTransferAsset() {

		}

		void PayloadTransferAsset::Serialize(ByteStream &ostream, uint8_t version) const {

		}

		bool PayloadTransferAsset::Deserialize(ByteStream &istream, uint8_t version) {
			return true;
		}

		nlohmann::json PayloadTransferAsset::toJson(uint8_t version) const {
			nlohmann::json jsonData;
			return jsonData;
		}

		void PayloadTransferAsset::fromJson(const nlohmann::json &jsonData, uint8_t version) {

		}
	}
}