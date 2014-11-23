/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_BUILTINS_H
#define HOARE_BUILTINS_H

namespace llvm {
	class Function;
}

namespace hoare {

class Context;

class Builtins
{
public:
	void addInContext(Context *context);

private:
	llvm::Function * insertPrintFunction(Context *context);
};

}

#endif /* HOARE_BUILTINS_H */
