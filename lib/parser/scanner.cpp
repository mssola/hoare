/*
 * Copyright (C) 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
 * This file is licensed under the MIT license.
 * See the LICENSE file.
 */

#include <iostream>
#include <ast/node.h>
#include <parser/driver.h>
#include <parser/scanner.h>
#include <parser/parser.hpp>

#define IS_EOF() ((unsigned int) (actual - contents) >= length)

using namespace hoare;

Scanner::Scanner()
	: Scanner("")
{
}

Scanner::Scanner(const std::string &filename)
	: path(filename)
{
	line = 0;
	column = 0;
	prevc = 0;
	length = 0;
	contents = nullptr;
	actual = nullptr;
}

Scanner::~Scanner()
{
	if (contents) {
		delete contents;
	}
}

bool Scanner::readFile()
{
	/* Open specified file */
	FILE *fd = fopen(path.c_str(), "r");
	if (!fd) {
		return false;
	}

	fseek(fd, 0, SEEK_END);
	length = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	if (!length) {
		fclose(fd);
		return false;
	}
	contents = (char *) malloc(sizeof(char) * length);

	if (!contents) {
		fclose(fd);
		return false;
	}
	fread(contents, length, 1, fd);
	if (ferror(fd)) {
		fclose(fd);
		return false;
	}
	actual = contents;
	fclose(fd);
	return true;
}

int Scanner::lex(YYSTYPE *lval, Driver *driver)
{
	int c, aux;
	char *prev;
	std::string name = "";

retry:
	prev = actual;
	c = nextc();
	counter = 0;

	switch (c) {
	case EOF:
	case '\0':
		return tEND;
	case ' ': case '\t': case '\f': case '\r': case '\n': case '\13':
		goto retry;
	case '#':
		for (c = nextc(); c != EOF && c != '\n'; c = nextc());
		goto retry;
	case ':':
		if (*actual == '=') {
			nextc();
			counter++;
			return tASSIGN;
		} else if (*actual == ':') {
			nextc();
			counter++;
			return tCOLON2;
		}
		break;
	case '.':
		if (*actual == '.') {
			nextc();
			counter++;
			if (*actual == '.') {
				nextc();
				counter++;
				return tELLIPSIS;
			}
			return tDOT2;
		}
		break;
	case '|':
		if (*actual == '|') {
			nextc();
			counter++;
			return tOR;
		}
		break;
	case '[':
		if (*actual == ']') {
			nextc();
			counter++;
			return tSQUARE;
		}
		break;
	case '-':
		if (*actual == '>') {
			nextc();
			counter++;
			return tARROW;
		}
		break;
	case '=':
		if (*actual == '=') {
			nextc();
			counter++;
			return tEQ;
		}
		break;
	case '<':
		if (*actual == '=') {
			nextc();
			counter++;
			return tLEQ;
		}
		return tLT;
	case '>':
		if (*actual == '=') {
			nextc();
			counter++;
			return tGEQ;
		}
		return tGT;
	case '!':
		if (*actual == '=') {
			nextc();
			counter++;
			return tNE;
		}
		break;
	case '"':
		c = nextc();
		counter++;
		while (c != '"') {
			if (c == EOF) {
				driver->addProblem("unterminated string");
				return tEND;
			}
			if (c == '\\') {
				c = readEscape(driver);
				if (c == 0) {
					return tEND;
				}
			}
			name += c;
			c = nextc();
			counter++;
		}
		lval->name = new std::string(name);
		return tSTRING;
	default:
		// Check if this is a numeric value.
		if (isdigit(c)) {
			do {
				name += c;
				c = nextc();
				counter++;
			} while (isdigit(c));
			counter--;
			pushback();
			lval->numeric = std::stoll(name);
			return tNUM;
		}

		// Check if this is a name.
		if (!isUtf8Alpha(prev) && *prev != '_') {
			break;
		}

		do {
			name += c;
			prev = actual;
			c = nextc();
			counter++;
		} while (isValidIdentifier(prev));
		counter--;
		pushback();

		// Check if this is a keyword.
		if (name == "printf") {
			return tPRINTF;
		}

		lval->name = new std::string(name);
		return tNAME;
	}
	return c;
}

unsigned int Scanner::nextc()
{
	int c;

	if (IS_EOF()) {
		return EOF;
	}

	c = (unsigned char) *actual++;
	prevc = column;
	if (c == '\n') {
		line++;
		column = -1;
	}
	column++;
	return c;
}

void Scanner::pushback()
{
	column--;
	actual--;
	if (*actual == '\n') {
		line--;
		column = prevc;
	}
}

char Scanner::readEscape(Driver *driver)
{
	int c;

	// Octals, \h, \u escape characters are not supported, too lazy :P

	switch (c = nextc()) {
	case '\\':
		return c;
	case '0':
		return '\0';
	case 'n':
		return '\n';
	case 't':
		return '\t';
	case 'r':
		return '\r';
	case 'f':
		return '\f';
	case 'v':
		return '\13';
	case 'a':
		return '\007';
	case 'e':
		return 033;
	case 'b':
		return '\010';
	case '\'':
		return '\'';
	case '"':
		return '"';
	}

	driver->addProblem("unknown escape character");
	return 0;
}

/*
 * Some macros to make easier the UTF-8 support
 */
#define isUtf(c) ((c & 0xC0) != 0x80)
#define isSpecial(c) (utf8CharSize(c) > 1)

int Scanner::utf8CharSize(const char *s)
{
	int size = 0;
	int i = 0;

	do {
		i++;
		size++;
	} while (s[i] && !isUtf(s[i]));
	return size;
}

int Scanner::isUtf8Alpha(const char *str)
{
	return isSpecial(str) ? 1 : isalpha(*str);
}

bool Scanner::isValidIdentifier(const char *p)
{
	return (*p == '_' || isdigit(*p) || isUtf8Alpha(p));
}

