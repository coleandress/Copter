#ifndef DATE_TIME_INFO_H
#  define DATE_TIME_INFO_H

   struct DateTimeInfo
   {
      int mSecond{0};    // (0 - 59)
      int mMinute{0};    // (0 - 59)
      int mHour{0};      // (0 - 23)
      int mDay{0};       // (1 - 31)
      int mMonth{0};     // (1 - 12)
      int mYear{0};      // (4 digit year)
      int mDayOfWeek{0}; // (0 - 6) (0 -> sun)
      int mDayOfYear{0}; // (0 - 365) (0 -> jan 1)
   }; 

   DateTimeInfo getDateTimeInfo();

#endif
