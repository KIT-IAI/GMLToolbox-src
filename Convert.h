#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;

#include <stdlib.h>
#include <string>
#include <vector>

typedef std::vector<float> FloatVector;
typedef std::vector<double> DoubleVector;

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//  QuConvert                                                                //
//  Verschiedene Konvertierungs-Routinen für std::string                     //
///////////////////////////////////////////////////////////////////////////////
public class QuConvert
{
public:
  QuConvert(void);
  ~QuConvert(void);
  static void systemStr2stdStr(std::string& toStr, System::String ^ fromStr);

  static String ^ ToString(std::string str);
  static double   ToDouble(std::string str);
  static string   ToAscii(std::string str);

  static String ^ ToString(double d);
  static String ^ ToString(double d, int precision);
  static String ^ ToHtml(std::string str);

};
