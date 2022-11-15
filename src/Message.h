#ifndef MESSAGE_H
#  define MESSAGE_H

#  include <string>

   using std::string;

   static const string DEFAULT_LOG_NAME = "log.txt";

   class Logger
   {
      public:

         Logger(const string &logFileName);
         Logger() = default;

         bool log(const string &message);

      private:

         string mLogFileName{DEFAULT_LOG_NAME};
   };

   class Message
   {
      public:

         Message(const string &logFileName);
         Message() = default;

         void fatalError(const string &msg);
         bool log(const string &msg);

      private:

         Logger mLogger;
   };

#endif
