// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_PARAMCHECKER_H__
#define __ELASTOS_SDK_PARAMCHECKER_H__

#include <string>

#include "CMemBlock.h"
#include "nlohmann/json.hpp"

namespace Elastos {
	namespace SDK {

		class ParamChecker {
		public:
			static void checkNullPointer(void *pointer, bool isParam = true);

			static void checkPassword(const std::string &password, bool isParam = true);

			static void checkPasswordWithNullLegal(const std::string &password, bool isParam = true);

			static void checkNotEmpty(const std::string &message, bool isParam = true);

			static void checkDataNotEmpty(const CMBlock &mem, bool isParam = true);

			static void checkJsonArrayNotEmpty(nlohmann::json jsonData, bool isParam = true);
		};

	}
}

#endif //__ELASTOS_SDK_PARAMCHECKER_H__
