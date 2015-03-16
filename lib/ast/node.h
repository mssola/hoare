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

#ifndef HOARE_NODE_H
#define HOARE_NODE_H

#include <string>
#include <vector>

namespace llvm {
	class Value;
}

namespace hoare {

/*
 * TODO: Be more clever with passed references.
 */

class Context;
struct NExpression;
struct NStatement;

using NExpressionList = std::vector<NExpression *>;
using NStatementList = std::vector<NStatement *>;

struct Node {
	Node();
	virtual ~Node();

	virtual llvm::Value * generateValue(Context *context);
};

struct NExpression : public Node {
};

struct NStatement : public Node {
};

struct NBlock : public Node {
	virtual llvm::Value * generateValue(Context *context) override;

	std::vector<NStatement*> statements;
};

struct NParallel : public NStatement {
	std::vector<NStatement *> statements;
};

struct NName : public NExpression {
	explicit NName(std::string &name);
	virtual llvm::Value * generateValue(Context *context) override;

	std::string name;
	unsigned int line;
	unsigned int startColumn, endColumn;
};

struct NString : public NExpression {
	explicit NString(std::string value);
	virtual llvm::Value * generateValue(Context *context) override;

	std::string value;
};

struct NNumeric : public NExpression {
	explicit NNumeric(unsigned long long value);
	virtual llvm::Value * generateValue(Context *context) override;

	unsigned long long value;
};

struct NDeclaration : public NStatement {
	NDeclaration(NName &left, NName &right);
	virtual llvm::Value * generateValue(Context *context) override;

	NName left, right;
};

struct NAssign : public NStatement {
	NAssign(NName &left, NNumeric &right);
	virtual llvm::Value * generateValue(Context *context) override;

	NName left;
	NNumeric right;
};

struct NFunctionCall : public NStatement {
	explicit NFunctionCall(NExpressionList &args);
	virtual llvm::Value * generateValue(Context *context) override;

	NExpressionList args;
};

}

#endif // HOARE_NODE_H
