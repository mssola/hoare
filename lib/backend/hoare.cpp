/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "hoare.h"
#include "context.h"
#include "printer.h"
#include <parser/parser.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/JIT.h>

using namespace hoare;

Hoare::Hoare()
{
}

Hoare::Hoare(const char *path)
	: path(path)
{
}

int Hoare::compile(bool print) const
{
	Parser parser;
	parser.parse(path);

	auto code = parser.code();
	if (!code) {
		return 1;
	}

	llvm::InitializeNativeTarget();

	Context context;
	context.generateCode(code);
	if (print) {
		Printer printer;
		printer.print(context);
	} else {
		context.run();
	}

	return 0;
}

