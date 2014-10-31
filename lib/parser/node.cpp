/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "node.h"
#include <backend/context.h>
#include <backend/types.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <iostream>

using namespace hoare;

Node::Node()
{
}

Node::~Node()
{
}

llvm::Value * Node::generateValue(Context *context)
{
	return nullptr;
}

llvm::Value * NCode::generateValue(Context *context)
{
	llvm::Value *last = nullptr;

	for (auto &it : statements) {
		last = it->generateValue(context);
	}
	return last;
}

llvm::Value * NNumeric::generateValue(Context *context)
{
	return llvm::ConstantInt::get(
		llvm::Type::getInt64Ty(llvm::getGlobalContext()),
		value, true
	);
}

llvm::Value * NDeclaration::generateValue(Context *context)
{
	auto type = Types::of(right.name);
	if (!type) {
		std::cout << "Error: type `" << right.name << "` not found."
			<< std::endl;
		return nullptr;
	}

	auto alloc = new llvm::AllocaInst(type, left.name.c_str(),
		context->getBlocks().current());
	context->getBlocks().locals()[right.name] = alloc;
	return alloc;
}

llvm::Value * NString::generateValue(Context *context)
{
    auto format = llvm::ConstantDataArray::getString(
		llvm::getGlobalContext(), value.c_str()
	);

	auto module = context->getModule();
    auto var = new llvm::GlobalVariable(
        *module,
		llvm::ArrayType::get(
			llvm::IntegerType::get(llvm::getGlobalContext(), 8),
			strlen(value.c_str())+1
		),
		true, llvm::GlobalValue::PrivateLinkage, format, ".str"
	);

    auto zero = llvm::Constant::getNullValue(
		llvm::IntegerType::getInt32Ty(llvm::getGlobalContext())
	);

    std::vector<llvm::Constant *> indices;
    indices.push_back(zero);
    indices.push_back(zero);

    return llvm::ConstantExpr::getGetElementPtr(var, indices);
}

llvm::Value * NFunctionCall::generateValue(Context *context)
{
	// Get the printn function.
	auto function = context->getModule()->getFunction("printf");
	if (!function) {
		std::cout << "Error: function `printf` not found." << std::endl;
		return nullptr;
	}

	// Push the arguments.
	std::vector<llvm::Value *> arguments;
	for (auto &a : args) {
		arguments.push_back(a->generateValue(context));
	}

	// And get a "call" instruction.
	return llvm::CallInst::Create(
		function,
		llvm::makeArrayRef(arguments), "",
		context->getBlocks().current()
	);
}

