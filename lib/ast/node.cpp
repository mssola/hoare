/*
 * Copyright (C) 2014-2015 Miquel Sabaté Solà <mikisabate@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ast/node.h>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <ast/types.h>
#include <codegen/context.h>
#include <utils/defines.h>

using namespace hoare;

Node::Node()
{
}

Node::~Node()
{
}

llvm::Value * Node::generateValue(Context *context)
{
	UNUSED(context);

	return nullptr;
}

llvm::Value * NBlock::generateValue(Context *context)
{
	llvm::Value *last = nullptr;

	for (const auto &it : statements) {
		last = it->generateValue(context);
	}
	return last;
}

NName::NName(std::string &n)
	: name(n)
	, line(0)
	, startColumn(0)
	, endColumn(0)
{
}

llvm::Value * NName::generateValue(Context *context)
{
	auto locals = context->blocks.locals();

	if (locals.find(name) == locals.end()) {
		auto msg = "variable `" + name + "` has not been declared in" +
			" this scope";
		context->problems.addProblem(line, startColumn, endColumn, msg);
		return nullptr;
	}

	return new llvm::LoadInst(locals[name], "", false,
		context->blocks.current());
}

NString::NString(std::string v)
	: value(v)
{
}

llvm::Value * NString::generateValue(Context *context)
{
	auto format = llvm::ConstantDataArray::getString(
		llvm::getGlobalContext(), value.c_str()
	);

	auto module = context->module.get();
	auto var = new llvm::GlobalVariable(
		*module,
		llvm::ArrayType::get(
			llvm::IntegerType::get(llvm::getGlobalContext(), 8),
			strlen(value.c_str()) + 1
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

NNumeric::NNumeric(unsigned long long v)
	: value(v)
{
}

llvm::Value * NNumeric::generateValue(Context *context)
{
	UNUSED(context);

	return llvm::ConstantInt::get(
		llvm::Type::getInt64Ty(llvm::getGlobalContext()),
		value, true
	);
}

NDeclaration::NDeclaration(NName &l, NName &r) : left(l), right(r)
{
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
	auto locals = context->blocks.locals();
	if (locals.find(left.name) != locals.end()) {
		auto msg = "variable `" + left.name + "` has already been declared" +
			" in this scope";
		// TODO: show where it has been declared for the first time.
		context->problems.addProblem(left.line, left.startColumn,
			left.endColumn, msg);
		return nullptr;
	}

	// TODO: there's gotta be a better way to declare variables.
	context->blocks.top()->unused.insert(left.name);

	auto alloc = new llvm::AllocaInst(type, left.name.c_str(),
		context->blocks.current());
	context->blocks.locals()[left.name] = alloc;
	return alloc;
}

NAssign::NAssign(NName &l, NNumeric &r) : left(l), right(r)
{
}

llvm::Value * NAssign::generateValue(Context *context)
{
	// TODO
	UNUSED(context);
	return nullptr;
}

NFunctionCall::NFunctionCall(NExpressionList &a) : args(a)
{
}

llvm::Value * NFunctionCall::generateValue(Context *context)
{
	// Get the printn function.
	auto function = context->module->getFunction("printf");
	if (!function) {
		auto msg = "function `printf` not found";
		context->problems << Problem(0, 0, msg);
		return nullptr;
	}

	// Push the arguments.
	std::vector<llvm::Value *> arguments;
	for (const auto &a : args) {
		arguments.push_back(a->generateValue(context));
	}

	// And get a "call" instruction.
	return llvm::CallInst::Create(
		function,
		llvm::makeArrayRef(arguments), "",
		context->blocks.current()
	);
}
