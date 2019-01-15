// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_PAYLOADREGISTERPRODUCER_H__
#define __ELASTOS_SDK_PAYLOADREGISTERPRODUCER_H__

#include "IPayload.h"

namespace Elastos {
	namespace ElaWallet {

		class PayloadRegisterProducer : public IPayload {
		public:
			PayloadRegisterProducer();

			~PayloadRegisterProducer();

			const std::string &GetPublicKey() const;

			void SetPublicKey(const std::string &key);

			const std::string &GetNickName() const;

			void SetNickName(const std::string &name);

			const std::string &GetUrl() const;

			void SetUrl(const std::string &url);

			uint64_t GetLocation() const;

			void SetLocation(uint64_t location);

			virtual void Serialize(ByteStream &ostream, uint8_t version) const;

			virtual bool Deserialize(ByteStream &istream, uint8_t version);

			virtual nlohmann::json toJson(uint8_t version) const;

			virtual void fromJson(const nlohmann::json &, uint8_t version);

		private:
			std::string _publicKey;
			std::string _nickName;
			std::string _url;
			uint64_t _location;
		};

	}
}

#endif //__ELASTOS_SDK_PAYLOADREGISTERPRODUCER_H__
