/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include <codegen/printer.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/PassManager.h>
#include <llvm/Support/raw_ostream.h>

using namespace hoare;

Printer::Printer()
{
}

void Printer::print(const Context &context)
{
	auto module = context.module;

	llvm::PassManager pm;
	pm.add(llvm::createPrintModulePass(llvm::outs()));
	pm.run(*module);
}
