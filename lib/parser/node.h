/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_NODE_H
#define HOARE_NODE_H

#include <vector>
#include "problem.h"

namespace llvm {
	class Value;
}

namespace hoare {

class Context;
class NExpression;

typedef std::vector<NExpression *> NExpressionList;

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

class NCode : public Node
{
public:
	virtual llvm::Value * generateValue(Context *context) override;

public:
	std::vector<NStatement*> statements;
};

class NName : public NExpression
{
public:
	NName(std::string &name)
		: name(name)
	{
	}

public:
	std::string &name;
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

class NAssign : public NStatement
{
public:
	explicit NAssign(NName &left, NNumeric &right)
		: left(left), right(right)
	{
	}

public:
	NName &left;
	NNumeric &right;
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

