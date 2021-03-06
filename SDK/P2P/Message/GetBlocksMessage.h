// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_GETBLOCKSMESSAGE_H__
#define __ELASTOS_SDK_GETBLOCKSMESSAGE_H__

#include "Message.h"

namespace Elastos {
	namespace ElaWallet {

		struct GetBlocksParameter : public SendMessageParameter {
			std::vector<uint256> locators;
			uint256 hashStop;

			GetBlocksParameter() { memset(&hashStop, 0, sizeof(hashStop)); }

			GetBlocksParameter(const std::vector<uint256> &locators, const uint256 &hashStop) :
				locators(locators), hashStop(hashStop)
			{}
		};

		class GetBlocksMessage : public Message {
		public:
			explicit GetBlocksMessage(const MessagePeerPtr &peer);

			virtual bool Accept(const bytes_t &msg);

			virtual void Send(const SendMessageParameter &param);

			virtual std::string Type() const;

		};

	}
}

#endif //__ELASTOS_SDK_GETBLOCKSMESSAGE_H__
