
#include "StrBuilder.h"

#define ASSERT(x, y) if(strcmp(x, y) != 0) { printf("failed: %s != %s\n", x, y); }
int main() {
	char b[1024];
	StrBuilder sb;


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

	sb.Fmt("%3d", (unsigned short) - 1);
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


	sb.Fmt("%3.3f", 1.1);
	sprintf(b, "%3.3f", 1.1);
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%3s", "abc");
	sprintf(b, "%3s", "abc");
	ASSERT(sb.Data(), b);
	sb.Clear();

	sb.Fmt("%6s", std::string("abc"));
	sprintf(b, "%6s", "abc");
	ASSERT(sb.Data(), b);
	sb.Clear();

	void * a_pointer = &sb;
	sb.Fmt("%6p", a_pointer);
	sprintf(b, "%6p", a_pointer);
	ASSERT(sb.Data(), b);
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

}
