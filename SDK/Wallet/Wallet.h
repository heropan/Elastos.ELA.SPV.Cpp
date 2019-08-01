// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_WALLET_H__
#define __ELASTOS_SDK_WALLET_H__

#include <SDK/Common/Lockable.h>
#include <SDK/Common/ElementSet.h>
#include <SDK/Account/SubAccount.h>
#include <SDK/Wallet/GroupedAsset.h>

#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <map>
#include <SDK/Plugin/Transaction/TransactionInput.h>

#define TX_FEE_PER_KB        1000ULL     // standard tx fee per kb of tx size, rounded up to nearest kb
#define TX_OUTPUT_SIZE       34          // estimated size for a typical transaction output
#define TX_INPUT_SIZE        148         // estimated size for a typical compact pubkey transaction input
#define TX_MAX_SIZE          100000      // no tx can be larger than this size in bytes
#define TX_UNCONFIRMED INT32_MAX
#define DEFAULT_FEE_PER_KB (10000)                  // 10 satoshis-per-byte
#define MIN_FEE_PER_KB     TX_FEE_PER_KB                       // bitcoind 0.12 default min-relay fee
#define MAX_FEE_PER_KB     ((TX_FEE_PER_KB*1000100 + 190)/191) // slightly higher than a 10,000bit fee on a 191byte tx
#define TX_MAX_LOCK_HEIGHT   500000000   // a lockTime below this value is a block height, otherwise a timestamp
#define TX_MIN_OUTPUT_AMOUNT (TX_FEE_PER_KB*3*(TX_OUTPUT_SIZE + TX_INPUT_SIZE)/1000) //no txout can be below this amount

namespace Elastos {
	namespace ElaWallet {

		class Transaction;
		class TransactionOutput;
		class TransactionInput;
		class Address;
		class Asset;
		class IPayload;
		class UTXO;
		typedef boost::shared_ptr<Asset> AssetPtr;
		typedef boost::shared_ptr<Transaction> TransactionPtr;
		typedef boost::shared_ptr<IPayload> PayloadPtr;
		typedef boost::shared_ptr<UTXO> UTXOPtr;
		typedef std::vector<UTXOPtr> UTXOArray;
		typedef boost::shared_ptr<TransactionOutput> OutputPtr;
		typedef boost::shared_ptr<TransactionInput> InputPtr;

		class Wallet : public Lockable {
		public:
			class Listener {
			public:
				virtual void balanceChanged(const uint256 &asset, const BigInt &balance) = 0;

				virtual void onCoinBaseTxAdded(const UTXOPtr &utxo) = 0;

				virtual void onCoinBaseUpdatedAll(const UTXOArray &cbs) = 0;

				virtual void onCoinBaseTxUpdated(const std::vector<uint256> &hashes, uint32_t blockHeight,
												 time_t timestamp) = 0;

				virtual void onCoinBaseSpent(const std::vector<uint256> &spentHashes) = 0;

				virtual void onCoinBaseTxDeleted(const uint256 &hash, bool notifyUser, bool recommendRescan) = 0;

				virtual void onTxAdded(const TransactionPtr &tx) = 0;

				virtual void onTxUpdated(const std::vector<uint256> &hashes, uint32_t blockHeight, time_t timeStamp) = 0;

				virtual void onTxDeleted(const uint256 &hash, bool notifyUser, bool recommendRescan) = 0;

				virtual void onTxUpdatedAll(const std::vector<TransactionPtr> &txns) = 0;

				virtual void onAssetRegistered(const AssetPtr &asset, uint64_t amount, const uint168 &controller) = 0;
			};

		public:

			Wallet(uint32_t lastBlockHeight,
				   const std::string &walletID,
				   const std::vector<AssetPtr> &assetArray,
				   const std::vector<TransactionPtr> &txns,
				   const std::vector<UTXOPtr> &utxos,
				   const SubAccountPtr &subAccount,
				   const boost::shared_ptr<Wallet::Listener> &listener);

			virtual ~Wallet();

			void InitListeningAddresses(const std::vector<std::string> &addrs);

			const std::string &GetWalletID() const;

			void SetBlockHeight(uint32_t height);

			nlohmann::json GetBalanceInfo();

			BigInt GetBalanceWithAddress(const uint256 &assetID, const std::string &addr, GroupedAsset::BalanceType type) const;

			// returns the first unused external address
			Address GetReceiveAddress() const;

			size_t GetAllAddresses(std::vector<Address> &addr, uint32_t start, size_t count, bool containInternal);

			Address GetOwnerDepositAddress() const;

			Address GetCROwnerDepositAddress() const;

			Address GetOwnerAddress() const;

			std::vector<Address> GetAllSpecialAddresses() const;

			bytes_ptr GetOwnerPublilcKey() const;

			bool IsVoteDepositAddress(const Address &addr) const;

			// true if the address was previously generated by BRWalletUnusedAddrs() (even if it's now used)
			bool ContainsAddress(const Address &address);

			BigInt GetBalance(const uint256 &assetID, GroupedAsset::BalanceType type) const;

			uint64_t GetFeePerKb() const;

			void SetFeePerKb(uint64_t fee);

			uint64_t GetDefaultFeePerKb();

