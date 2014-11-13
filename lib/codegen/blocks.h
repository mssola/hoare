/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_BLOCKS_H
#define HOARE_BLOCKS_H

#include <map>
#include <stack>

namespace llvm {
	class BasicBlock;
	class Value;
}

namespace hoare {

typedef struct {
	llvm::BasicBlock *block;
	std::map<std::string, llvm::Value *> locals;
} Block;

class Blocks
{
public:
	void push(llvm::BasicBlock *block);
	void pop();

	inline llvm::BasicBlock * current() const
	{
		return blocks.top()->block;
	}

	inline std::map<std::string, llvm::Value *> & locals() const
	{
		return blocks.top()->locals;
	}

private:
	std::stack<Block *> blocks;
};

}

#endif /* HOARE_BLOCKS_H */

