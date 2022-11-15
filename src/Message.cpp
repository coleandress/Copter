#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <SDL2/SDL.h>

#include "Datetimeinfo.h"
#include "Message.h"

// --------------------------------------------
// Utility function: writeToConsole
// --------------------------------------------

static void writeToConsole(const string &msg)
{
   std::cerr << msg << "\n";
}

// --------------------------
// Logger
// --------------------------

Logger::Logger(const string &logFileName) : mLogFileName(logFileName)
{
}

bool Logger::log(const string &message)
{
   std::ofstream out(mLogFileName, std::ios_base::app);

   if(!out)
   {
      writeToConsole("Could not open log file: [" + mLogFileName + "]");
      return(false);
   }

   DateTimeInfo info = getDateTimeInfo();

   using std::setfill;
   using std::setw;

   out <<
      "[" <<
      info.mYear << "-" <<
      setfill('0') << setw(2) << info.mMonth  << "-" <<
      setfill('0') << setw(2) << info.mDay    << " " <<
      setfill('0') << setw(2) << info.mHour   << "-" <<
      setfill('0') << setw(2) << info.mMinute << "-" <<
      setfill('0') << setw(2) << info.mSecond << "]" <<
      " " << message << "\n";

   return(true);
}

// -------------------------------------------------------
// Utility Function: doLog
// Calls: writeToConsole if the call to 'log' fails
// -------------------------------------------------------

static bool doLog(Logger &logger, const string &msg)
{
   if(logger.log(msg))
      return(true);

   writeToConsole(msg);

   return(false);
}

// --------------------------
// Message
// --------------------------

Message::Message(const string &logFileName) : mLogger(logFileName)
{
}

void Message::fatalError(const string &msg)
{
   if(SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", msg.c_str(), NULL))
      doLog(mLogger, string("Call to: 'SDL_ShowSimpleMessageBox' failed (") + SDL_GetError() + ")");

   doLog(mLogger, msg);

   exit(1);
}

bool Message::log(const string &msg)
{
   return(doLog(mLogger, msg));
}
