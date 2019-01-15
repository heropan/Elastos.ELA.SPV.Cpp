// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_PAYLOADSIDEMINING_H
#define __ELASTOS_SDK_PAYLOADSIDEMINING_H

#include "IPayload.h"
#include "BRInt.h"

namespace Elastos {
	namespace ElaWallet {

		class PayloadSideMining :
				public IPayload {
		public:
			PayloadSideMining();

			PayloadSideMining(const UInt256 &sideBlockHash, const UInt256 &sideGensisHash, uint32_t height, const CMBlock &signedData);

			~PayloadSideMining();

            void setSideBlockHash(const UInt256 &sideBlockHash);
            void setSideGenesisHash(const UInt256 &sideGensisHash);
            void setBlockHeight(uint32_t height);
            void setSignedData(const CMBlock &signedData);


			virtual void Serialize(ByteStream &ostream, uint8_t version) const;

			virtual bool Deserialize(ByteStream &istream, uint8_t version);

			virtual nlohmann::json toJson(uint8_t version) const;

			virtual void fromJson(const nlohmann::json &jsonData, uint8_t version);

		private:
			UInt256 _sideBlockHash;
			UInt256 _sideGenesisHash;
			uint32_t _blockHeight;
			CMBlock _signedData;
		};
	}
}

#endif //__ELASTOS_SDK_PAYLOADSIDEMINING_H
