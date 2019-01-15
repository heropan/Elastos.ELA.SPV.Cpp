// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_PAYLOADISSUETOKEN_H
#define __ELASTOS_SDK_PAYLOADISSUETOKEN_H

#include "IPayload.h"

namespace Elastos {
	namespace ElaWallet {

		class PayloadIssueToken :
				public IPayload {
		public:
			enum Version {
				V0,
				V1,
			};

		public:
			PayloadIssueToken();

			PayloadIssueToken(const CMBlock &merkeProff, const CMBlock &mainChainTransaction);

			~PayloadIssueToken();

			virtual void Serialize(ByteStream &ostream, uint8_t version) const;

			virtual bool Deserialize(ByteStream &istream, uint8_t version);

			virtual nlohmann::json toJson(uint8_t version) const;

			virtual void fromJson(const nlohmann::json &jsonData, uint8_t version);

		private:
			CMBlock _merkeProof;
			CMBlock _mainChainTransaction;
			UInt256 _mainChainTxHash;
		};
	}
}

#endif //__ELASTOS_SDK_PAYLOADISSUETOKEN_H
