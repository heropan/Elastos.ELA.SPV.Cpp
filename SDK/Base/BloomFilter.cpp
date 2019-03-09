// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "BloomFilter.h"

#include <SDK/Common/Log.h>

#include <Core/BRInt.h>
#include <Core/BRAddress.h>

#include <cfloat>

#define BLOOM_MAX_HASH_FUNCS 50

namespace Elastos {
	namespace ElaWallet {

		BloomFilter::BloomFilter(double falsePositiveRate, size_t elemCount, uint32_t tweak, uint8_t flags) :
				_flags(flags),
				_tweak(tweak) {

			size_t length = (falsePositiveRate < DBL_EPSILON) ? BLOOM_MAX_FILTER_LENGTH :
							(-1.0 / (M_LN2 * M_LN2)) * elemCount * log(falsePositiveRate) / 8.0;
			if (length > BLOOM_MAX_FILTER_LENGTH) length = BLOOM_MAX_FILTER_LENGTH;
			if (length < 1) length = 1;

			_filter.Resize(length);
			memset(_filter, 0, length * sizeof(*_filter));

			_hashFuncs = uint32_t(((length * 8.0) / elemCount) * M_LN2);
			if (_hashFuncs > BLOOM_MAX_HASH_FUNCS) _hashFuncs = BLOOM_MAX_HASH_FUNCS;
		}

		BloomFilter::~BloomFilter() {
		}

		void BloomFilter::Serialize(ByteStream &ostream) const {
			ostream.WriteVarBytes(_filter);
			ostream.WriteUint32(_hashFuncs);
			ostream.WriteUint32(_tweak);
			ostream.WriteByte(_flags);
		}

		//todo add max size check of BLOOM_MAX_FILTER_LENGTH
		bool BloomFilter::Deserialize(ByteStream &istream) {
			if (!istream.ReadVarBytes(_filter)) {
				Log::error("Bloom filter deserialize filter fail");
				return false;
			}

			if (!istream.ReadUint32(_hashFuncs)) {
				Log::error("Bloom filter deserialize hash funcs fail");
				return false;
			}

			if (!istream.ReadUint32(_tweak)) {
				Log::error("Bloom filter deserialize tweak fail");
				return false;
			}

			if (!istream.ReadByte(_flags)) {
				Log::error("Bloom filter deserialize flags fail");
				return false;
			}

			return true;
		}

		nlohmann::json BloomFilter::ToJson() const {
			nlohmann::json jsonData;
			jsonData["length"] = _filter.GetSize();

			std::vector<uint8_t> filters(_filter.GetSize());
			for (int i = 0; i < filters.size(); ++i) {
				filters[i] = _filter[i];
			}
			jsonData["filter"] = filters;
			jsonData["hashFuncs"] = _hashFuncs;
			jsonData["tweak"] = _tweak;

			return jsonData;
		}

		void BloomFilter::FromJson(const nlohmann::json &jsonData) {
			size_t length = jsonData["length"].get<size_t>();

			_filter.Resize(length);
			std::vector<uint8_t> filters = jsonData["filter"];
			assert(length == filters.size());
			for (int i = 0; i < length; ++i) {
				_filter[i] = filters[i];
			}

			_hashFuncs = jsonData["hashFuncs"].get<uint32_t>();
			_tweak = jsonData["tweak"].get<uint32_t>();
		}

		void BloomFilter::InsertData(const void *data, size_t dataLen) {
			size_t i, idx;

			for (i = 0; i < _hashFuncs; i++) {
				idx = CalculateHash(data, dataLen, i);
				_filter[idx >> 3] |= (1 << (7 & idx));
			}

			if (data != nullptr) _elemCount++;
		}

		uint32_t BloomFilter::CalculateHash(const void *data, size_t dataLen, uint32_t hashNum) {
			return BRMurmur3_32(data, dataLen, hashNum * 0xfba4c795 + _tweak) % (_filter.GetSize() * 8);
		}

		bool BloomFilter::ContainsData(const void *data, size_t dataLen) {
			size_t i, idx;

			for (i = 0; i < _hashFuncs; i++) {
				idx = CalculateHash(data, dataLen, i);
				if (!(_filter[idx >> 3] & (1 << (7 & idx)))) return false;
			}

			return data != nullptr ? true : false;
		}
	}
}
