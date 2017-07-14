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
#include <iterator>
#include <array>

namespace strbuilder {

	enum FmtArgType
	{
		SIGNED_BIT = 64,
		UNSIGNED_BIT = 128,
		_CHAR_BIT = 256,

		Char_ = 0,
		Char16_,
		Char32_,

		SignedChar_,
		Short_,
		Int_,
		Long_,
		LongLong_,

		UnsignedChar_,
		UnsignedShort_,
		UnsignedInt_,
		UnsignedLong_,
		UnsignedLongLong_,

#if CHAR_MAX == 127
		Char = Char_ | SIGNED_BIT | _CHAR_BIT,
#else
		Char = Char_ | UNSIGNED_BIT,
#endif

		Char16 = Char16_ | UNSIGNED_BIT | _CHAR_BIT,
		Char32 = Char32_ | UNSIGNED_BIT | _CHAR_BIT,

		SignedChar = SignedChar_ | SIGNED_BIT | _CHAR_BIT,
		Short = Short_ | SIGNED_BIT,
		Int = Int_ | SIGNED_BIT,
		Long = Long_ | SIGNED_BIT,
		LongLong = LongLong_ | SIGNED_BIT,

		UnsignedChar = UnsignedChar_ | UNSIGNED_BIT | _CHAR_BIT,
		UnsignedShort = UnsignedShort_ | UNSIGNED_BIT,
		UnsignedInt = UnsignedInt_ | UNSIGNED_BIT,
		UnsignedLong = UnsignedLong_ | UNSIGNED_BIT,
		UnsignedLongLong = UnsignedLongLong_ | UNSIGNED_BIT,


