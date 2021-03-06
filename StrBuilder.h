#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <string.h>
#include <iterator>

#if defined(__GNUC__) && __GNUC__ > 4
#define SB_NEWCHAR
#endif

namespace strbuilder {

	enum ArgType
	{
		_SIGNED_FLAG = 64,
		_UNSIGNED_FLAG = 128,
		_CHAR_FLAG = 256,

		_Char = 0,
		_Char16,
		_Char32,

		_SignedChar,
		_Short,
		_Int,
		_Long,
		_LongLong,

		_UnsignedChar,
		_UnsignedShort,
		_UnsignedInt,
		_UnsignedLong,
		_UnsignedLongLong,

#if CHAR_MAX == 127
		Char = _Char | _SIGNED_FLAG | _CHAR_FLAG,
#else
		Char = _Char | _UNSIGNED_FLAG | _CHAR_FLAG,
#endif

		Char16 = _Char16 | _UNSIGNED_FLAG | _CHAR_FLAG,
		Char32 = _Char32 | _UNSIGNED_FLAG | _CHAR_FLAG,

		SignedChar = _SignedChar | _SIGNED_FLAG | _CHAR_FLAG,
		Short = _Short | _SIGNED_FLAG,
		Int = _Int | _SIGNED_FLAG,
		Long = _Long | _SIGNED_FLAG,
		LongLong = _LongLong | _SIGNED_FLAG,

		UnsignedChar = _UnsignedChar | _UNSIGNED_FLAG | _CHAR_FLAG,
		UnsignedShort = _UnsignedShort | _UNSIGNED_FLAG,
		UnsignedInt = _UnsignedInt | _UNSIGNED_FLAG,
		UnsignedLong = _UnsignedLong | _UNSIGNED_FLAG,
		UnsignedLongLong = _UnsignedLongLong | _UNSIGNED_FLAG,


		Float = _UnsignedLongLong + 1,
		Double,
		LongDouble,
		Pointer,
		C_Str, // pointer to char
		Custom

	};

	struct ArgFmt
	{
		size_t width;
		size_t precision;
		char flags;
		char specfier;

		char const *Str();
		char buf[100];
	};

	class StrAppender;
	template<class T>
	struct CustomTrait
	{
		//static const bool value = false;
		//static void ToStr(StrAppender &sa, ArgFmt &fmt, long long);
	};


	struct ArgInfo
	{
		ArgType fType;
		long long fData;
		void(*fToStr)(StrAppender &b, ArgFmt &fmt, long long);

		bool isChar() const;
		bool isIntegral() const;
		bool isUnsigned() const;
		bool isSigned() const;
		bool isFloating() const;
		bool isPointer() const;
		bool isString() const;

		static ArgType GetFmtArgType(char c) { return Char; }
#ifdef SB_NEWCHAR
		static ArgType GetFmtArgType(char16_t c) { return Char16; }
		static ArgType GetFmtArgType(char32_t c) { return Char32; }
#endif
		static ArgType GetFmtArgType(signed char) { return SignedChar; }
		static ArgType GetFmtArgType(unsigned char) { return UnsignedChar; }
		static ArgType GetFmtArgType(short) { return Short; }
		static ArgType GetFmtArgType(unsigned short) { return UnsignedShort; }
		static ArgType GetFmtArgType(int) { return Int; }
		static ArgType GetFmtArgType(unsigned) { return UnsignedInt; }
		static ArgType GetFmtArgType(long) { return Long; }
		static ArgType GetFmtArgType(unsigned long) { return UnsignedLong; }
		static ArgType GetFmtArgType(long long) { return LongLong; }
		static ArgType GetFmtArgType(unsigned long long) { return UnsignedLongLong; }
		static ArgType GetFmtArgType(char const *) { return C_Str; }
		static ArgType GetFmtArgType(long double) { return LongDouble; }
		static ArgType GetFmtArgType(double) { return Double; }
		static ArgType GetFmtArgType(float) { return Float; }
		static ArgType GetFmtArgType(void const *) { return Pointer; }
	};

