#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <exception>
#include <stddef.h>
#include <type_traits>


namespace details {

	template<class T>
	bool is_char(T const &) {
		return false;
	}

	template<>
	bool is_char(char const &) {
		return true;
	}
	template<>
	bool is_char(signed char const &) {
		return true;
	}
	template<>
	bool is_char(unsigned char const &) {
		return true;
	}

	template<class T>
	bool is_pointer(T const &) {
		return false;
	}

	template<class T>
	bool is_pointer(T const * const &) {
		return true;
	}

	template<class T>
	bool is_pointer(T * const &) {
		return true;
	}

	template<class T, size_t n>
	bool is_pointer(T const (&)[n]) {
		return true;
	}

	template<class T, size_t n>
	bool is_pointer(T (&)[n]) {
		return true;
	}

	template<class T>
	bool is_string(T const &) {
		return false;
	}

	template<>
	bool is_string(std::string const &str) {
		return true;
	}

	template<>
	bool is_string(char const * const &str) {
		return true;
	}

	template<size_t n>
	bool is_string(char const (&str)[n]) {
		return true;
	}

	template<class T>
	bool is_signed(T const &) {
		return std::is_signed<T>::value;
	}

	template<class T>
	bool is_unsigned(T const &) {
		return std::is_unsigned<T>::value;
	}

	template<class T>
	bool is_integral(T const &) {
		return std::is_integral<T>::value;
	}

	template<class T>
	bool is_floating_point(T const &) {
		return std::is_floating_point<T>::value;
	}


	struct FmtBuf {

		void init(char const *fmt, size_t n) {
			if (n <= 20) {
				fBuf = fStaticBuf;
			} else {
				fBuf = new char[n + 1];
			}
			memcpy(fBuf, fmt, n);
			fBuf[n] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, char, char spec) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, int, char spec) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, unsigned, char spec) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, long, char spec) {
			init(fmt, n);
			strcat(fBuf + n, "l");
			n++;
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, unsigned long, char spec) {
			init(fmt, n);
			strcat(fBuf + n, "l");
			n++;
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, long long, char spec) {
			init(fmt, n);
			strcat(fBuf + n, "ll");
			n += 2;
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, unsigned long long, char spec) {
			init(fmt, n);
			strcat(fBuf + n, "ll");
			n += 2;
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}
		FmtBuf(char const *fmt, size_t n, char const*, char spec) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}
		FmtBuf(char const *fmt, size_t n, double, char spec) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		FmtBuf(char const *fmt, size_t n, void **, char spec) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		~FmtBuf() {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
		}
		char *fBuf;
	private:
		char fStaticBuf[32];
	};

	struct DataBuf {


		DataBuf() {
			fLen = 31;
			fBuf = fStaticBuf;
		}
		void Double() {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
			fLen *= 2;
			fBuf = new char[fLen + 1];
		}
		~DataBuf() {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
		}
		size_t fLen;
		char *fBuf;
	private:
		char fStaticBuf[32];
	};

}

class StrBuilder
{
public:

	// you need to delete the return value
	char *Clone() {
		char *b = new char[fLen + 1];
		memcpy(b, fBuf, fLen + 1);
		return b;
	}

	char const *Data() const {
		return fBuf;
	}

	size_t Length() const {
		return fLen;
	}

	~StrBuilder() {
		Clear();
	}

	StrBuilder(char const *str) {
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
		_Append(str, strlen(str));
	}

	StrBuilder(std::string const &str) {
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
		_Append(str.data(), str.size());
	}

