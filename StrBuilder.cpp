#include "StrBuilder.h"
#include <assert.h>
#include <stdio.h>

namespace strbuilder {

	char const *ArgFmt::Str() {
		char *b = buf;
		*b++ = '%';
		if (flags) {
			*b++ = flags;
		}
		if (width < SIZE_MAX / 2) {
			b += sprintf(b, "%zd", width);
		}
		if (precision < SIZE_MAX / 2) {
			*b++ = '.';
			b += sprintf(b, "%zd", precision);
		}
		*b++ = specfier;
		*b++ = '\0';
		return buf;
	}

	struct DataBuffer {

		DataBuffer() {
			fLen = 31;
			fBuf = fStaticBuf;
		}
		void Reinit(size_t n) {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
			if (n <= 31) {
				fLen = n;
				fBuf = fStaticBuf;
			}
			else {
				fLen = n;
				fBuf = new char[fLen + 1];
			}
		}
		void Double() {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
			fLen *= 2;
			fBuf = new char[fLen + 1];
		}

		~DataBuffer() {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
		}
		size_t fLen;
		char *fBuf;
	private:
		char fStaticBuf[32];
	};

	struct AutoFmtBuf
	{

		void init(char const *fmt, size_t n) {
			if (n <= 20) {
				fBuf = fStaticBuf;
			} else {
				fBuf = new char[n + 1];
			}
			memcpy(fBuf, fmt, n);
		}

