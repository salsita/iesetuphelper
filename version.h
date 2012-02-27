#pragma once

#define VER_COMPANY		"Salsita"
#define VER_COPYRIGHT	"Copyright © 2012 Salsita. All rights reserved"
#define VER_PRODUCT		"Setup Helper Library"

#define VER_NUMBER_MAJOR	1
#define VER_NUMBER_MINOR	0
#define VER_NUMBER_RELEASE	0

#ifndef VER_NUMBER_BUILD
#define VER_NUMBER_BUILD	-1
#endif

#define _VERSTR(m)		#m
#define VERSTR(m)		_VERSTR(m)
#define _VERSTR4(a, b, c, d)	a.b.c.d
#define VERSTR4(a, b, c, d)	_VERSTR4(a, b, c, d)
#define _VER_STRVER		VERSTR4(VER_NUMBER_MAJOR,VER_NUMBER_MINOR,VER_NUMBER_RELEASE,VER_NUMBER_BUILD)

#define VER_NUMBER_LIST		VER_NUMBER_MAJOR, VER_NUMBER_MINOR, VER_NUMBER_RELEASE, VER_NUMBER_BUILD
#define VER_NUMBER_STRING	VERSTR(_VER_STRVER)