		Float = UnsignedLongLong_ + 1,
		Double,
		LongDouble,
		Pointer,
		C_Str, // pointer to char
		Custom,

	};

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
			} else {
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

	struct ArgFmt
	{
		size_t width;
		size_t precision;
		char flags;
		char specfier;

		char const *Str() {
			char *b = buf;
			*b++ = '%';
			if (flags) {
				*b++ = flags;
			}
			if (width < SIZE_MAX/2) {
				b += sprintf(b, "%zd", width);
			}
			if (precision < SIZE_MAX/2) {
				*b++ = '.';
				b += sprintf(b, "%zd", precision);
			}
			*b++ = specfier;
			*b++ = '\0';
			return buf;
		}
		char buf[100];
	};
	
	class StrAppender;
	template<class T>
	struct CustomTrait
	{
		static const bool value = false;
		static void ToStr(StrAppender &sa, ArgFmt &fmt, void const *data) {
			throw std::logic_error("you should implements the StringTrait::ToStr for string");
		}
	};

	template< >
	struct CustomTrait<std::string>
	{
		static const bool value = true;
		static void ToStr(StrAppender &sa, ArgFmt &fmt, void const *data);
	};

#ifdef ROOT_TString
	template< >
	struct CustomTrait<TString>
	{
		static const bool value = true;
		static void ToStr(DataBuffer &sb, ArgFmt &fmt, void const *data);
	};
#endif

	struct FmtArg
	{
		FmtArgType fType;
		void const *fData;
		void(*fToStr)(StrAppender &b, ArgFmt &fmt, void const *data);

		bool isChar() const;
		bool isIntegral() const;
		bool isUnsigned() const;
		bool isSigned() const;
		bool isFloating() const;
		bool isPointer() const;
		bool isString() const;

		static FmtArgType GetFmtArgType(char c) { return Char; }
		static FmtArgType GetFmtArgType(char16_t c) { return Char16; }
		static FmtArgType GetFmtArgType(char32_t c) { return Char32; }
		static FmtArgType GetFmtArgType(signed char) { return SignedChar; }
		static FmtArgType GetFmtArgType(unsigned char) { return UnsignedChar; }
		static FmtArgType GetFmtArgType(short) { return Short; }
		static FmtArgType GetFmtArgType(unsigned short) { return UnsignedShort; }
		static FmtArgType GetFmtArgType(int) { return Int; }
		static FmtArgType GetFmtArgType(unsigned) { return UnsignedInt; }
		static FmtArgType GetFmtArgType(long) { return Long; }
		static FmtArgType GetFmtArgType(unsigned long) { return UnsignedLong; }
		static FmtArgType GetFmtArgType(long long) { return LongLong; }
		static FmtArgType GetFmtArgType(unsigned long long) { return UnsignedLongLong; }
		static FmtArgType GetFmtArgType(char const *) { return C_Str; }
		static FmtArgType GetFmtArgType(long double) { return LongDouble; }
		static FmtArgType GetFmtArgType(double) { return Double; }
		static FmtArgType GetFmtArgType(float) { return Float; }
		static FmtArgType GetFmtArgType(void *) { return Pointer; }
		
		template<class T>
		static typename std::enable_if<CustomTrait<T>::value,
			FmtArgType>::type GetFmtArgType(T const &) { return Custom; }
	};

	class StrBuilder
	{
	public:

		// simulate the std::string
		std::size_t size() const { return Length(); }
		char const *c_str() const { return Data(); }
		char const *data() const { return Data(); }
		char const *begin() const { return Data(); }
		char const *end() const { return Data() + Length(); }
		char const *cbegin() const { return Data(); }
		char const *cend() const { return Data() + Length(); }
		std::reverse_iterator<char const *> crbeign() const { return std::make_reverse_iterator(cend()); }
		std::reverse_iterator<char const *> crend() const { return std::make_reverse_iterator(cbegin()); }

		StrBuilder();
		StrBuilder(char const *str);
		StrBuilder(std::string const &str);
		~StrBuilder();

		// you need to delete the return pointer
		char *Clone() const;
		char const *Data() const;
		size_t Length() const;
		void Clear();
		StrBuilder &App();

		template<class... Args>
		StrBuilder &App(char const *str, Args const &... args) {
			_Append(str, strlen(str));
			App(args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &App(std::string const &str, Args const &... args) {
			_Append(str.data(), str.length());
			App(args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &App(StrBuilder const &str, Args const &... args) {
			_Append(str.Data(), str.Length());
			App(args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &App(int i, Args const &... args) {
			char str[32];
			sprintf(str, "%i", i);
			_Append(str, strlen(str));
			App(args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &App(unsigned i, Args const &... args) {
			char str[32];
			sprintf(str, "%u", i);
			_Append(str, strlen(str));
			App(args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &App(char c, Args const &... args) {
			char str[32];
			sprintf(str, "%c", c);
			_Append(str, strlen(str));
			App(args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &Fmt(char const * const fmt, Args const &... args)
		{
			std::array<FmtArg, sizeof...(args)> fmtArgs;
			// we use decay_t<Args>(args) here to convert args of array a[n] to pointer
			// so that &arg will obtain the address of the the  address to array
			_FillFmt(fmt, fmtArgs, args...);
			return *this;
		}

		template<class... Args>
		StrBuilder &FmtLn(char const * fmt, Args const&... args) {
			Fmt(fmt, args...);
			_Append("\n", 1);
			return *this;
		}

		void VFmt(char const *fmt, FmtArg const args[], size_t nargs);

		void Append(char const *str, size_t len) { _Append(str, len); }
		void AppendN(char c, size_t len) { _AppendN(c, len); }
	private:
		// for internal use, currently it's same as public version Append
		void _Append(char const *str, size_t len);
		void _AppendN(char c, size_t len);

		template<size_t N, class Arg0, class... Args>
		void _FillFmt(char const * const fmt, std::array<FmtArg, N> &fmtArgs, Arg0 const &arg0, Args const &... args)
		{
			FmtArg fmtArg0;
			fmtArg0.fData = &arg0;
			fmtArg0.fToStr = CustomTrait<Arg0>::ToStr;
			fmtArg0.fType = FmtArg::GetFmtArgType(arg0);
			fmtArgs[N - sizeof...(args)-1] = fmtArg0;
			_FillFmt(fmt, fmtArgs, args...);
		}

		template<size_t N, size_t N1, class... Args>
		void _FillFmt(char const * const fmt, std::array<FmtArg, N> &fmtArgs, char const (&arg0)[N1], Args const &... args)
		{
			FmtArg fmtArg0;
			char const *arg0_ = arg0;
			fmtArg0.fData = &arg0_;
			fmtArg0.fToStr = CustomTrait<char const*>::ToStr;
			fmtArg0.fType = FmtArg::GetFmtArgType(arg0);
			fmtArgs[N - sizeof...(args)-1] = fmtArg0;
			_FillFmt(fmt, fmtArgs, args...);
		}

		template<size_t N>
		void _FillFmt(char const * const fmt, std::array<FmtArg, N> &fmtArgs)
		{
			VFmt(fmt, fmtArgs.data(), fmtArgs.size());
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

	template<class... Args>
	void Print(char const *fmt, Args... args) {
		StrBuilder sb;
		sb.Fmt(fmt, args...);
		printf("%s", sb.Data());
	}

	template<class... Args>
	void PrintLn(char const *fmt, Args... args) {
		StrBuilder sb;
		sb.Fmt(fmt, args...);
		printf("%s\n", sb.Data());
	}


	class StrAppender
	{
		StrBuilder &sb;
	public:
		StrAppender(StrBuilder &sb_) : sb(sb_) { }
		StrAppender &Append(char const *data, size_t len) {
			sb.Append(data, len);
			return *this;
		}
		StrAppender &AppendN(char c, size_t len) {
			sb.AppendN(c, len);
			return *this;
		}
		template<class... Args>
		StrAppender &Fmt(char const * const fmt, Args const &... args)
		{
			sb.Fmt(fmt, args...);
			return *this;
		}
	};

}
