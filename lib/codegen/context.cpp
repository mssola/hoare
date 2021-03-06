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

#include <codegen/context.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/PassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <ast/node.h>
#include <codegen/builtins.h>

using namespace hoare;

namespace {

std::string filename(const std::string &path)
{
	size_t idx = path.find_last_of(".");
	if (idx == std::string::npos) {
		return path + ".ll";
	}
	return path.substr(0, idx) + ".ll";
}

}

Context::Context(const std::string &path, const Problems &probs)
	: problems(probs)
	, m_path(path)
{
	module = std::make_unique<llvm::Module>("main", llvm::getGlobalContext());
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
	auto main = llvm::Function::Create(
		mainType,
		llvm::GlobalValue::ExternalLinkage,
		"main", module.get()
	);
	auto bblock = llvm::BasicBlock::Create(
		llvm::getGlobalContext(),
		"entry", main, 0
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

bool Context::emit()
{
	std::string file = filename(m_path);
	std::error_code ec;

	llvm::raw_fd_ostream stream(file.c_str(), ec, llvm::sys::fs::F_RW);
	llvm::PassManager pm;
	pm.add(llvm::createPrintModulePass(stream));
	pm.run(*module);
	return true;
}

bool Context::run()
{
	std::string str;
	std::vector<llvm::GenericValue> noargs;
	auto main = module->getFunction("main");

	auto ee = llvm::EngineBuilder(std::move(module))
		.setErrorStr(&str)
		.create();
	if (str != "") {
		Problem problem(0, 0, str);
		problem.print(m_path);
		return false;
	}

	ee->finalizeObject();
	ee->runFunction(main, noargs);
	return true;
}

