/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include <ast/types.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

using namespace hoare;

llvm::Type * Types::of(std::string &name)
{
	if (name == "integer") {
		return llvm::Type::getInt64Ty(llvm::getGlobalContext());
	} else if (name == "character") {
		return llvm::Type::getInt8Ty(llvm::getGlobalContext());
	}
	return nullptr;
}
