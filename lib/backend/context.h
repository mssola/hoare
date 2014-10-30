/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_CONTEXT_H
#define HOARE_CONTEXT_H

#include "blocks.h"
#include <parser/node.h>
#include <llvm/IR/Function.h>

namespace hoare {

class Context
{
public:
	Context();

	void generateCode(NCode *code);
	void run();

	inline llvm::Module * getModule() const
	{
		return module;
	}

	// TODO
	inline Blocks getBlocks() const
	{
		return blocks;
	}

private:
	llvm::Module *module;
	llvm::Function *main;
	Blocks blocks;
};

}

#endif /* HOARE_CONTEXT_H */

