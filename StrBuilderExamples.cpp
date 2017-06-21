
#include "StrBuilder.h"


int main() {
	StrBuilder sb;
	sb.Fmt("% %% %", 1, "abc");
	sb.App("abc");
	sb.App(1, "abc");
	sb.App((size_t)1, "abc");

	sb.Clone();

}