	StrBuilder() {
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
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
	StrBuilder &App(StrBuilder const &str, Args... args) {
		_Append(str.Data(), str.Length());
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
	StrBuilder &Fmt(char const * const fmt, Arg0&& arg0, Args&&... args) {
		char const *last = fmt;
		for (size_t i = 0; fmt[i];) {

			if (fmt[i] == '%') {
				if (fmt[i + 1] == '%') {
					_Append(fmt, fmt + i + 1 - last);
					i += 2;
					last = fmt + i;
				} else {
					_Append(last, fmt + i - last);
					int const i_mod = i;
					i += 1; // read `%'

					// read flags
					if (fmt[i] == '-' ||
						fmt[i] == '+' ||
						fmt[i] == '#' || 
						fmt[i] == '0') {
						++i;
					}

					// read width
					size_t width = 0;
					const size_t max_size = (size_t)-1;
					const size_t max_size_div_by_10 = max_size / 10;
					const size_t max_size_sub_by_10 = max_size - 10;

					for (; fmt[i] >= '0' && fmt[i] <= '9'; ++i) {
						if (width > max_size_div_by_10) {
							throw std::logic_error("too large width!");
						} else {
							width *= 10;
							if (width > max_size_sub_by_10) {
								throw std::logic_error("too large width!");
							}
							width += 10;
						}
					}

					// precision
					if (fmt[i] == '.') {
						i += 1;
						for (; fmt[i] >= '0' && fmt[i] <= '9'; ++i) {
						}
					}

					// specifier
					switch (fmt[i]) {
					case 'd': // signed or unsigned integers
					{
						if (details::is_integral(arg0)) {
							_Append0(fmt + i_mod, i - i_mod, arg0);
						} else {
							throw std::logic_error("arg0 is not integral!");
						}
						break;
					}
					case 'o':
					case 'x':
					case 'X':
					{
						if (details::is_integral(arg0)) {
							_AppendInt1(fmt + i_mod, i - i_mod, arg0, fmt[i]);
						} else {
							throw std::logic_error("arg0 is not integral!");
						}
						break;
					}
					case 'i':
					{
						if (details::is_signed(arg0)) {
							_AppendInt1(fmt + i_mod, i - i_mod, arg0, 'i');
						} else {
							throw std::logic_error("arg0 is not signed!");
						}
						break;
					}
					case 'u': // unsigned integers
					{
						if (details::is_unsigned(arg0)) {
							_AppendInt1(fmt + i_mod, i - i_mod, arg0, 'u');
						} else {
							throw std::logic_error("arg0 is not unsigned!");
						}
						break;
					}
					case 'f': // float or double
					case 'F': // float or double
					case 'e': // float or double
					case 'E': // float or double
					case 'g': // float or double
					case 'G': // float or double
					case 'a': // float or double
					case 'A': // float or double
					{
						if (details::is_floating_point(arg0)) {
							_AppendInt1(fmt + i_mod, i - i_mod, arg0, fmt[i]);
						} else {
							throw std::logic_error("arg0 is not floating point!");
						}
						break;
					}
					case 'c': // char
					{
						if (details::is_char(arg0)) {
							_AppendInt1(fmt + i_mod, i - i_mod, arg0, 'c');
						} else {
							throw std::logic_error("arg0 is not floating point!");
						}
						break;
					}
					case 's': // std::string or char const*
					{
						if (details::is_string(arg0)) {
							_Append0(fmt + i_mod, i - i_mod, arg0);
						} else {
							throw std::logic_error("arg0 is not string!");
						}
						break;
					}
					case 'p': // pointer
					{
						if (details::is_pointer(arg0)) {
							_AppendPointer(fmt + i_mod, i - i_mod, arg0, 'p');
						} else {
							throw std::logic_error("arg0 is not pointer!");
						}
						break;
					}
					case 'n': // any type
						break;
					default:
						break;
					}
					i += 1;

					return Fmt(fmt + i, std::forward<Args>(args)...);
				}
			} else {
				i += 1;
			}
		}
		throw std::logic_error("there are unused parameter(s)");
		return *this;
	}
	
	StrBuilder &Fmt(char const * fmt) {
        size_t i = 0;
		for ( ; fmt[i]; ++i) {
			if (fmt[i] == '%') {
				throw std::logic_error("there are too many %");
			}
		}
        _Append(fmt, i);
		return *this;
	}

	template<class... Args>
	StrBuilder &FmtLn(char const * fmt, Args&&... args) {
		Fmt(fmt, std::forward<Args>(args)...);
		_Append("\n", 1);
		return *this;
	}

	void Clear() {
		if (fBuf != fStaticBuf) {
			delete fBuf;
		}
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
	}

private:

	template<class Int>
	void _Append0(char const *fmt_, size_t n, Int const &arg) {
	}
	void _Append0(char const *fmt_, size_t n, signed char arg) {
		_AppendArich(fmt_, n, arg, 'i');
	}
	void _Append0(char const *fmt_, size_t n, short arg) {
		_AppendArich(fmt_, n, arg, 'i');
	}
	void _Append0(char const *fmt_, size_t n, int arg) {
		_AppendArich(fmt_, n, arg, 'i');
	}
	void _Append0(char const *fmt_, size_t n, long arg) {
		_AppendArich(fmt_, n, arg, 'i');
	}
	void _Append0(char const *fmt_, size_t n, long long arg) {
		_AppendArich(fmt_, n, arg, 'i');
	}
	void _Append0(char const *fmt_, size_t n, unsigned char arg) {
		_AppendArich(fmt_, n, arg, 'u');
	}
	void _Append0(char const *fmt_, size_t n, unsigned short arg) {
		_AppendArich(fmt_, n, arg, 'u');
	}
	void _Append0(char const *fmt_, size_t n, unsigned int arg) {
		_AppendArich(fmt_, n, arg, 'u');
	}
	void _Append0(char const *fmt_, size_t n, unsigned long arg) {
		_AppendArich(fmt_, n, arg, 'u');
	}
	void _Append0(char const *fmt_, size_t n, unsigned long long arg) {
		_AppendArich(fmt_, n, arg, 'u');
	}
	void _Append0(char const *fmt_, size_t n, std::string const &arg) {
		_AppendArich(fmt_, n, arg.c_str(), 's');
	}
	void _Append0(char const *fmt_, size_t n, char const *arg) {
		_AppendArich(fmt_, n, arg, 's');
	}

	template<class Int>
	void _AppendInt1(char const *fmt_, size_t n, Int const &arg, char spec) {
	}

	void _AppendInt1(char const *fmt_, size_t n, signed char arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, short arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, int arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, long arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, long long arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, unsigned char arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, unsigned short arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, unsigned int arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, unsigned long arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, unsigned long long arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, double arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}
	void _AppendInt1(char const *fmt_, size_t n, float arg, char spec) {
		_AppendArich(fmt_, n, arg, spec);
	}

	template<class T>
	void _AppendPointer(char const *fmt_, size_t n, T const &arg, char spec) {
	}
	void _AppendPointer(char const *fmt_, size_t n, void const *arg, char spec) {
		_AppendArich(fmt_, n, (void**)arg, spec);
	}
	void _AppendPointer(char const *fmt_, size_t n, void *arg, char spec) {
		_AppendArich(fmt_, n, (void**)arg, spec);
	}

	template<class Int>
	void _AppendArich(char const *fmt_, size_t n, Int arg, char spec) {

		details::FmtBuf fmt(fmt_, n, arg, spec);
		details::DataBuf data;
		int n_write = 0;
		for (;; ) {

			//Number of characters that would have been written for a sufficiently
			// large buffer if successful(not including the terminating null character),
			//or a negative value if an error occurred.Thus, the(null - terminated) 
			// output has been completely written if and only if the returned value
			// is nonnegative and less than buf_size
			n_write = snprintf(data.fBuf, data.fLen, fmt.fBuf, arg);
			if (n_write >= 0) {
				break;
			} else {
				data.Double();
			}
		}
		_Append(data.fBuf, n_write);
	}

	void _Append(char const *str, size_t len) {
		if (len != 0) {
			if (fLen + len >= fCap) {
				for (; fCap <= fLen + len; fCap *= 3, fCap /= 2) {
				}
				char *b = new char[fCap];
				memcpy(b, fBuf, fLen);
				if (fBuf != fStaticBuf) delete fBuf;
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
	char fStaticBuf[8];
};

template<class... Args>
std::string Fmt(char const *fmt, Args... args) {
	StrBuilder sb;
	sb.Fmt(fmt, args...);
	return std::string(sb.Data(), sb.Length());
}

template<class... Args>
std::string FmtLn(char const *fmt, Args... args) {
	StrBuilder sb;
	sb.FmtLn(fmt, args...);
	return std::string(sb.Data(), sb.Length());
}
