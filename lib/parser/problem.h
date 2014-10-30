/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_PROBLEM_H
#define HOARE_PROBLEM_H

#include <string>

namespace hoare {

/*
 * TODO: maybe this class doesn't need header an all that.
 */
class Problem
{
public:
	explicit Problem(unsigned int l, unsigned int c, std::string m);

public:
	unsigned int line, column;
	std::string message;
};

}

#endif /* HOARE_PROBLEM_H */

