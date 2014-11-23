/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include <codegen/builtins.h>

#include <iostream>

#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>

#include <codegen/context.h>

using namespace hoare;

llvm::Function * Builtins::insertPrintFunction(Context *context)
{
	// The first argument: char *. The rest of arguments are specified later as
	// a variable length list.
    std::vector<llvm::Type *> args;
    args.push_back(llvm::Type::getInt8PtrTy(llvm::getGlobalContext()));

	auto printf = llvm::FunctionType::get(
		llvm::Type::getInt32Ty(llvm::getGlobalContext()), args, true
	);

	auto func = llvm::Function::Create(
		printf, llvm::Function::ExternalLinkage,
		llvm::Twine("printf"),
		context->module
	);
	func->setCallingConv(llvm::CallingConv::C);
	return func;
}

void Builtins::addInContext(Context *context)
{
	auto print = insertPrintFunction(context);
	if (!print) {
		std::cout << "Fatal: could not insert builtin function `printf`."
			<< std::endl;
	}
}
