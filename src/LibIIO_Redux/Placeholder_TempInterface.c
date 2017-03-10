//Test module for Temp-Module while LibIIO is being ported
//
//acts as random number generator to generate fake temp vals

#include "Placeholder_TempInterface.h"

void getTempVals(char* Buff)
{
  float RTDRandom = 0,TCRandom = 0;
  
  RTDRandom = rand()/(float)0x3fffffff*(float)2;
  TCRandom = rand()/(float)0x3fffffff*(float)2;
  
  sprintf(Buff, "CJT:%f;TCT:%f;\n\r", (float)20 + RTDRandom, (float)21 + TCRandom);
}
  
