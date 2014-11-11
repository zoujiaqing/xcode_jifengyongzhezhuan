// Copyright (c) 2012, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "breakpad_startup.h"
#include <android/log.h>
#include "client/linux/handler/exception_handler.h"
#include "client/linux/handler/minidump_descriptor.h"
#include <sys/stat.h>


static std::string dump_path;

namespace {

bool DumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
                  void* context,
                  bool succeeded) {
/*  printf("Dump path: %s\n", descriptor.path());*/
//  __android_log_print(ANDROID_LOG_INFO, "taomee", "Dump path: %s\n", descriptor.path());
  return succeeded;
}

}  // namespace


void SetDumpPath() {
  google_breakpad::MinidumpDescriptor *descriptor = new google_breakpad::MinidumpDescriptor(dump_path.c_str());
  google_breakpad::ExceptionHandler *eh = new google_breakpad::ExceptionHandler(*descriptor, NULL, DumpCallback,
                                       NULL, true, -1);
}


void breakpadStartup(const char *packageName, const char *versionNumber) {
	std::string s = "/data/data/";
	dump_path = s + packageName;
//	dump_path = dump_path + "/cache";
	dump_path = dump_path + "/dump";
  	mkdir(dump_path.c_str(),0777);
	dump_path = dump_path + "/";
  	dump_path = dump_path + versionNumber;
  	mkdir(dump_path.c_str(),0777);
	SetDumpPath();
}