		void setSpec(char const *fmt, size_t n, char spec, int) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, unsigned) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, long) {
			init(fmt, n);
			fBuf[n++] = 'l';
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, unsigned long) {
			init(fmt, n);
			fBuf[n++] = 'l';
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, long long) {
			init(fmt, n);
			fBuf[n++] = 'l';
			fBuf[n++] = 'l';
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, unsigned long long) {
			init(fmt, n);
			fBuf[n++] = 'l';
			fBuf[n++] = 'l';
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, char const*) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, double) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, long double) {
			init(fmt, n);
			fBuf[n++] = 'L';
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		void setSpec(char const *fmt, size_t n, char spec, void *) {
			init(fmt, n);
			fBuf[n++] = spec;
			fBuf[n++] = '\0';
		}

		~AutoFmtBuf() {
			if (fBuf != fStaticBuf) {
				delete fBuf;
			}
		}
		char *fBuf;
	private:
		char fStaticBuf[32];
	};

	template<class Int>
	void _Append1(StrBuilder &sb, char const *fmt_, size_t n, Int arg, char spec) {

		AutoFmtBuf fmt;
		fmt.setSpec(fmt_, n, spec, arg);
		DataBuffer data;
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
		sb.Append(data.fBuf, n_write);
	}



	StrBuilder &StrBuilder::App() {
		return *this;
	}

	StrBuilder &StrBuilder::App(char const *str) {
		_Append(str, strlen(str));
		return *this;
	}

	StrBuilder &StrBuilder::App(std::string const &str) {
		_Append(str.data(), str.length());
		return *this;
	}

	StrBuilder &StrBuilder::App(StrBuilder const &str) {
		_Append(str.Data(), str.Length());
		return *this;
	}

	StrBuilder &StrBuilder::App(int i) {
		char str[32];
		sprintf(str, "%i", i);
		_Append(str, strlen(str));
		return *this;
	}

	StrBuilder &StrBuilder::App(unsigned i) {
		char str[32];
		sprintf(str, "%u", i);
		_Append(str, strlen(str));
		return *this;
	}

	StrBuilder &StrBuilder::App(char c) {
		char str[32];
		sprintf(str, "%c", c);
		_Append(str, strlen(str));
		return *this;
	}






	char *StrBuilder::Clone() const {
		char *b = new char[fLen + 1];
		memcpy(b, fBuf, fLen + 1);
		return b;
	}

	char const *StrBuilder::Data() const {
		return fBuf;
	}

	size_t StrBuilder::Length() const {
		return fLen;
	}

	StrBuilder::~StrBuilder() {
		Clear();
	}

	StrBuilder::StrBuilder(char const *str) {
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
		_Append(str, strlen(str));
	}

	StrBuilder::StrBuilder(std::string const &str) {
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
		_Append(str.data(), str.size());
	}

	StrBuilder::StrBuilder() {
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
	}

	void StrBuilder::Clear() {
		if (fBuf != fStaticBuf) {
			delete fBuf;
		}
		fBuf = fStaticBuf;
		fLen = 0;
		fCap = 7;
		fBuf[0] = '\0';
	}


	void StrBuilder::_AppendN(char c, size_t len) {
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
			memset(fBuf + fLen, c, len);
			fLen += len;
			fBuf[fLen] = '\0';
		}
	}

	void StrBuilder::_Append(char const *str, size_t len) {
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

	bool ArgInfo::isChar() const {
		return (fType & _CHAR_FLAG) != 0;
	}

	bool ArgInfo::isIntegral() const
	{
		return (fType & (_UNSIGNED_FLAG | _SIGNED_FLAG)) != 0;
	}

	bool ArgInfo::isSigned() const
	{
		return (fType & _SIGNED_FLAG) != 0;
	}

	bool ArgInfo::isUnsigned() const
	{
		return (fType & _UNSIGNED_FLAG) != 0;
	}

	bool ArgInfo::isFloating() const
	{
		return fType == Float || fType == Double || fType == LongDouble;
	}

	bool ArgInfo::isPointer() const {
		return fType == Pointer || fType == C_Str;
	}

	bool ArgInfo::isString() const {
		return fType == C_Str;
	}

#ifdef ROOT_TString
	void CustomTrait<std::string>::ToStr(DataBuffer &sb, char const *fmt, size_t n, void const *data)
	{
		FmtBuffer fb(fmt, n);
		char const *str = *(TString*)data;
		for (;;) {
			if (snprintf(sb.fBuf, sb.fLen, fb.fBuf, str) >= 0) {
				break;
			}
			sb.Double();
		}
	}
#endif

	void covert_to_double(ArgInfo &arg)
	{
		double d;
		switch (arg.fType)
		{
		case Char:
			d = *(char*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
#ifdef SB_NEWCHAR
		case Char16:
			d = *(char16_t*)arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case Char32:
			d = *(char32_t*)arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
#endif
		case SignedChar:
			d = *(signed char*)arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case Short:
			d = *(short*)arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case Int:
			d = *(int*)arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case Long:
			d = *(long*)arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case LongLong:
			d = (double)*(long long*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case UnsignedChar:
			d = *(unsigned char*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case UnsignedShort:
			d = *(unsigned short*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case UnsignedInt:
			d = *(unsigned int*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case UnsignedLong:
			d = *(unsigned long*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case UnsignedLongLong:
			d = (double)*(unsigned long long*)&arg.fData;
			arg.fData = *(long long*)&d;
			arg.fType = Double;
			break;
		case Double:
		case Float:
		case LongDouble:
			break;
		default:
			throw std::logic_error("arg0 is not floating point!");
			break;
		}
	}

	void StrBuilder::VFmt(char const *fmt, ArgInfo const args[], size_t nargs)
	{
		size_t argIdx = 0;

		char const *last = fmt;
		for (; ;)
		{
			char const *new_fmt = strchr(fmt, '%');
			if (new_fmt) {
				fmt = new_fmt;
				if (fmt[1] == '%') {
					++fmt;
					_Append(last, fmt - last);
					++fmt;
					last = fmt;
				} else {
					char const *fmt_ = fmt;
					_Append(last, fmt - last);
					++fmt; // read `%'

					// read flags
					char flags = '\0';
					if (fmt[0] == '-' ||
						fmt[0] == '+' ||
						fmt[0] == '#' ||
						fmt[0] == '0') {
						flags = fmt[0];
						++fmt;
					}

					// read width
					size_t width = -1;
					const size_t max_size = (size_t)-1;
					const size_t max_size_div_by_10 = max_size / 10;
					if (fmt[0] >= '0' && fmt[0] <= '9') {
						width = 0;
						for (; fmt[0] >= '0' && fmt[0] <= '9'; ++fmt) {
							if (width > max_size_div_by_10) {
								throw std::runtime_error("too large width!");
							} else {
								width *= 10;
								size_t old_width = width;
								width += fmt[0] - '0';
								if (width < old_width) {
									throw std::runtime_error("too large width!");
								}
							}
						}
					}
					
					size_t precision = -1;
					// precision
					if (fmt[0] == '.') {
						++fmt;
						precision = 0;
						for (; fmt[0] >= '0' && fmt[0] <= '9'; ++fmt) {
							if (precision > max_size_div_by_10) {
								throw std::runtime_error("too large precision!");
							} else {
								precision *= 10;
								size_t old_width = precision;
								precision += fmt[0] - '0';
								if (precision < old_width) {
									throw std::runtime_error("too large precision!");
								}
							}
						}
					}

					// specifier
					size_t nfmt_ = fmt - fmt_;
					char spec = fmt[0];
					++fmt;
					last = fmt;

					if (argIdx >= nargs) {
						throw std::runtime_error("too less arg!");
					}
					ArgInfo arg = args[argIdx];
					argIdx++;

					if (arg.fType == Custom) {
						ArgFmt argFmt;
						argFmt.width = width;
						argFmt.precision = precision;
						argFmt.flags = flags;
						argFmt.specfier = spec;
						StrAppender sa(*this);
						arg.fToStr(sa, argFmt, arg.fData);
					} else {
						switch (spec)
						{
						case 'd': // signed or unsigned integers
						{
							if (arg.isSigned()) {
								spec = 'i';
							} else if (arg.isUnsigned()) {
								spec = 'u';
							} else {
								throw std::runtime_error("arg is not integral!");
							}
							break;
						}
						case 'o':
						case 'x':
						case 'X':
						{
							if (!arg.isIntegral()) {
								throw std::logic_error("arg0 is not integral!");
							}
							break;
						}
						case 'i':
						{
							if (!arg.isSigned()) {
								throw std::logic_error("arg is not signed!");
							}
							break;
						}
						case 'u': // unsigned integers
						{
							if (!arg.isUnsigned()) {
								throw std::logic_error("arg is not unsigned!");
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
							covert_to_double(arg);
							break;
						}
						case 'c': // char
						{
							if (!arg.isChar()) {
								throw std::logic_error("arg is not unsigned!");
							}
							break;
						}
						case 's': // std::string or char const*
						{
							if (!arg.isString()) {
								throw std::logic_error("arg is not string!");
							}
							break;
						}
						case 'p': // pointer
						{
							if (!arg.isPointer()) {
								throw std::logic_error("arg is not pointer!");
							}
							break;
						}
						case 'n': // any type
							throw std::logic_error("%n is not supported!");
							break;
						default:
						{
							throw std::logic_error("unkown specifier is not supported!");
							break;
						}
						}
						switch (arg.fType)
						{
						case Char:
							_Append1(*this, fmt_, nfmt_, (char)arg.fData, spec);
							break;
#ifdef SB_NEWCHAR
						case Char16:
							_Append1(*this, fmt_, nfmt_, (char16_t)arg.fData, spec);
							break;
						case Char32:
							_Append1(*this, fmt_, nfmt_, (char32_t)arg.fData, spec);
							break;
#endif
						case SignedChar:
							_Append1(*this, fmt_, nfmt_, (signed char)arg.fData, spec);
							break;
						case Short:
							_Append1(*this, fmt_, nfmt_, (short)arg.fData, spec);
							break;
						case Int:
							_Append1(*this, fmt_, nfmt_, (int)arg.fData, spec);
							break;
						case Long:
							_Append1(*this, fmt_, nfmt_, (long)arg.fData, spec);
							break;
						case LongLong:
							_Append1(*this, fmt_, nfmt_, (long long)arg.fData, spec);
							break;
						case UnsignedChar:
							_Append1(*this, fmt_, nfmt_, (unsigned char)arg.fData, spec);
							break;
						case UnsignedShort:
							_Append1(*this, fmt_, nfmt_, (unsigned short)arg.fData, spec);
							break;
						case UnsignedInt:
							_Append1(*this, fmt_, nfmt_, (unsigned int)arg.fData, spec);
							break;
						case UnsignedLong:
							_Append1(*this, fmt_, nfmt_, (unsigned long)arg.fData, spec);
							break;
						case UnsignedLongLong:
							_Append1(*this, fmt_, nfmt_, (unsigned long long)arg.fData, spec);
							break;
						case Double:
							_Append1(*this, fmt_, nfmt_, *(double*)&arg.fData, spec);
							break;
						case LongDouble:
							_Append1(*this, fmt_, nfmt_, *(long double*)arg.fData, spec);
							break;
						case Float:
							_Append1(*this, fmt_, nfmt_, *(float*)&arg.fData, spec);
						case C_Str:
							_Append1(*this, fmt_, nfmt_, (char const*)arg.fData, spec);
							break;
						case Pointer:
							_Append1(*this, fmt_, nfmt_, (void *)arg.fData, spec);
							break;
						default:
							throw std::logic_error("unknow type!");
							break;
						}
					}
				}
			} else { // if(fmt = strchr(fmt, '%'))
				fmt = fmt + strlen(fmt);
				break;
			}
		}
		_Append(last, fmt - last);
		if (argIdx != nargs && false) {
			throw std::runtime_error("there are unused parameter(s)");
		}
	}


	void StdOut(char const *str) {
		printf("%s", str);
	}

}

