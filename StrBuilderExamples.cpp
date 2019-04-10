
#include "StrBuilder.h"
//#include "StrBuilderForTime.h"
//#define FMT_HEADER_ONLY 1
//#include <D:/projects/fmt/fmt/format.h>
#include <chrono>
using namespace strbuilder;

#define ASSERT(x, y) if(strcmp(x, y) != 0) { printf("failed %5d: %s != %s\n", __LINE__, x, y); }

int main() {

	char b[1024];
	StrBuilder sb;

	//fmt::format("%3d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

	sb.Fmt("%d %d %d %d %d %d %d %d %d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	sb.Clear();

	sb.Fmt("%f %f %f %f %f %f %f %f %f", 1., 2., 3., 4., 5., 6., 7., 8., 9., 10.);
	sb.Clear();

 	sb.Fmt("%3d", 1);
	sprintf(b, "%3d", 1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (unsigned long long)-1);
	sprintf(b, "%3llu", (unsigned long long)-1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (signed long long) - 1);
	sprintf(b, "%3lli", (signed long long) - 1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (unsigned long) - 1);
	sprintf(b, "%3lu", (unsigned long) - 1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (signed long) - 1);
	sprintf(b, "%3li", (signed long) - 1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (unsigned short) -1);
	sprintf(b, "%3u", (unsigned short) - 1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (signed short)-1);
	sprintf(b, "%3d", (signed short)-1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (unsigned char)-1);
	sprintf(b, "%3u", (unsigned char)-1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3d", (signed char)-1);
	sprintf(b, "%3d", (signed char)-1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%c", (signed char)'a');
	sprintf(b, "%c", (signed char)'a');
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%c", (unsigned char)'a');
	sprintf(b, "%c", (signed char)'a');
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%c", (unsigned char)'a');
	sprintf(b, "%c", (unsigned char)'a');
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%c", u'a');
	sprintf(b, "%c", u'a');
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%d", U'a');
	sprintf(b, "%u", (uint32_t)U'a');
	ASSERT(sb.Data(), b);
	sb.Clear();


	sb.Fmt("%3.3f", 1.1);
	sprintf(b, "%3.3f", 1.1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3s", "dd");
	sprintf(b, "%3s", "dd");
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%6s", std::string("fsss"));
	sprintf(b, "%6s", "fsss");
	ASSERT(sb.Data(), b);
	sb.Clear();

	void * a_pointer = &sb;
	sb.Fmt("%6p", a_pointer);
	sprintf(b, "%6p", a_pointer);
	ASSERT(sb.Data(), b);
	sb.Clear();

	StrBuilder * ab_pointer = &sb;
	sb.Fmt("%6p", ab_pointer);
	sprintf(b, "%6p", ab_pointer);
	ASSERT(sb.Data(), b);
	sb.Clear();

	char const * str = "def";
	sb.Fmt("%6p", str);
	sprintf(b, "%6p", str);
	ASSERT(sb.Data(), b);
	sb.Clear();

	char const arr[] = "def";
	sb.Fmt("%6p", arr);
	sprintf(b, "%6p", arr);
	ASSERT(sb.Data(), b);
	sb.Clear();

	try {
		sb.Fmt("", 1);
	}
	catch (...) {
		printf("faield %s", R"--(	sb.Fmt("", 1)		)--");
	}
	sb.Clear();

	try {
		sb.Fmt("%d%d", 1);
		printf("faield %s", R"--(	sb.Fmt("%d%d", 1)		)--");
	}
	catch (...) {
	}
	sb.Clear();

	try {
		sb.Fmt("%i", &sb);
		printf("faield %s", R"--(	sb.Fmt("%i", &sb)		)--");
	}
	catch (...) {
	}
	sb.Clear();

	try {
		sb.Fmt("%c", 1);
		printf("faield %s", R"--(	sb.Fmt("%c", 1)		)--");
	}
	catch (...) {
	}
	sb.Clear();

	try {
		sb.Fmt("%u", (int)1);
		printf("faield %s", R"--(	sb.Fmt("%u", (long long)1)		)--");
	}
	catch (...) {
	}
	sb.Clear();

	try {
		sb.Fmt("%i", (unsigned)1);
		printf("faield %s", R"--(	sb.Fmt("%i", (unsigned)1)		)--");
	}
	catch (...) {
	}
	sb.Clear();

	sb.Fmt("%3d %3d", 1, 2);
	sprintf(b, "%3d %3d", 1, 2);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%%abc");
	sprintf(b, "%%abc");
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt(" %%abc");
	sprintf(b, " %%abc");
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt(" %%");
	sprintf(b, " %%");
	ASSERT(sb.Data(), b);
	sb.Clear();


	{
		//using namespace std::chrono;
		//auto t = 5.0ms;
		//sb.Fmt("%9.2m", t);
		//sprintf(b, "%7.2fms", 5.0);
		//ASSERT(sb.Data(), b);
		//sb.Clear();

	}

	{
		//using namespace std::chrono;
		//auto t = 5.0ms;
		//sb.Fmt("%1.2m", t);
		//sprintf(b, "%1.2fms", 5.0);
		//ASSERT(sb.Data(), b);
		//sb.Clear();

	}

	{
		std::string lstr = "ddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
		lstr += lstr;
		auto t0 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < 10000; ++i)
		{
			sb.Fmt(lstr.c_str());
			sb.Clear();
		}
		auto t1 = std::chrono::high_resolution_clock::now();
		auto t3 = t1 - t0;
		PrintLn("benchmark %20d", (t1 - t0).count());
	}


}

#include "StrBuilder.impl.h"
