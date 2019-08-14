# StrBuilder

## Basic Usage
```
// use StrBuilder
printf("%s", StrBuilder().Fmt("%d %d %d %d %d", 1, 2, 3, 4, 5).c_str());
// use free function Fmt
printf("%s", Fmt("%d %d %d %d %d", 1, 2, 3, 4, 5).c_str());
// use PrintLn
PrintLn("%d %d %d %d %d", 1, 2, 3, 4, 5);
```
## Variable number of arguments
```
// put as many arguments as you want
sb.Fmt("%f %f %f %f %f %f %f %f %f", 1., 2., 3., 4., 5., 6., 7., 8., 9., 10.);
```

## Type and more safety

### Check number of arguments
```
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
```

### Check if the type match
```
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

```

## Easy use
```
  // %d is OK for any kinds of integer
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

  // %c is OK for any kinds of char
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

  // %d is indeed for character
	sb.Fmt("%d", U'a');
	sprintf(b, "%u", (uint32_t)U'a');
	ASSERT(sb.Data(), b);
	sb.Clear();


	sb.Fmt("%3.3f", 1.1);
	sprintf(b, "%3.3f", 1.1);
	ASSERT(sb.Data(), b);
	sb.Clear();

  // %s is OK for c-style string
	sb.Fmt("%3s", "dd");
	sprintf(b, "%3s", "dd");
	ASSERT(sb.Data(), b);
	sb.Clear();

  // %s is OK for std::string string
	sb.Fmt("%6s", std::string("fsss"));
	sprintf(b, "%6s", "fsss");
	ASSERT(sb.Data(), b);
	sb.Clear();

  // %p is OK for any kinds of pointer
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

  // escape, OK!
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
```

