// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_PAYLOADCOINBASE_H
#define __ELASTOS_SDK_PAYLOADCOINBASE_H

#include "IPayload.h"

namespace Elastos {
	namespace ElaWallet {
		class PayloadCoinBase :
				public IPayload {
		public:
			PayloadCoinBase();

			PayloadCoinBase(CMBlock &coinBaseData);

			~PayloadCoinBase();

			void setCoinBaseData(const CMBlock &coinBaseData);

			virtual void Serialize(ByteStream &ostream, uint8_t version) const;

			virtual bool Deserialize(ByteStream &istream, uint8_t version);

			virtual nlohmann::json toJson(uint8_t version) const;

			virtual void fromJson(const nlohmann::json &jsonData, uint8_t version);

		private:
			CMBlock _coinBaseData;
		};
	}
}

#endif //__ELASTOS_SDK_PAYLOADCOINBASE_H
