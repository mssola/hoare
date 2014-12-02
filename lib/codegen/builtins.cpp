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

#include <codegen/builtins.h>

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
        std::string str = "could not insert builtin function `printf`";
        context->problems.addProblem(0, 0, str);
	}
}
