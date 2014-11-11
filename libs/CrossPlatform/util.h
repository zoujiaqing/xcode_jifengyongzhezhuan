#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cstdlib>

typedef unsigned char byte;
typedef unsigned long ulong;

/*
*convert int to hex char.
*example:10 -> 'A',15 -> 'F'
*/
char intToHexChar(int x);

/*
*convert hex char to int.
*example:'A' -> 10,'F' -> 15
*/
int hexCharToInt(char hex);

using std::string;
/*
*convert a byte array to hex string.
*hex string format example:"AF B0 80 7D"
*/
string bytesToHexString(const byte *in, size_t size);

/*
*convert a hex string to a byte array.
*hex string format example:"AF B0 80 7D"
*/
size_t hexStringToBytes(const string &str, byte *out);
void ShowToast( char* content);

bool isAppOnForeground();
void openWebUrl(std::string urlstring);

#endif/*UTIL_H*/
