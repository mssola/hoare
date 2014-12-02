/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HOARE_LEXER_H
#define HOARE_LEXER_H

#include <string>

// TODO: free the contents.

union YYSTYPE;

namespace hoare {

class Driver;

class Scanner
{
public:
	Scanner();
	explicit Scanner(const std::string &filename);
	~Scanner();

	/**
	 * TODO: instead of reading the whole file, we read by big chunks, not
	 * closing the file until we reach the end.
	 */
	bool readFile();

	int lex(YYSTYPE *lval, Driver *driver);
	unsigned int nextc();
	void pushback();

private:
	char readEscape(Driver *driver);
	int utf8CharSize(const char *s);
	int isUtf8Alpha(const char *str);
	bool isValidIdentifier(const char *p);

public:
	unsigned int line, column, prevc, counter;
	unsigned int length;
	char *contents, *actual;

private:
	std::string path;
};

}

#endif // HOARE_LEXER_H
