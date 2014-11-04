/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_NODE_H
#define HOARE_NODE_H

#include <string>
#include <vector>

namespace llvm {
	class Value;
}

// TODO: the code generation should not be in the "parser" dir.

namespace hoare {

/*
 * TODO: Be more clever with passed references.
 */

class Context;
class NExpression;
class NStatement;

typedef std::vector<NExpression *> NExpressionList;
typedef std::vector<NStatement *> NStatementList;

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

/*
 * TODO Rename to Block or something like that
 */
class NCode : public Node
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
	NName(std::string &name)
		: name(name)
	{
		line = -1;
		startColumn = 0;
		endColumn = 0;
	}

	virtual llvm::Value * generateValue(Context *context) override;

public:
	std::string name;
	int line;
	int startColumn, endColumn;
};

class NString : public NExpression
{
public:
	NString(std::string value)
		: value(value)
	{
	}

	virtual llvm::Value * generateValue(Context *context) override;

public:
	std::string value;
};

class NNumeric : public NExpression
{
public:
	NNumeric(unsigned long long value)
		: value(value)
	{
	}

	virtual llvm::Value * generateValue(Context *context) override;

public:
	unsigned long long value;
};

class NDeclaration : public NStatement
{
public:
	explicit NDeclaration(NName &left, NName &right)
		: left(left), right(right)
	{
	}

	virtual llvm::Value * generateValue(Context *context) override;

public:
	NName left, right;
};

class NAssign : public NStatement
{
public:
	explicit NAssign(NName &left, NNumeric &right)
		: left(left), right(right)
	{
	}

	virtual llvm::Value * generateValue(Context *context) override;

public:
	NName left;
	NNumeric right;
};

class NFunctionCall : public NStatement
{
public:
	explicit NFunctionCall(NExpressionList &args)
		: args(args)
	{
	}

	virtual llvm::Value * generateValue(Context *context) override;

public:
	NExpressionList &args;
};


}

#endif /* HOARE_NODE_H */

