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

#ifndef HOARE_CONTEXT_H
#define HOARE_CONTEXT_H

#include <vector>

#include <llvm/IR/Function.h>

#include <ast/node.h>
#include <codegen/blocks.h>
#include <utils/problem.h>

namespace hoare {

class Context
{
public:
	Context(const std::string &path, const Problems &problems);

	void generateCode(NBlock *code);
	void run();

public:
	Problems problems;
	Blocks blocks;
	llvm::Module *module;

private:
	std::string m_path;
	llvm::Function *m_main;
};

}

#endif // HOARE_CONTEXT_H
