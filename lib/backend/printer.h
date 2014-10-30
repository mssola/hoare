/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_PRINTER_H
#define HOARE_PRINTER_H

#include "context.h"

namespace hoare {

class Printer
{
public:
	Printer();

	void print(const Context &context);
};

}

#endif /* HOARE_PRINTER_H */