	class StrBuilder
	{
	public:

		// simulate the std::string
		std::size_t size() const { return Length(); }
		char const *c_str() const { return Data(); }
		/// ends with '\0', like new standard std::string data
		char const *data() const { return Data(); }
		char const *begin() const { return Data(); }
		char const *end() const { return Data() + Length(); }
		char const *cbegin() const { return Data(); }
		char const *cend() const { return Data() + Length(); }
		std::reverse_iterator<char const *> rbeign() const { return std::reverse_iterator<char const *>(cend()); }
		std::reverse_iterator<char const *> rend() const { return std::reverse_iterator<char const *>(cbegin()); }
		std::reverse_iterator<char const *> crbeign() const { return std::reverse_iterator<char const *>(cend()); }
		std::reverse_iterator<char const *> crend() const { return std::reverse_iterator<char const *>(cbegin()); }

		StrBuilder();
		StrBuilder(char const *str);
		StrBuilder(std::string const &str);
		~StrBuilder();

		/// Get a copy of the internal data
		/// delete it by yourself
		char *Clone() const;
		/// same as dataf()
		char const *Data() const;
		/// Same as length()
		size_t Length() const;
		/// clear internal data
		void Clear();

		/// apppend nothing, and return this
		StrBuilder &App();
		/// apppend string, and return this
		StrBuilder &App(char const *str);
		/// apppend std::string, and return this
		StrBuilder &App(std::string const &str);
		/// apppend StrBuilder, and return this
		StrBuilder &App(StrBuilder const &str);
		/// apppend integer, and return this
		StrBuilder &App(int i);
		/// apppend unsigned, and return this
		StrBuilder &App(unsigned i);
		/// apppend char, and return this
		StrBuilder &App(char c);

		/// append args
		template<class T1, class T2>
		StrBuilder &App(T1 v1, T2 v2) {
			App(v1);
			App(v2);
			return *this;
		}

		/// append args
		template<class T1, class T2, class T3>
		StrBuilder &App(T1 v1, T2 v2, T3 v3) {
			App(v1);
			App(v2);
			App(v3);
			return *this;
		}

		/// append args
		template<class T1, class T2, class T3, class T4>
		StrBuilder &App(T1 v1, T2 v2, T3 v3, T4 v4) {
			App(v1);
			App(v2);
			App(v3);
			App(v4);
			return *this;
		}

		/// append args
		template<class T1, class T2, class T3, class T4, class T5>
		StrBuilder &App(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5) {
			App(v1);
			App(v2);
			App(v3);
			App(v4);
			App(v5);
			return *this;
		}

		/// append args
		template<class T1, class T2, class T3, class T4, class T5, class T6>
		StrBuilder &App(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6) {
			App(v1);
			App(v2);
			App(v3);
			App(v4);
			App(v5);
			App(v6);
			return *this;
		}

#ifdef SB_VARARG
		/// append args
		/// hope your compiler is not too old
		template<class T1, class T2, class T3, class T4, class T5, class... Args>
		StrBuilder &App(T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5, Args const &... args) {
			App(v1);
			App(v2);
			App(v3);
			App(v4);
			App(v5);
			App(args...);
			return *this;
		}
#endif

		/// format with formatter fmt
		StrBuilder &Fmt(char const * const fmt)
		{
			ArgInfo fmtArgs[1];
			VFmt(fmt, fmtArgs, 0);
			return *this;
		}

		/// format with formatter fmt
		template<class T1>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1)
		{
			ArgInfo fmtArgs[1];
			SetFmt1(fmtArgs[0], v1);
			VFmt(fmt, fmtArgs, 1);
			return *this;
		}

