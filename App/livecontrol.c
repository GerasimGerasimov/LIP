#include "livecontrol.h"
#include <iar_dlmalloc.h>

#include "ramdata.h"

static struct mallinfo IARHeapInfo;

void ctrlSysLive() {

   IARHeapInfo = __iar_dlmallinfo();
   RAM_DATA.UsedMem = IARHeapInfo.uordblks;
   RAM_DATA.TotalMem = IARHeapInfo.usmblks;
}