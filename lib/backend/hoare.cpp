/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "hoare.h"
#include "context.h"
#include "printer.h"
#include <parser/driver.h>
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

int Hoare::compile(bool print)
{
	Driver parser;
	parser.parse(path);
	if (!parser.code) {
		return 1;
	}

	llvm::InitializeNativeTarget();

	Context context(path);
	context.generateCode(parser.code);
	if (print) {
		Printer printer;
		printer.print(context);
	} else {
		context.run();
	}

	return 0;
}

