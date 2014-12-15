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

#include <codegen/printer.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

using namespace hoare;

namespace {

std::string && filename(const std::string &path)
{
	int idx = path.find_last_of(".");
	return std::move(path.substr(0, idx) + ".ll");
}

}

Printer::Printer()
{
}

void Printer::print(const Context &context)
{
	auto module = context.module;
	std::string file = filename(context.path());
	std::string ec;

	llvm::raw_fd_ostream stream(file.c_str(), ec, llvm::sys::fs::F_RW);
	llvm::PassManager pm;
	pm.add(llvm::createPrintModulePass(stream));
	pm.run(*module);
	stream.close();
}
