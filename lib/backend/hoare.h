/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_HOARE_H
#define HOARE_HOARE_H

#include <string>

namespace hoare {

class Hoare
{
public:
	Hoare();
	explicit Hoare(const char *path);

	int compile(bool print = false) const;

private:
	std::string path;
};

}

#endif /* HOARE_HOARE_H */

