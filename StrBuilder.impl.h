#include "StrBuilder.h"

namespace strbuilder {

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
		sb._Append(data.fBuf, n_write);
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

	StrBuilder &StrBuilder::App() {
		return *this;
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

	bool FmtArg::isChar() const {
		return (fType & _CHAR_BIT) != 0;
	}

	bool FmtArg::isIntegral() const
	{
		return (fType & (UNSIGNED_BIT | SIGNED_BIT)) != 0;
	}

	bool FmtArg::isSigned() const
	{
		return (fType & SIGNED_BIT) != 0;
	}

	bool FmtArg::isUnsigned() const
	{
		return (fType & UNSIGNED_BIT) != 0;
	}

	bool FmtArg::isFloating() const
	{
		return fType == Float || fType == Double || fType == LongDouble;
	}

	bool FmtArg::isPointer() const {
		return fType == Pointer || fType == C_Str;
	}

	bool FmtArg::isString() const {
		return fType == C_Str || fType == String;
	}

	void StringTrait<std::string>::ToStr(DataBuffer &sb, char const *fmt, size_t n, void const *data)
	{
		FmtBuffer fb(fmt, n);
		char const *str = (*(std::string*)data).c_str();
		for (;;) {
			if (snprintf(sb.fBuf, sb.fLen, fb.fBuf, str) >= 0) {
				break;
			}
			sb.Double();
		}
	}

#ifdef ROOT_TString
	void StringTrait<std::string>::ToStr(DataBuffer &sb, char const *fmt, size_t n, void const *data)
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
	void StrBuilder::VFmt(char const *fmt, FmtArg const args[], size_t nargs)
	{
		size_t argIdx = 0;

		char const *last = fmt;
		for (; fmt[0];) {	

			if (fmt[0] == '%') {
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
					if (fmt[0] == '-' ||
						fmt[0] == '+' ||
						fmt[0] == '#' ||
						fmt[0] == '0') {
						++fmt;
					}

					// read width
					size_t width = 0;
					const size_t max_size = (size_t)-1;
					const size_t max_size_div_by_10 = max_size / 10;

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

					// precision
					if (fmt[0] == '.') {
						++fmt;
						for (; fmt[0] >= '0' && fmt[0] <= '9'; ++fmt) {
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
					FmtArg const &arg = args[argIdx];
					argIdx++;
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
						if (!arg.isFloating()) {
							throw std::logic_error("arg0 is not floating point!");
						}
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
						break;
					}
					switch (arg.fType)
					{
					case Char:
						_Append1(*this, fmt_, nfmt_, *(char*)arg.fData, spec);
						break;
					case Char16:
						_Append1(*this, fmt_, nfmt_, *(char16_t*)arg.fData, spec);
						break;
					case Char32:
						_Append1(*this, fmt_, nfmt_, *(char32_t*)arg.fData, spec);
						break;
					case SignedChar:
						_Append1(*this, fmt_, nfmt_, *(signed char*)arg.fData, spec);
						break;
					case Short:
						_Append1(*this, fmt_, nfmt_, *(short*)arg.fData, spec);
						break;
					case Int:
						_Append1(*this, fmt_, nfmt_, *(int*)arg.fData, spec);
						break;
					case Long:
						_Append1(*this, fmt_, nfmt_, *(long*)arg.fData, spec);
						break;
					case LongLong:
						_Append1(*this, fmt_, nfmt_, *(long long*)arg.fData, spec);
						break;
					case UnsignedChar:
						_Append1(*this, fmt_, nfmt_, *(unsigned char*)arg.fData, spec);
						break;
					case UnsignedShort:
						_Append1(*this, fmt_, nfmt_, *(unsigned short*)arg.fData, spec);
						break;
					case UnsignedInt:
						_Append1(*this, fmt_, nfmt_, *(unsigned int*)arg.fData, spec);
						break;
					case UnsignedLong:
						_Append1(*this, fmt_, nfmt_, *(unsigned long*)arg.fData, spec);
						break;
					case UnsignedLongLong:
						_Append1(*this, fmt_, nfmt_, *(unsigned long long*)arg.fData, spec);
						break;
					case Double:
						_Append1(*this, fmt_, nfmt_, *(double*)arg.fData, spec);
						break;
					case Float:
						_Append1(*this, fmt_, nfmt_, *(float*)arg.fData, spec);
					case LongDouble:
						_Append1(*this, fmt_, nfmt_, *(long double*)arg.fData, spec);
						break;
					case C_Str:
						_Append1(*this, fmt_, nfmt_, *(char const**)arg.fData, spec);
						break;
					case Pointer:
						_Append1(*this, fmt_, nfmt_, *(void **)arg.fData, spec);
						break;
					case String:
					{
						DataBuffer db;
						arg.fToStr(db, fmt_, nfmt_ + 1, arg.fData);
						_Append(db.fBuf, db.fLen);
						break;
					}
					default:
						throw std::logic_error("unknow type!");
						break;
					}
				}
			} else {
			   ++fmt;
			}
		}
		_Append(last, fmt - last);
		if (argIdx != nargs && false) {
			throw std::runtime_error("there are unused parameter(s)");
		}
	}
}

