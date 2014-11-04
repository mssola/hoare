/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "context.h"
#include "builtins.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <iostream>

using namespace hoare;

Context::Context(const std::string &path)
	: path(path), problems(Problems(path))
{
	module = new llvm::Module("main", llvm::getGlobalContext());
}

void Context::generateCode(NCode *code)
{
	// Add the builtins.
	Builtins builtins;
	builtins.addInContext(this);

	// Create the top level interpreter function to call as entry.
	std::vector<llvm::Type *> argTypes;
	auto mainType = llvm::FunctionType::get(
		llvm::Type::getInt32Ty(llvm::getGlobalContext()),
		llvm::makeArrayRef(argTypes),
		false
	);
	main = llvm::Function::Create(
		mainType,
		llvm::GlobalValue::ExternalLinkage,
		"main", module
	);
	auto bblock = llvm::BasicBlock::Create(
		llvm::getGlobalContext(),
		"entry", main, 0
	);

	/* Push a new variable/block context */
	blocks.push(bblock);
	code->generateValue(this);
	problems.print();

    auto zero = llvm::Constant::getNullValue(
		llvm::IntegerType::getInt32Ty(llvm::getGlobalContext())
	);
	llvm::ReturnInst::Create(llvm::getGlobalContext(), zero, bblock);
	blocks.pop();
}

void Context::run()
{
	std::string str;
	std::vector<llvm::GenericValue> noargs;

	auto ee = llvm::EngineBuilder(module).setErrorStr(&str).create();
	if (str != "") {
		Problem problem(0, 0, str);
		problem.print(path);
	} else {
		ee->runFunction(main, noargs);
	}
}

