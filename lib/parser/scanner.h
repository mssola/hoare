/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#ifndef HOARE_LEXER_H
#define HOARE_LEXER_H

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

#endif /* HOARE_LEXER_H */

