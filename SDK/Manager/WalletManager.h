// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_WALLETMANAGER_H__
#define __ELASTOS_SDK_WALLETMANAGER_H__

#include <boost/function.hpp>

#include "CoreWalletManager.h"
#include "DatabaseManager.h"
#include "BackgroundExecutor.h"

namespace Elastos {
	namespace SDK {

		class WalletManager :
				public CoreWalletManager {
		public:
			WalletManager(const ChainParams &chainParams = ChainParams::mainNet());

			WalletManager(const std::string &phrase, const ChainParams &chainParams = ChainParams::mainNet());

			virtual ~WalletManager();

			void start();

			void stop();

			UInt256 signAndPublishTransaction(const TransactionPtr &transaction);

			SharedWrapperList<Transaction, BRTransaction *> getTransactions(
					const boost::function<bool(const TransactionPtr &)> filter) const;

			void registerWalletListener(Wallet::Listener *listener);

			void registerPeerManagerListener(PeerManager::Listener *listener);

		public:
			// func balanceChanged(_ balance: UInt64)
			virtual void balanceChanged(uint64_t balance);

			// func txAdded(_ tx: BRTxRef)
			virtual void onTxAdded(Transaction *tx);

			// func txUpdated(_ txHashes: [UInt256], blockHeight: UInt32, timestamp: UInt32)
			virtual void onTxUpdated(const std::string &hash, uint32_t blockHeight, uint32_t timeStamp);

			// func txDeleted(_ txHash: UInt256, notifyUser: Bool, recommendRescan: Bool)
			virtual void onTxDeleted(const std::string &hash, bool notifyUser, bool recommendRescan);
		public:
			// func syncStarted()
			virtual void syncStarted();

			// func syncStopped(_ error: BRPeerManagerError?)
			virtual void syncStopped(const std::string &error);

			// func txStatusUpdate()
			virtual void txStatusUpdate();

			// func saveBlocks(_ replace: Bool, _ blocks: [BRBlockRef?])
			virtual void saveBlocks(bool replace, const SharedWrapperList<MerkleBlock, BRMerkleBlock *> &blocks);

			// func savePeers(_ replace: Bool, _ peers: [BRPeer])
			virtual void savePeers(bool replace, const WrapperList<Peer, BRPeer> &peers);

			// func networkIsReachable() -> Bool
			virtual bool networkIsReachable();

			// Called on publishTransaction
			virtual void txPublished(const std::string &error);

		protected:
			virtual SharedWrapperList<Transaction, BRTransaction *> loadTransactions();

			virtual SharedWrapperList<MerkleBlock, BRMerkleBlock *> loadBlocks();

			virtual WrapperList<Peer, BRPeer> loadPeers();

			virtual int getForkId() const;

			virtual const PeerManagerListenerPtr &createPeerManagerListener();

			virtual const WalletListenerPtr &createWalletListener();

		private:
			DatabaseManager _databaseManager;
			BackgroundExecutor _executor;
			MasterPubKeyPtr _masterPubKey;
			ByteData _phraseData;

			std::vector<Wallet::Listener *> _walletListeners;
			std::vector<PeerManager::Listener *> _peerManagerListeners;
		};

	}
}

#endif //__ELASTOS_SDK_WALLETMANAGER_H__