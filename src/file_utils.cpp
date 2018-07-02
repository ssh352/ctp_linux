#include "file_utils.h"

Document d;


bool read_json(const char *file_name,Document &d){

	FILE* fp = fopen(file_name, "r"); // 非 Windows 平台使用 "r"
	if (!fp){
		return false;
	}
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);
	fclose(fp);

	return true;
}
