/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include "problem.h"

using namespace hoare;

Problem::Problem(unsigned int l, unsigned int c, std::string m)
{
	line = l;
	column = c;
	message = m;
}

