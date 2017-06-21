#pragma once

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <exception>
#include <stddef.h>


class StrBuilder
{
public:

	// you need to delete the return value
	char *Clone() {
		char *b = new char[fLen + 1];
		memcpy(b, fBuf, fLen + 1);
		return b;
	}

	char const *Data() {
		return fBuf;
	}

	~StrBuilder() {
		if (fBuf) delete fBuf;
	}

	StrBuilder(char const *str) {
		fBuf = NULL;
		fLen = 0;
		fCap = 0;
		_Append(str, strlen(str));
	}

	StrBuilder(std::string const &str) {
		fBuf = NULL;
		fLen = 0;
		fCap = 0;
		_Append(str.data(), str.size());
	}

	StrBuilder() {
		fBuf = NULL;
		fLen = 0;
		fCap = 0;
	}


	StrBuilder &App() {
		return *this;
	}

	template<class... Args>
	StrBuilder &App(char const *str, Args... args) {
		_Append(str, strlen(str));
		return App(args...);
	}

	template<class... Args>
	StrBuilder &App(int i, Args... args) {
		char str[32];
		sprintf(str, "%d", i);
		_Append(str, strlen(str));
		return App(args...);
	}

	template<class... Args>
	StrBuilder &App(size_t i, Args... args) {
		char str[32];
		sprintf(str, "%zu", i);
		_Append(str, strlen(str));
		return App(args...);
	}

#if !defined(_MSC_VER)
	template<class... Args>
	StrBuilder &App(ssize_t i, Args... args) {
		char str[32];
		sprintf(str, "%zd", i);
		_Append(str, strlen(str));
		return App(args...);
	}
#endif


	template<class... Args>
	StrBuilder &App(std::string const &str, Args... args) {
		_Append(str.data(), str.size());
		return App(args...);
	}

	template<class Arg0, class... Args>
	StrBuilder &Fmt(char const * fmt, Arg0 arg0, Args... args) {
		char const *b = fmt;
		for (size_t i = 0; fmt[i]; ++i) {
			if (fmt[i] == '%') {
				if (fmt[i + 1] == '%') {
					_Append(b, fmt + i - b + 1);
					++i;
				} else {
					_Append(b, fmt + i - b);
					App(arg0);
					return Fmt(fmt + i + 1, args...);
				}
			}
		}
		throw std::exception("there are unused parameter(s)");
	}
	
	StrBuilder &Fmt(char const * fmt) {
		char const *b = fmt;
		for (size_t i = 0; fmt[i]; ++i) {
			if (fmt[i] == '%') {
				throw std::exception("there are too many %");
			}
		}
	}

	template<class... Args>
	StrBuilder &FmtLn(char const * fmt, Args... args) {
		Fmt(fmt, args);
		_Append("\n");
		return *this;
	}


private:
	template<class... Args>
	void _Append(char const *str, size_t len) {
		if (len != 0) {
			if (fLen + len >= fCap) {
				if (fCap == 0) fCap = 20;
				for (; fCap <= fLen + len; fCap += 1, fCap *= 3, fCap /= 2) {
				}
				char *b = new char[fCap];
				if (fBuf) {
					memcpy(b, fBuf, fLen);
					delete fBuf;
				}
				fBuf = b;
			}
			assert(fBuf);
			memcpy(fBuf + fLen, str, len);
			fLen += len;
			fBuf[fLen] = '\0';
		}
	}

	char *fBuf;
	size_t fLen;
	size_t fCap;
};
