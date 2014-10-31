/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_TYPES_H
#define HOARE_TYPES_H

#include <string>

namespace llvm {
	class Type;
}

namespace hoare {

class Types
{
public:
	static llvm::Type * of(std::string &name);
};

}

#endif /* HOARE_TYPES_H */

