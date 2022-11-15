#include <ctime>
#include "Datetimeinfo.h"

DateTimeInfo getDateTimeInfo()
{
   DateTimeInfo info = { 0, 0, 0, 0, 0, 0, 0, 0 };

   time_t now{};

   time(&now);

   struct tm theTime{};

#ifdef _WIN32

   if(localtime_s(&theTime, &now))
      return(info);

#else

   struct tm *ptr = localtime(&now);

   if(!ptr)
      return(info);

   theTime = *ptr;

#endif

   info.mSecond    = theTime.tm_sec;
   info.mMinute    = theTime.tm_min;
   info.mHour      = theTime.tm_hour;
   info.mDay       = theTime.tm_mday;
   info.mMonth     = theTime.tm_mon + 1;
   info.mYear      = theTime.tm_year + 1900;
   info.mDayOfWeek = theTime.tm_wday;
   info.mDayOfYear = theTime.tm_yday;

   return(info);
}
