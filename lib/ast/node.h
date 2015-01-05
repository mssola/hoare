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
class NExpression;
class NStatement;

using NExpressionList = std::vector<NExpression *>;
using NStatementList = std::vector<NStatement *>;

class Node
{
public:
	Node();
	virtual ~Node();

	virtual llvm::Value * generateValue(Context *context);
};

class NExpression : public Node
{
};

class NStatement : public Node
{
};

class NBlock : public Node
{
public:
	virtual llvm::Value * generateValue(Context *context) override;

public:
	std::vector<NStatement*> statements;
};

class NParallel : public NStatement
{
public:
	std::vector<NStatement *> statements;
};

class NName : public NExpression
{
public:
	explicit NName(std::string &name);

	virtual llvm::Value * generateValue(Context *context) override;

public:
	std::string name;
	int line;
	int startColumn, endColumn;
};

class NString : public NExpression
{
public:
	explicit NString(std::string value);

	virtual llvm::Value * generateValue(Context *context) override;

public:
	std::string value;
};

class NNumeric : public NExpression
{
public:
	explicit NNumeric(unsigned long long value);

	virtual llvm::Value * generateValue(Context *context) override;

public:
	unsigned long long value;
};

class NDeclaration : public NStatement
{
public:
	explicit NDeclaration(NName &left, NName &right);

	virtual llvm::Value * generateValue(Context *context) override;

public:
	NName left, right;
};

class NAssign : public NStatement
{
public:
	NAssign(NName &left, NNumeric &right);

	virtual llvm::Value * generateValue(Context *context) override;

public:
	NName left;
	NNumeric right;
};

class NFunctionCall : public NStatement
{
public:
	explicit NFunctionCall(NExpressionList &args);

	virtual llvm::Value * generateValue(Context *context) override;

public:
	NExpressionList args;
};

}

#endif // HOARE_NODE_H
