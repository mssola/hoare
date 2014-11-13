/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "blocks.h"

namespace hoare {

void Blocks::push(llvm::BasicBlock *block)
{
	auto b = new Block;
	b->block = block;
	blocks.push(b);
}

void Blocks::pop()
{
	auto top = blocks.top();
	blocks.pop();
	delete top;
}

}

