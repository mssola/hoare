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
	// First of all we have to parse the given file.
	Driver driver;
	driver.parse(path);
	if (!driver.code) {
		driver.problems.print();
		return 1;
	}

	// LLVM requires this, otherwise it will crash.
	llvm::InitializeNativeTarget();

	// Generate the code.
	Context context(path, driver.problems);
	context.generateCode(driver.code);
	if (!context.problems.empty()) {
		return 1;
	}

	// The code could be compiled, now it's time to print/execute it.
	if (print) {
		Printer printer;
		printer.print(context);
	} else {
		context.run();
	}

	return 0;
}

