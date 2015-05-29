#ifndef __siamoz_cn_util
#define __siamoz_cn_util

#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>

using namespace std;

string toString(int a);
int removeFile(const string &url);

string wholeAsciiFile(const string &url);
void write2File_entire(const string &url, const string &s);
int deleteFirstLineOfFile(const string &url);
void deleteFirstLineOfString(string &s);
void append2File(const string &url, const string &s);

bool FileExist(const string &url);

void clear_buff(char *x,size_t s);

#endif
