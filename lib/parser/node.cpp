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
	// First we check whether the type exists.
	auto type = Types::of(right.name);
	if (!type) {
		auto msg = "type `" + right.name + "` not found";
		// TODO: offer the possibility of "Did you mean?" (like clang).
		context->problems.addProblem(right.line, right.startColumn,
			right.endColumn, msg);
		return nullptr;
	}

	// We cannot re-declare variables.
	auto locals = context->getBlocks().locals();
	if (locals.find(left.name) != locals.end()) {
		auto msg = "variable `" + left.name + "` has already been declared" +
			" in this scope";
		// TODO: show where it has been declared for the first time.
		context->problems.addProblem(left.line, left.startColumn,
			left.endColumn, msg);
		return nullptr;
	}

	auto alloc = new llvm::AllocaInst(type, left.name.c_str(),
		context->getBlocks().current());
	context->getBlocks().locals()[left.name] = alloc;
	return alloc;
}

llvm::Value * NAssign::generateValue(Context *context)
{
	// TODO
	return nullptr;
}

llvm::Value * NName::generateValue(Context *context)
{
	auto locals = context->getBlocks().locals();

	if (locals.find(name) == locals.end()) {
		auto msg = "variable `" + name + "` has not been declared in" +
			" this scope";
		context->problems.addProblem(line, startColumn, endColumn, msg);
		return nullptr;
	}

	return new llvm::LoadInst(locals[name], "", false,
		context->getBlocks().current());
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
		auto msg = "function `printf` not found";
		context->problems << Problem(0, 0, msg);
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