		/// format with formatter fmt
		template<class T1, class T2>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1, T2 const &v2)
		{
			ArgInfo fmtArgs[2];
			SetFmt1(fmtArgs[0], v1);
			SetFmt1(fmtArgs[1], v2);
			VFmt(fmt, fmtArgs, 2);
			return *this;
		}

		/// format with formatter fmt
		template<class T1, class T2, class T3>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1, T2 const &v2, T3 const &v3)
		{
			ArgInfo fmtArgs[3];
			SetFmt1(fmtArgs[0], v1);
			SetFmt1(fmtArgs[1], v2);
			SetFmt1(fmtArgs[2], v3);
			VFmt(fmt, fmtArgs, 3);
			return *this;
		}

		/// format with formatter fmt
		template<class T1, class T2, class T3, class T4>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4)
		{
			ArgInfo fmtArgs[4];
			SetFmt1(fmtArgs[0], v1);
			SetFmt1(fmtArgs[1], v2);
			SetFmt1(fmtArgs[2], v3);
			SetFmt1(fmtArgs[3], v4);
			VFmt(fmt, fmtArgs, 4);
			return *this;
		}

		/// format with formatter fmt
		template<class T1, class T2, class T3, class T4, class T5>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5)
		{
			ArgInfo fmtArgs[5];
			SetFmt1(fmtArgs[0], v1);
			SetFmt1(fmtArgs[1], v2);
			SetFmt1(fmtArgs[2], v3);
			SetFmt1(fmtArgs[3], v4);
			SetFmt1(fmtArgs[4], v5);
			VFmt(fmt, fmtArgs, 5);
			return *this;
		}

		/// format with formatter fmt
		template<class T1, class T2, class T3, class T4, class T5, class T6>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1, T2 const &v2,
			T3 const &v3, T4 const &v4, T5 const &v5, T6 const &v6)
		{
			ArgInfo fmtArgs[6];
			SetFmt1(fmtArgs[0], v1);
			SetFmt1(fmtArgs[1], v2);
			SetFmt1(fmtArgs[2], v3);
			SetFmt1(fmtArgs[3], v4);
			SetFmt1(fmtArgs[4], v5);
			SetFmt1(fmtArgs[5], v6);
			VFmt(fmt, fmtArgs, 6);
			return *this;
		}

#ifdef SB_VARARG

		//private:

		/// This is private function.
		/// we let it be public for testing,
		/// but you don't use this function.
		void SetFmt(ArgInfo &fmt) {
		}

		/// This is private function.
		/// we let it be public for testing,
		/// but you don't use this function.
		template<class T1, class... Args>
		void SetFmt(ArgInfo &fmt, T1 const &t1, Args const &... args) {
			SetFmt1(fmt, t1);
			SetFmt((&fmt)[1], args...);
		}
	public:

		/// format with formatter fmt
		template<class T1, class T2, class T3, class T4, class T5, class T6, class... Args>
		StrBuilder &Fmt(char const * const fmt, T1 const &v1, T2 const &v2,
			T3 const &v3, T4 const &v4, T5 const &v5, T6 const &v6, Args const &... args)
		{
			ArgInfo fmtArgs[6 + sizeof...(args)];
			SetFmt1(fmtArgs[0], v1);
			SetFmt1(fmtArgs[1], v2);
			SetFmt1(fmtArgs[2], v3);
			SetFmt1(fmtArgs[3], v4);
			SetFmt1(fmtArgs[4], v5);
			SetFmt1(fmtArgs[5], v6);
			SetFmt(fmtArgs[6], args...);
			VFmt(fmt, fmtArgs, 6 + sizeof...(args));
			return *this;
		}
