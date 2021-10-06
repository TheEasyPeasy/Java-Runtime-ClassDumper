#include "jvm_dumper.h"
#include "MinHook.h"
#include <stdio.h>

/* Convert java string to UTF char*. Use local buffer if possible,
   otherwise malloc new memory. Returns null IFF malloc failed. */
static char* getUTF(JNIEnv* env, jstring str, char* localBuf, int bufSize)
{
	char* utfStr = NULL;

	int len = (*env)->GetStringUTFLength(env, str);
	int unicode_len = (*env)->GetStringLength(env, str);
	if (len >= bufSize) {
		utfStr = malloc(len + 1);
		if (utfStr == NULL) {
			return NULL;
		}
	}
	else {
		utfStr = localBuf;
	}
	(*env)->GetStringUTFRegion(env, str, 0, unicode_len, utfStr);

	return utfStr;
}


#define SKIP_JAVA_CLASSES = true;

jclass JNICALL hookedDefineClass(JNIEnv* env, jobject loader, jstring name, jbyteArray data, jint offset, jint length, jobject pd, jstring source) {
	if (data == NULL || length < 0) {
		return defineClassV_(env, loader, name, data, offset, length, pd, source);
	}
	jbyte* body = (jbyte*)malloc(length);
	if (body == NULL) {
		perror("Malloc failed, OutOfMemory exception!");
		return;
	}
	(*env)->GetByteArrayRegion(env, data, offset, length, body);
	if ((*env)->ExceptionOccurred(env)) {
		goto free_mem;
	}

	const char* utfName;
	char buf[128];

	if (name != NULL) {
		utfName = getUTF(env, name, buf, sizeof(buf));
		if (utfName == NULL) {
			goto free_mem;
		}
	}
	else {
		goto free_mem;
	}
	printf("Class name %s", utfName);
	#ifdef SKIP_JAVA_CLASSES 
	if (strstr(utfName, "java.lang")) {
		goto free_mem;
	}
	#endif
	char finalName[128];
	snprintf(finalName, 128, "%s.class", utfName);
	const char* folder = "C:\\JavaClassDump\\";
	char fullPath[1024];
	snprintf(fullPath, 1024, "%s%s", folder, finalName);
	FILE* out = fopen(fullPath, "wb");
	fwrite(body, 1, (*env)->GetArrayLength(env, data), out);
	fclose(out);
	goto free_mem;
	free_mem:
		free(body);
		return defineClassV_(env, loader, name, data, offset, length, pd, source);
}


boolean started = 0;

void hookSetup(void) {
	if (started) return;
	started = 1;
	AllocConsole();
	FILE* fIn;
	FILE* fOut;
	freopen_s(&fIn, "conin$", "r", stdin);
	freopen_s(&fOut, "conout$", "w", stdout);
	freopen_s(&fOut, "conout$", "w", stderr);
	if (MH_Initialize() != MH_OK) {
		puts("Error while initializing hooks!");
		return;
	}
	HMODULE module = GetModuleHandleA("java.dll");
	if (!module) {
		puts("java.dll module not found!");
		return;
	}
	void* defineClass = (void*)GetProcAddress(module, "Java_java_lang_ClassLoader_defineClass1");
	if (!defineClass) {
		puts("defineClass function not found!");
		return;
	}
	if (MH_CreateHook((void*)defineClass, (void*)hookedDefineClass, (void**)(&defineClassV_)) != MH_OK) {
		puts("Creating hook failed!");
		return;
	}
	if (MH_EnableHook((void*)defineClass) != MH_OK) {
		puts("Enabling hook failed!");
		return;
	}
	puts("Java Runtime Class Dumper created by TheEasyPeasy (https://github.com/TheEasyPeasy)");
}
