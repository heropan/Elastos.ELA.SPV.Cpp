// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_MERKLEBLOCKDATASOURCE_H__
#define __ELASTOS_SDK_MERKLEBLOCKDATASOURCE_H__

#include "Sqlite.h"
#include "TableBase.h"

#include <Core/BRInt.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace Elastos {
	namespace ElaWallet {

		struct MerkleBlockEntity {
			MerkleBlockEntity() :
				id(0),
				blockHeight(0)
			{
			}

			MerkleBlockEntity(long i, CMBlock bytes, uint32_t h) :
				id(i),
				blockBytes(bytes),
				blockHeight(h)
			{
			}

			long id;
			CMBlock blockBytes;
			uint32_t blockHeight;
		};

		class MerkleBlockDataSource : public TableBase {

		public:
			MerkleBlockDataSource(Sqlite *sqlite);
			MerkleBlockDataSource(SqliteTransactionType type, Sqlite *sqlite);
			~MerkleBlockDataSource();

			bool PutMerkleBlock(const std::string &iso, const MerkleBlockEntity &blockEntity);
			bool PutMerkleBlocks(const std::string &iso, const std::vector<MerkleBlockEntity> &blockEntities);
			bool DeleteMerkleBlock(const std::string &iso, const MerkleBlockEntity &blockEntity);
			bool DeleteAllBlocks(const std::string &iso);
			std::vector<MerkleBlockEntity> GetAllMerkleBlocks(const std::string &iso) const;

		private:
			bool PutMerkleBlockInternal(const std::string &iso, const MerkleBlockEntity &blockEntity);


		private:
			/*
			 * merkle block table
			 */
			const std::string MB_TABLE_NAME = "merkleBlockTable";
			const std::string MB_COLUMN_ID = "_id";
			const std::string MB_BUFF = "merkleBlockBuff";
			const std::string MB_HEIGHT = "merkleBlockHeight";
			const std::string MB_ISO = "merkleBlockISO";

			const std::string MB_DATABASE_CREATE = "create table if not exists " + MB_TABLE_NAME + " (" +
				MB_COLUMN_ID + " integer primary key autoincrement, " +
				MB_BUFF + " blob, " +
				MB_HEIGHT + " integer, " +
				MB_ISO + " text DEFAULT 'ELA');";
		};

	}
}


#endif //__ELASTOS_SDK_MERKLEBLOCKDATASOURCE_H__