#endif

		/// format with formatter fmt
		/// a '\n' added automatically
		StrBuilder &FmtLn(char const * const fmt)
		{
			Fmt(fmt);
			_Append("\n", 1);
			return *this;
		}

		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1)
		{
			Fmt(fmt, v1);
			_Append("\n", 1);
			return *this;
		}

		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1, class T2>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1, T2 const &v2)
		{
			Fmt(fmt, v1, v2);
			_Append("\n", 1);
			return *this;
		}

		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1, class T2, class T3>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1, T2 const &v2, T3 const &v3)
		{
			Fmt(fmt, v1, v2, v3);
			_Append("\n", 1);
			return *this;
		}

		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1, class T2, class T3, class T4>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4)
		{
			Fmt(fmt, v1, v2, v3, v4);
			_Append("\n", 1);
			return *this;
		}

		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1, class T2, class T3, class T4, class T5>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5)
		{
			Fmt(fmt, v1, v2, v3, v4, v5);
			_Append("\n", 1);
			return *this;
		}

		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1, class T2, class T3, class T4, class T5, class T6>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1, T2 const &v2,
			T3 const &v3, T4 const &v4, T5 const &v5, T6 const &v6)
		{
			Fmt(fmt, v1, v2, v3, v4, v5, v6);
			_Append("\n", 1);
			return *this;
		}


#ifdef SB_VARARG
		/// format with formatter fmt
		/// a '\n' added automatically
		template<class T1, class T2, class T3, class T4, class T5, class T6, class... Args>
		StrBuilder &FmtLn(char const * const fmt, T1 const &v1, T2 const &v2,
			T3 const &v3, T4 const &v4, T5 const &v5, T6 const &v6, Args const &... args)
		{
			Fmt(fmt, v1, v2, v3, v4, v5, v6, args);
			return *this;
		}
#endif

		/// format with formatter fmt
		/// this is high level function, before you use this function
		/// you need undertand the code firstly
		void VFmt(char const *fmt, ArgInfo const args[], size_t nargs);

		// appends str of length len
		void Append(char const *str, size_t len) { _Append(str, len); }
		// appends c, repeat len times
		void AppendN(char c, size_t len) { _AppendN(c, len); }
	




