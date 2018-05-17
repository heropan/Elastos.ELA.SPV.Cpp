// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "KeyStore/SjclBase64.h"

using namespace Elastos::SDK;

TEST_CASE( "toBits method test", "[SjclBase64]" ) {

	SECTION("test1") {
		const unsigned char expect[] = {0x65, 0x15, 0x63, 0x6B, 0x82, 0xC5, 0xAC, 0x56};
		size_t length = 0;
		const unsigned char *actual = SjclBase64::toBits("ZRVja4LFrFY=", length);
		REQUIRE(length == sizeof(expect));

		for (int i = 0; i < length; ++i) {
			REQUIRE(expect[i] == actual[i]);
		}
	}

	SECTION("test2") {
		const unsigned char expect[] = {0x9F, 0x62, 0x54, 0x4C, 0x9D, 0x3F, 0xCA, 0xB2, 0xDD, 0x08, 0x33, 0xDF, 0x21, 0xCA, 0x80,
										0xCF};
		size_t length = 0;
		const unsigned char *actual = SjclBase64::toBits("n2JUTJ0/yrLdCDPfIcqAzw==", length);
		REQUIRE(length == sizeof(expect));

		for (int i = 0; i < length; ++i) {
			REQUIRE(expect[i] == actual[i]);
		}
	}

}

TEST_CASE( "fromBits method test", "[SjclBase64]" ) {

	SECTION("test1") {
		std::string expect = "ZRVja4LFrFY=";

		const unsigned char bits[] = {0x65, 0x15, 0x63, 0x6B, 0x82, 0xC5, 0xAC, 0x56};
		std::string actual = SjclBase64::fromBits(bits, sizeof(bits));

		REQUIRE(expect == actual);
	}

	SECTION("test2") {
		std::string expect = "n2JUTJ0/yrLdCDPfIcqAzw==";

		const unsigned char bits[] = {0x9F, 0x62, 0x54, 0x4C, 0x9D, 0x3F, 0xCA, 0xB2, 0xDD, 0x08, 0x33, 0xDF, 0x21, 0xCA, 0x80,
									  0xCF};
		std::string actual = SjclBase64::fromBits(bits, sizeof(bits));

		REQUIRE(expect == actual);
	}

}
