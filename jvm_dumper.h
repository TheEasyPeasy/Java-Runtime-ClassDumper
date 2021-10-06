#pragma once

#include "jni.h"
#include "jni_md.h"

typedef __int64(__fastcall *DefineClassV)(JNIEnv* env, jobject loader, jstring name, jbyteArray data, jint offset, jint length, jobject pd, jstring source);
DefineClassV defineClassV_;

void hookSetup(void);



