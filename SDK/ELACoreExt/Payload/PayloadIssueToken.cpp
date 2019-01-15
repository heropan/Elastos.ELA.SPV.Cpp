// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cstring>
#include <SDK/Common/Log.h>
#include <SDK/Common/Utils.h>

#include "PayloadIssueToken.h"

namespace Elastos {
	namespace ElaWallet {

		PayloadIssueToken::PayloadIssueToken() {

		}

		PayloadIssueToken::PayloadIssueToken(const CMBlock &merkeProff, const CMBlock &mainChainTransaction) {
			_merkeProof = merkeProff;
			_mainChainTransaction = mainChainTransaction;
		}

		PayloadIssueToken::~PayloadIssueToken() {
		}

		void PayloadIssueToken::Serialize(ByteStream &ostream, uint8_t version) const {
			if (version == PayloadIssueToken::V0) {
				ostream.writeVarBytes(_merkeProof);
				ostream.writeVarBytes(_mainChainTransaction);
			} else if (version == PayloadIssueToken::V1) {
				ostream.writeBytes(&_mainChainTxHash, sizeof(_mainChainTxHash));
			} else {
				Log::getLogger()->error("Serialize: invalid recharge to side chain payload version = {}", version);
			}
		}

		bool PayloadIssueToken::Deserialize(ByteStream &istream, uint8_t version) {
			if (version == PayloadIssueToken::V0) {
				if (!istream.readVarBytes(_merkeProof)) {
					Log::getLogger()->error("PayloadIssueToken deserialize merke proof error");
					return false;
				}

				if (!istream.readVarBytes(_mainChainTransaction)) {
					Log::getLogger()->error("PayloadIssueToken deserialize main chain transaction error");
					return false;
				}
			} else if (version == PayloadIssueToken::V1) {
				if (!istream.readBytes(&_mainChainTxHash, sizeof(_mainChainTxHash))) {
					Log::getLogger()->error("Deserialize: recharge to side chain payload read tx hash");
					return false;
				}
			} else {
				Log::getLogger()->error("Serialize: invalid recharge to side chain payload version = {}", version);
				return false;
			}

			return true;
		}

		nlohmann::json PayloadIssueToken::toJson(uint8_t version) const {
			nlohmann::json j;

			if (version == PayloadIssueToken::V0) {
				j["MerkleProof"] = Utils::encodeHex(_merkeProof);
				j["MainChainTransaction"] = Utils::encodeHex(_mainChainTransaction);
			} else if (version == PayloadIssueToken::V1) {
				j["MainChainTxHash"] = Utils::UInt256ToString(_mainChainTxHash, true);
			} else {
				Log::getLogger()->error("toJson: invalid recharge to side chain payload version = {}", version);
			}

			return j;
		}

		void PayloadIssueToken::fromJson(const nlohmann::json &j, uint8_t version) {
			if (version == PayloadIssueToken::V0) {
				_merkeProof = Utils::decodeHex(j["MerkleProof"].get<std::string>());
				_mainChainTransaction = Utils::decodeHex(j["MainChainTransaction"].get<std::string>());
			} else if (version == PayloadIssueToken::V1) {
				_mainChainTxHash = Utils::UInt256FromString(j["MainChainTxHash"].get<std::string>(), true);
			} else {
				Log::getLogger()->error("fromJson: invalid recharge to side chain payload version = {}", version);
			}
		}
	}
}