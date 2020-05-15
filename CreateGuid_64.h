#ifndef _CREATEGUID64_H_
#define _CREATEGUID64_H_

///////////////////////////////////////////////////////////////////////////////
//  Verschiedene Strukturen und Routinen für GUIDs                           //
///////////////////////////////////////////////////////////////////////////////

typedef struct _GUID {          // size is 16
  unsigned long Data1;
  unsigned short  Data2;
  unsigned short  Data3;
  unsigned char Data4[8];
} GUID;

typedef int BOOL;

char * CreateCompressedGuidString( char * buf, int len );
BOOL  GetGuidFromString64( const char *string, GUID *pGuid );
void CreateReadableStringFromGuid (GUID *pGuid, char *);
void CreatetGuidFromReadableString (char *guid, GUID *pGuid);
char * getString64FromGuid( const GUID *pGuid, char * buf, int len );

#endif