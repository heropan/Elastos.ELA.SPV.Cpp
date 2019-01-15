// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cstring>
#include <SDK/Common/Utils.h>

#include "PayloadCoinBase.h"
#include "BRInt.h"

namespace Elastos {
	namespace ElaWallet {
		PayloadCoinBase::PayloadCoinBase() {

		}

		PayloadCoinBase::PayloadCoinBase(CMBlock &coinBaseData) {
			_coinBaseData = coinBaseData;
		}

		PayloadCoinBase::~PayloadCoinBase() {
		}

		void PayloadCoinBase::setCoinBaseData(const CMBlock &coinBaseData) {
			_coinBaseData = coinBaseData;
		}

		void PayloadCoinBase::Serialize(ByteStream &ostream, uint8_t version) const {
			ostream.writeVarBytes(_coinBaseData);
		}

		bool PayloadCoinBase::Deserialize(ByteStream &istream, uint8_t version) {
			return istream.readVarBytes(_coinBaseData);
		}

		nlohmann::json PayloadCoinBase::toJson(uint8_t version) const {
			nlohmann::json j;
			j["CoinBaseData"] = Utils::encodeHex(_coinBaseData);
			return j;
		}

		void PayloadCoinBase::fromJson(const nlohmann::json &j, uint8_t version) {
			_coinBaseData = Utils::decodeHex(j["CoinBaseData"].get<std::string>());

		}
	}
}