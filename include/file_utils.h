#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <rapidjson/writer.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <glog/logging.h>
#include <string>
#include <iostream>

using namespace std;
using namespace rapidjson;

extern Document d;


bool read_json(const char *file_name,Document & d);
class logger
{
	public:
		static inline void init(const char *name)
		{
			string home = "./log/glog/";
			google::InitGoogleLogging(name);
			string info_log = home + "_info_";
			google::SetLogDestination(google::INFO, info_log.c_str());
			string warning_log = home + "_warning_";
			google::SetLogDestination(google::WARNING, warning_log.c_str());
			string error_log = home + "_error_";
			google::SetLogDestination(google::ERROR, error_log.c_str());
			string fatal_log = home + "_fatal_";
			google::SetLogDestination(google::FATAL, fatal_log.c_str());

			// FLAGS_logbuflevel = 1;
			// FLAGS_alsologtostderr = true;
			// FLAGS_logtostderr = true;
			//
			// FLAGS_minloglevel = 1;
			// FLAGS_v = 1;
			// FLAGS_colorlogtostderr = true;
			// FLAGS_logbufsecs;
			//
		}
};
