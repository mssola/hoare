/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
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

#include <codegen/context.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <codegen/builtins.h>

using namespace hoare;

Context::Context(const std::string &path, const Problems &problems)
	: problems(problems)
	, m_path(path)
{
	module = new llvm::Module("main", llvm::getGlobalContext());
}

void Context::generateCode(NBlock *code)
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
	m_main = llvm::Function::Create(
		mainType,
		llvm::GlobalValue::ExternalLinkage,
		"main", module
	);
	auto bblock = llvm::BasicBlock::Create(
		llvm::getGlobalContext(),
		"entry", m_main, 0
	);

	// Push a new variable/block context.
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
		problem.print(m_path);
	} else {
		ee->runFunction(m_main, noargs);
	}
}