private:
		// for internal use, currently it's same as public version Append
		void _Append(char const *str, size_t len);
		void _AppendN(char c, size_t len);

		template<class Arg>
		void SetFmt1(ArgInfo &fmtArg, Arg const &arg0)
		{
			fmtArg.fData = (long long)(void*)&arg0;
			// if some error happened here, it means you use a unsupported type
			// you need to implement CustomTrait to support the `Arg` type.
			// just see CustomTrait<std::string>
			fmtArg.fToStr = CustomTrait<Arg>::ToStr;
			fmtArg.fType = /*ArgType::*/Custom;
		}

		void SetFmt1(ArgInfo &fmtArg, double arg0)
		{
            memcpy(&fmtArg.fData, &arg0, sizeof(double));
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, float arg0)
		{
			SetFmt1(fmtArg, (double)arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, unsigned long long arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, long long arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, unsigned long arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, long arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, unsigned int arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, int arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, unsigned short arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, short arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}


		void SetFmt1(ArgInfo &fmtArg, unsigned char arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, char arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, signed char arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}
#ifdef SB_NEWCHAR
		void SetFmt1(ArgInfo &fmtArg, char16_t arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, char32_t arg0)
		{
			fmtArg.fData = arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}
#endif
		void SetFmt1(ArgInfo &fmtArg, char const *arg0)
		{
			fmtArg.fData = (long long)arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

		void SetFmt1(ArgInfo &fmtArg, char *arg0)
		{
			SetFmt1(fmtArg, static_cast<char const *>(arg0));
		}

		template<class T>
		void SetFmt1(ArgInfo &fmtArg, T *arg0)
		{
			SetFmt1(fmtArg, static_cast<T const *>(arg0));
		}

		template<class T>
		void SetFmt1(ArgInfo &fmtArg, T const *arg0)
		{
			fmtArg.fData = (long long)arg0;
			fmtArg.fType = ArgInfo::GetFmtArgType(arg0);
		}

	private:
		char *fBuf;
		size_t fLen;
		size_t fCap;
		char fStaticBuf[8];
	};

	template<class T1>
	std::string Fmt(char const *fmt, T1 const &v1) {
		StrBuilder sb;
		sb.Fmt(fmt, v1);
		return std::string(sb.Data(), sb.Length());
	}

	template<class T1, class T2>
	std::string Fmt(char const *fmt, T1 const &v1, T2 const &v2) {
		StrBuilder sb;
		sb.Fmt(fmt, v1, v2);
		return std::string(sb.Data(), sb.Length());
	}

	template<class T1, class T2, class T3>
	std::string Fmt(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3) {
		StrBuilder sb;
		sb.Fmt(fmt, v1, v2, v3);
		return std::string(sb.Data(), sb.Length());
	}

	template<class T1, class T2, class T3, class T4>
	std::string Fmt(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4) {
		StrBuilder sb;
		sb.Fmt(fmt, v1, v2, v3, v4);
		return std::string(sb.Data(), sb.Length());
	}

	template<class T1, class T2, class T3, class T4, class T5>
	std::string Fmt(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5) {
		StrBuilder sb;
		sb.Fmt(fmt, v1, v2, v3, v4, v5);
		return std::string(sb.Data(), sb.Length());
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	std::string Fmt(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4,
		T5 const &v5, T6 const &v6) {
		StrBuilder sb;
		sb.Fmt(fmt, v1, v2, v3, v4, v5, v6);
		return std::string(sb.Data(), sb.Length());
	}

	void StdOut(char const *str);


	inline void PrintLn(char const *fmt) {
		StrBuilder sb;
		sb.FmtLn(fmt);
		StdOut(sb.Data());
	}


	template<class T1>
	void PrintLn(char const *fmt, T1 const &v1) {
		StrBuilder sb;
		sb.FmtLn(fmt, v1);
		StdOut(sb.Data());
	}

	template<class T1, class T2>
	void PrintLn(char const *fmt, T1 const &v1, T2 const &v2) {
		StrBuilder sb;
		sb.FmtLn(fmt, v1, v2);
		StdOut(sb.Data());
	}

	template<class T1, class T2, class T3>
	void PrintLn(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3) {
		StrBuilder sb;
		sb.FmtLn(fmt, v1, v2, v3);
		StdOut(sb.Data());
	}

	template<class T1, class T2, class T3, class T4>
	void PrintLn(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4) {
		StrBuilder sb;
		sb.FmtLn(fmt, v1, v2, v3, v4);
		StdOut(sb.Data());
	}

	template<class T1, class T2, class T3, class T4, class T5>
	void PrintLn(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4, T5 const &v5) {
		StrBuilder sb;
		sb.FmtLn(fmt, v1, v2, v3, v4, v5);
		StdOut(sb.Data());
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	void PrintLn(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4,
		T5 const &v5, T6 const &v6) {
		StrBuilder sb;
		sb.FmtLn(fmt, v1, v2, v3, v4, v5, v6);
		StdOut(sb.Data());
	}

#ifdef SB_VARARG
	template<class T1, class T2, class T3, class T4, class T5, class T6, class... Args>
	std::string Fmt(char const *fmt, T1 const &v1, T2 const &v2, T3 const &v3, T4 const &v4,
		T5 const &v5, T6 const &v6, Args const &... args) {
		StrBuilder sb;
		sb.Fmt(fmt, v1, v2, v3, v4, v5, v6, args);
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

#endif


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

#ifdef SB_VARARG
		template<class... Args>
		StrAppender &Fmt(char const * const fmt, Args const &... args)
		{
			sb.Fmt(fmt, args...);
			return *this;
		}
#endif
	};


	template< >
	struct CustomTrait<std::string>
	{
		//static const bool value = true;

		static void ToStr(StrAppender &sa, ArgFmt &fmt, long long data) {
			std::string &str = *(std::string*)data;
			if (fmt.width > str.size()) {
				size_t off = fmt.width - str.size();
				sa.AppendN(' ', off);
				sa.Append(str.data(), str.size());
			}
			else {
				sa.Append(str.data(), str.size());
			}
		}
	};

}