			TransactionPtr Consolidate(const std::string &memo, const uint256 &asset, bool useVoteUTXO);

			TransactionPtr CreateTransaction(const Address &fromAddress, const std::vector<OutputPtr> &outputs,
											 const std::string &memo,
											 bool useVotedUTXO, bool autoReduceOutputAmount);

			bool ContainsTransaction(const TransactionPtr &transaction);

			bool RegisterTransaction(const TransactionPtr &tx);

			void RemoveTransaction(const uint256 &transactionHash);

			void UpdateTransactions(const std::vector<uint256> &txHashes, uint32_t blockHeight, time_t timestamp);

			TransactionPtr TransactionForHash(const uint256 &transactionHash);

			UTXOPtr CoinBaseTxForHash(const uint256 &txHash) const;

			std::vector<TransactionPtr> GetAllTransactions() const;

			std::vector<UTXOPtr> GetAllCoinBaseTransactions() const;

			bool TransactionIsValid(const TransactionPtr &transaction);

#if 0
			bool TransactionIsPending(const TransactionPtr &transaction);

			bool TransactionIsVerified(const TransactionPtr &transaction);
#endif

			BigInt AmountSentByTx(const TransactionPtr &tx);

			bool IsReceiveTransaction(const TransactionPtr &tx) const;

			bool StripTransaction(const TransactionPtr &tx) const;

			void SignTransaction(const TransactionPtr &tx, const std::string &payPassword);

			void UpdateLockedBalance();

			std::vector<UTXOPtr> GetAllUTXO(const std::string &address) const;

			std::vector<TransactionPtr> TxUnconfirmedBefore(uint32_t blockHeight);

			void SetTxUnconfirmedAfter(uint32_t blockHeight);

			const std::vector<std::string> &GetListeningAddrs() const;

			std::vector<Address> UnusedAddresses(uint32_t gapLimit, bool internal);

			AssetPtr GetAsset(const uint256 &assetID) const;

			nlohmann::json GetAllAssets() const;

			bool AssetNameExist(const std::string &name) const;

		private:
			bool ContainsAsset(const uint256 &assetID) const;

			bool ContainsTx(const TransactionPtr &tx) const;

			bool CoinBaseContains(const uint256 &txHash) const;

			UTXOPtr CoinBaseForHashInternal(const uint256 &txHash) const;

			UTXOPtr RegisterCoinBaseTx(const TransactionPtr &tx);

			void InsertTx(const TransactionPtr &tx);

			int TxCompare(const TransactionPtr &tx1, const TransactionPtr &tx2) const;

			bool TxIsAscending(const TransactionPtr &tx1, const TransactionPtr &tx2) const;

			std::vector<UTXOPtr> GetUTXO(const uint256 &assetID, const std::string &addr) const;

			bool IsAssetUnique(const std::vector<OutputPtr> &outputs) const;

			std::map<uint256, BigInt> BalanceAfterUpdatedTx(const TransactionPtr &tx);

			void BalanceAfterRemoveTx(const TransactionPtr &tx);

			void RemoveSpendingUTXO(const InputArray &inputs);

			void AddSpendingUTXO(const InputArray &inputs);

			void InstallAssets(const std::vector<AssetPtr> &assets);

			void InstallDefaultAsset();

			bool IsUTXOSpending(const UTXOPtr &utxo) const;

			void AddSpendingUTXO(const InputPtr &input);

			void GetSpentCoinbase(const InputArray &inputs, std::vector<uint256> &coinbase) const;

		protected:
			void balanceChanged(const uint256 &asset, const BigInt &balance);

			void coinBaseTxAdded(const UTXOPtr &cb);

			void coinBaseUpdatedAll(const UTXOArray &cbs);

			void coinBaseTxUpdated(const std::vector<uint256> &txHashes, uint32_t blockHeight, time_t timestamp);

			void coinBaseSpent(const std::vector<uint256> &spentHashes);

			void coinBaseDeleted(const uint256 &txHash, bool notifyUser, bool recommendRescan);

			void txAdded(const TransactionPtr &tx);

			void txUpdated(const std::vector<uint256> &txHashes, uint32_t blockHeight, time_t timestamp);

			void txDeleted(const uint256 &txHash, bool notifyUser, bool recommendRescan);

			void txUpdatedAll(const std::vector<TransactionPtr> &txns);

			void assetRegistered(const AssetPtr &asset, uint64_t amount, const uint168 &controller);

		protected:
			friend class GroupedAsset;

			std::string _walletID;

			std::vector<std::string> _listeningAddrs;

			SubAccountPtr _subAccount;

			typedef std::map<uint256, GroupedAssetPtr> GroupedAssetMap;
			mutable GroupedAssetMap _groupedAssets;

			typedef ElementSet<TransactionPtr> TransactionSet;
			std::vector<TransactionPtr> _transactions;
			TransactionSet _allTx;

			UTXOArray _spendingOutputs;
			UTXOArray _coinBaseUTXOs;

			uint64_t _feePerKb;

			uint32_t _blockHeight;
			boost::weak_ptr<Listener> _listener;
		};

		typedef boost::shared_ptr<Wallet> WalletPtr;

	}
}

#endif //__ELASTOS_SDK_WALLET_H__
