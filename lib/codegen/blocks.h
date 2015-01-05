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

#ifndef HOARE_BLOCKS_H
#define HOARE_BLOCKS_H

#include <map>
#include <memory>
#include <set>
#include <stack>

namespace llvm {
	class BasicBlock;
	class Value;
}

namespace hoare {

using Block = struct BlockType {
	llvm::BasicBlock *block;
	std::map<std::string, llvm::Value *> locals;
	std::set<std::string> unused;
};

class Blocks
{
public:
	void push(llvm::BasicBlock *block);
	void pop();

	inline std::shared_ptr<Block> top() const
	{
		return m_blocks.top();
	}

	inline llvm::BasicBlock * current() const
	{
		return m_blocks.top()->block;
	}

	inline std::map<std::string, llvm::Value *> & locals() const
	{
		return m_blocks.top()->locals;
	}

private:
	std::stack<std::shared_ptr<Block>> m_blocks;
};

}

#endif // HOARE_BLOCKS_H
