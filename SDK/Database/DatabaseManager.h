/*
 * Copyright (c) 2019 Elastos Foundation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ELASTOS_SDK_DATABASEMANAGER_H__
#define __ELASTOS_SDK_DATABASEMANAGER_H__

#include "MerkleBlockDataSource.h"
#include "TransactionDataStore.h"
#include "PeerDataSource.h"
#include "PeerBlackList.h"
#include "AssetDataStore.h"
#include "TransactionCoinbase.h"
#include "TransactionPending.h"
#include "DIDDataStore.h"
#include "UTXOStore.h"

namespace Elastos {
	namespace ElaWallet {

		class UTXO;
		typedef boost::shared_ptr<UTXO> UTXOPtr;

		class DatabaseManager {
		public:
			DatabaseManager(const boost::filesystem::path &path);
			DatabaseManager();
			~DatabaseManager();

			// CoinBase Transaction
			bool PutCoinbases(const std::vector<TransactionPtr> &entitys);
			bool PutCoinbase(const TransactionPtr &entity);
			bool DeleteAllCoinbase();
			size_t GetCoinbaseTotalCount() const;
			std::vector<TransactionPtr> GetAllCoinbase(const std::string &chainID) const;
			bool UpdateCoinbase(const std::vector<uint256> &txHashes, uint32_t blockHeight, time_t timestamp);
			bool DeleteCoinbase(const uint256 &hash);

			// Confirmed Transaction
			bool PutTransaction(const TransactionPtr &tx);
			bool PutTransactions(const std::vector<TransactionPtr> &txns);
			bool DeleteAllTransactions();
			size_t GetAllTransactionsCount() const;
			TransactionPtr GetTransaction(const uint256& hash, const std::string &chainID);
			std::vector<TransactionPtr> GetAllConfirmedTxns(const std::string &chainID) const;
			bool UpdateTransaction(const std::vector<uint256> &hashes, uint32_t blockHeight, time_t timestamp);
			bool DeleteTxByHash(const uint256 &hash);
			bool DeleteTxByHashes(const std::vector<uint256> &hashes);

			// Pending Transaction
			bool PutPendingTxn(const TransactionPtr &txn);
			bool PutPendingTxns(const std::vector<TransactionPtr> txns);
			bool DeleteAllPendingTxns();
			bool DeletePendingTxn(const uint256 &hash);
			size_t GetPendingTxnTotalCount() const;
			std::vector<TransactionPtr> GetAllPendingTxns(const std::string &chainID) const;
			bool ExistPendingTxnTable() const;

			// Peer Address
			bool PutPeer(const PeerEntity &peerEntity);
			bool PutPeers(const std::vector<PeerEntity> &peerEntities);
			bool DeletePeer(const PeerEntity &peerEntity);
			bool DeleteAllPeers();
			size_t GetAllPeersCount() const;
			std::vector<PeerEntity> GetAllPeers() const;

			// Peer Blacklist
			bool PutBlackPeer(const PeerEntity &entity);
			bool PutBlackPeers(const std::vector<PeerEntity> &entitys);
			bool DeleteBlackPeer(const PeerEntity &entity);
			bool DeleteAllBlackPeers();
			std::vector<PeerEntity> GetAllBlackPeers() const;

			// MerkleBlock
			bool PutMerkleBlock(const MerkleBlockPtr &blockPtr);
			bool PutMerkleBlocks(bool replace, const std::vector<MerkleBlockPtr> &blocks);
			bool DeleteMerkleBlock(long id);
			bool DeleteAllBlocks();
			std::vector<MerkleBlockPtr> GetAllMerkleBlocks(const std::string &chainID) const;

			// Asset
			bool PutAsset(const std::string &iso, const AssetEntity &asset);
			bool DeleteAsset(const std::string &assetID);
			bool DeleteAllAssets();
			bool GetAssetDetails(const std::string &assetID, AssetEntity &asset) const;
			std::vector<AssetEntity> GetAllAssets() const;

			// DID
			bool PutDID(const DIDEntity &didEntity);
			bool UpdateDID(const std::vector<uint256> &hashes, uint32_t blockHeight, time_t timestamp);
			bool DeleteDID(const std::string &did);
			bool DeleteDIDByTxHash(const std::string &txHash);
			bool GetDIDDetails(const std::string &did, DIDEntity &didEntity) const;
			std::string GetDIDByTxHash(const std::string &txHash) const;
			std::vector<DIDEntity> GetAllDID() const;
			bool DeleteAllDID();

			// UTXO store
			bool PutUTXOs(const std::vector<UTXOEntity> &entities);
			std::vector<UTXOEntity> GetUTXOs() const;
			bool DeleteAllUTXOs();
			bool DeleteUTXOs(const std::vector<UTXOEntity> &entities);
			bool ExistUTXOTable() const;

			const boost::filesystem::path &GetPath() const;

			void flush();

		private:
			boost::filesystem::path _path;
			Sqlite                	_sqlite;
			PeerDataSource        	_peerDataSource;
			PeerBlackList           _peerBlackList;
			TransactionCoinbase     _transactionCoinbase;
			TransactionDataStore  	_transactionDataStore;
			TransactionPending      _transactionPending;
			MerkleBlockDataSource 	_merkleBlockDataSource;
			AssetDataStore          _assetDataStore;
			DIDDataStore            _didDataStore;
			UTXOStore               _utxoStore;
		};

	}
}

#endif //__ELASTOS_SDK_DATABASEMANAGER_H__
