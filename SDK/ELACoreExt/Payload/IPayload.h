// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IPAYLOAD_H__
#define __ELASTOS_SDK_IPAYLOAD_H__

#include <boost/shared_ptr.hpp>

#include "SDK/Plugin/Interface/ELAMessageSerializable.h"
#include "CMemBlock.h"

namespace Elastos {
	namespace ElaWallet {

		class IPayload {
		public:
			virtual ~IPayload();

			virtual CMBlock getData(uint8_t version) const;

			virtual void Serialize(ByteStream &ostream, uint8_t version) const = 0;
			virtual bool Deserialize(ByteStream &istream, uint8_t version) = 0;

			virtual nlohmann::json toJson(uint8_t version) const = 0;
			virtual void fromJson(const nlohmann::json &, uint8_t version) = 0;

			virtual bool isValid() const;
		};

		typedef boost::shared_ptr<IPayload> PayloadPtr;

	}
}

#endif //__ELASTOS_SDK_IPAYLOAD_H__
