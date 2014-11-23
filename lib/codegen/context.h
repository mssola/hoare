/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_CONTEXT_H
#define HOARE_CONTEXT_H

#include "blocks.h"
#include <ast/node.h>
#include <utils/problem.h>
#include <llvm/IR/Function.h>
#include <vector>

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

#endif /* HOARE_CONTEXT_H */
