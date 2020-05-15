// stdafx.h: Includedatei für Standardsystem-Includedateien
// oder häufig verwendete, projektspezifische Includedateien,
// die nur selten geändert werden.
#pragma once

#ifndef WINVER
#define WINVER 0x0501
#define _WIN32_WINNT 0x500
#else
#if defined(_WIN32_WINNT) && (WINVER < 0x0500) && (_WIN32_WINNT > 0x0500)
#error WINVER setting conflicts with _WIN32_WINNT setting
#endif
#endif

#define WIN32_LEAN_AND_MEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
// C RunTime-Headerdateien
#using <mscorlib.dll>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>

// TODO: Verweisen Sie hier auf zusätzliche Header, die Ihr Programm erfordert
