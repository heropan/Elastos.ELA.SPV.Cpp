// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <fstream>
#include <iconv.h>
#include "Utils.h"

#include "BigIntFormat.h"
#include "BRCrypto.h"
#include "BRBIP39Mnemonic.h"
#include "BRBIP39WordsEn.h"
#include "brBIP39WordsCh.h"
#include "brBIP39WordsFrench.h"
#include "brBIP39WordsItalian.h"
#include "brBIP39WordsJapanese.h"
#include "brBIP39WordsSpanish.h"
#include "BTCBase58.h"
#include "Wallet_Tool.h"

#define MAX_BUFFER 1024 * 6

namespace Elastos {
	namespace SDK {
		CMemBlock<char>
		Wallet_Tool::_Code_Convert(const std::string from_charset, const std::string to_charset,
								   const CMemBlock<char> &input) {
			CMemBlock<char> output;
			if (true == input) {
				iconv_t cd;
				char **pin = &input;
				static char sc_tmp[MAX_BUFFER] = {0};
				char *out[1] = {sc_tmp};
				size_t inlen = input.GetSize(), outlen = MAX_BUFFER;

				cd = iconv_open(to_charset.c_str(), from_charset.c_str());
				if (nullptr != cd) {
					if (-1 != iconv(cd, pin, &inlen, &out[0], &outlen) && 0 < outlen) {
						output.Resize(MAX_BUFFER - outlen);
						memcpy(output, sc_tmp, output.GetSize());
					}
					iconv_close(cd);
				}
			}

			return output;
		}

		CMemBlock<uint8_t> Wallet_Tool::GetRandom(size_t bits) {
			size_t bytes = 0 == bits % 8 ? bits / 8 : bits / 8 + 1;
			CMemBlock<uint8_t> out(bytes);
			for (size_t i = 0; i < bytes; i++) {
				out[i] = Utils::getRandomByte();
			}
			return out;
		}

		CMemBlock<char> Wallet_Tool::U8ToU16(const CMemBlock<char> &input) {
			return _Code_Convert("utf-8", "utf-16", input);
		}

		CMemBlock<char> Wallet_Tool::U16ToU8(const CMemBlock<char> &input) {
			return _Code_Convert("utf-16", "utf-8", input);
		}

		CMemBlock<uint8_t> Wallet_Tool::GenerateSeed128() {
			return GetRandom(128);
		}

		CMemBlock<uint8_t> Wallet_Tool::GenerateSeed256() {
			return GetRandom(256);
		}

		std::string Wallet_Tool::GenerateEntropySource(const CMemBlock<uint8_t> &seed) {
			std::string out = "";
			if (true == seed) {
				uint8_t t[32];
				BRSHA256_2(t, seed, seed.GetSize());
				CMemBlock<uint8_t> tmp;
				tmp.SetMemFixed(t, sizeof(t));
				CMemBlock<char> cb_entropy = Hex2Str(tmp);
				out = (const char *) cb_entropy;
			}
			return out;
		}

		std::string Wallet_Tool::getDeriveKeyFromEntropySource_base58(const std::string &seed) {
			std::string out = "";
			if (seed != "") {
				CMemBlock<char> cb_seed;
				cb_seed.SetMemFixed(seed.c_str(), seed.size() + 1);
				CMemBlock<uint8_t> entropySource = Str2Hex(cb_seed);
				if (true == entropySource) {
					uint8_t key64[64] = {0};
					uint8_t key[] = "reqPrivKey";
					BRHMAC(key64, BRSHA512, 512 / 8, key, strlen((const char *) key), entropySource,
						   entropySource.GetSize());
					CMemBlock<uint8_t> tmp;
					tmp.SetMemFixed(key64, sizeof(key64));
					out = BTCBase58::EncodeBase58((unsigned char *) key64, sizeof(key64));
				}
			}
			return out;
		}

		std::string Wallet_Tool::getStringFromSeed(const CMemBlock<uint8_t> &seed) {
			std::string out = "";
			if (true == seed) {
				CMemBlock<char> str = Hex2Str(seed);
				out = (const char *) str;
			}
			return out;
		}

		CMemBlock<uint8_t> Wallet_Tool::getSeedFromString(const std::string &str_seed) {
			CMemBlock<uint8_t> out;
			if (str_seed != "") {
				CMemBlock<char> str_chg;
				str_chg.SetMemFixed(str_seed.c_str(), str_seed.size() + 1);
				out = Str2Hex(str_chg);
			}
			return out;
		}

		CMemBlock<char> Wallet_Tool::GeneratePhraseFromSeed(const CMemBlock<uint8_t> &seed, const std::string &language) {
			CMemBlock<char> out;
			if (true == seed) {
				const char **wordList = language == "english" ? BRBIP39WordsEn :
										language == "chinese" ? brBIP39WordsCh :
										language == "french" ? brBIP39WordsFrench :
										language == "italian" ? brBIP39WordsItalian :
										language == "japanese" ? brBIP39WordsJapanese :
										language == "spanish" ? brBIP39WordsSpanish : nullptr;
				size_t phraselen = BRBIP39Encode(nullptr, 0, wordList, seed, seed.GetSize());
				out.Resize(phraselen);
				phraselen = BRBIP39Encode(out, phraselen, wordList, seed, seed.GetSize());
			}
			return out;
		}

		bool Wallet_Tool::PhraseIsValid(const CMemBlock<char> &phrase, const std::string &language) {
			bool out = false;
			if (true == phrase) {
				const char **wordList = language == "english" ? BRBIP39WordsEn :
										language == "chinese" ? brBIP39WordsCh :
										language == "french" ? brBIP39WordsFrench :
										language == "italian" ? brBIP39WordsItalian :
										language == "japanese" ? brBIP39WordsJapanese :
										language == "spanish" ? brBIP39WordsSpanish : nullptr;
				out = 1 == BRBIP39PhraseIsValid(wordList, phrase) ? true : false;
			}
			return out;
		}

		CMemBlock<uint8_t> Wallet_Tool::getSeedFromPhrase(const CMemBlock<char> &phrase, const std::string &language) {
			CMemBlock<uint8_t> out;
			if (true == phrase) {
				const char **wordList = language == "english" ? BRBIP39WordsEn :
										language == "chinese" ? brBIP39WordsCh :
										language == "french" ? brBIP39WordsFrench :
										language == "italian" ? brBIP39WordsItalian :
										language == "japanese" ? brBIP39WordsJapanese :
										language == "spanish" ? brBIP39WordsSpanish : nullptr;
				size_t datalen = BRBIP39Decode(nullptr, 0, wordList, phrase);
				out.Resize(datalen);
				datalen = BRBIP39Decode(out, datalen, wordList, phrase);
			}
			return out;
		}

		std::string Wallet_Tool::getDeriveKey_base58(CMemBlock<char> &phrase, const std::string &passphrase) {
			uint8_t prikey[64] = {0};
			BRBIP39DeriveKey(prikey, phrase, passphrase.c_str());
			return BTCBase58::EncodeBase58((unsigned char *) prikey, 64);
		}
	}
}