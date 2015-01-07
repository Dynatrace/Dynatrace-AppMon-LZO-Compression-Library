#include "com_dynatrace_diagnostics_sdk_io_LZOCompression.h"
#include "miniLZO/lzoconf.h"
#include "miniLZO/minilzo.h"
#include <stdlib.h>

/* FROM: testmini.c
 * We want to compress the data block at `in' with length `IN_LEN' to
 * the block at `out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */

#if defined(__LZO_STRICT_16BIT)
#define IN_LEN      (8*1024)
#elif defined(LZO_ARCH_I086) && !defined(LZO_HAVE_MM_HUGE_ARRAY)
#define IN_LEN      (60*1024)
#else
#define IN_LEN      (128*1024l)
#endif
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

static unsigned char __LZO_MMODEL in  [ IN_LEN ];
static unsigned char __LZO_MMODEL out [ OUT_LEN ];


/* Work-memory needed for compression. Allocate memory in units
 * of `lzo_align_t' (instead of `char') to make sure it is properly aligned.
 */

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);
					

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_init
  (JNIEnv *, jclass) {
	return lzo_init();
}

// ----- compress/decompress with static memory allocated above, i.e. having a max input size ---- 

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_getMaxInSize
  (JNIEnv *, jclass) {
	return IN_LEN;
}

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_getMaxOutSize
  (JNIEnv *, jclass) {
	return OUT_LEN;
}


JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_compress
  (JNIEnv *env, jclass, jbyteArray src , jint offset , jint srcLen , jbyteArray dest) {

	if (srcLen>IN_LEN) return -1;

	env->GetByteArrayRegion(src, offset, srcLen, (jbyte*)in);
	if (env->ExceptionOccurred()) {
		return -1;
	}

	lzo_uint destLen = 0;
	int result = lzo1x_1_compress(in, srcLen, out, &destLen, wrkmem);
	if (result!=LZO_E_OK) return -1;

	env->SetByteArrayRegion(dest, 0, (jsize)destLen, (jbyte*)out);	
	if (env->ExceptionOccurred()) {
		return -1;
	}
	return (jsize)destLen;
}

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_decompress
  (JNIEnv *env, jclass, jbyteArray src, jint offset, jint srcLen, jbyteArray dest) {

	if (srcLen>IN_LEN) return -1;

	lzo_uint destLen=0;

	env->GetByteArrayRegion(src, offset, srcLen, (jbyte*)in);
	if (env->ExceptionOccurred()) {
		return -1;
	}

	int result = lzo1x_decompress(in, srcLen, out, &destLen, NULL);
	if (result!=LZO_E_OK) return -1;
	env->SetByteArrayRegion(dest,0,(jsize)destLen,(jbyte*)out);
	if (env->ExceptionOccurred()) {
		return -1;
	}

	return (jsize)destLen;
}

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_decompressSafe
  (JNIEnv *env, jclass, jbyteArray src , jint offset, jint srcLen, jbyteArray dest)
 {
	if (srcLen>IN_LEN) return -1;

	lzo_uint destLen=0;

	env->GetByteArrayRegion(src, offset, srcLen,(jbyte*)in);
	if (env->ExceptionOccurred()) {
		return -1;
	}

	int result = lzo1x_decompress_safe(in, srcLen, out, &destLen,NULL);
	if (result!=LZO_E_OK) return -1;
	env->SetByteArrayRegion(dest,0,(jsize)destLen,(jbyte*)out);
	if (env->ExceptionOccurred()) {
		return -1;
	}

	return (jsize)destLen;
}


// -------- compress/decompress with dynamic memory, i.e. abitrary input-size --------

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_dynMemCompress
(JNIEnv *env, jclass, jbyteArray src, jint offset, jint srcLen, jbyteArray dest){

	lzo_uint destLen=0;
	jbyte *srcBuf = (jbyte*)malloc(srcLen*sizeof(jbyte));
	if (srcBuf==NULL) return -1;

	env->GetByteArrayRegion(src, offset, srcLen, srcBuf);
	if (env->ExceptionOccurred()) {
		return -1;
	}	

	int maxDestLen = srcLen + srcLen / 16 + 64 +3;
	jbyte *destBuf = (jbyte*)malloc(maxDestLen*sizeof(jbyte));
	if (destBuf==NULL) return -1;

	int result = lzo1x_1_compress((unsigned char*)srcBuf,srcLen, (unsigned char*)destBuf, &destLen, wrkmem);
	if (result!=LZO_E_OK)  {
		free(srcBuf);
		free(destBuf);
		return -1;
	}

	env->SetByteArrayRegion(dest, 0, (jsize)destLen, destBuf);	
	if (env->ExceptionOccurred()) {
		return -1;
	}

	free(srcBuf);
	free(destBuf);
	return (jsize)destLen;
}

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_dynMemDecompress
  (JNIEnv *env, jclass, jbyteArray src,  jint offset, jint srcLen, jbyteArray dest) {

	lzo_uint destLen=0;
	jbyte *srcBuf = (jbyte*)malloc(srcLen*sizeof(jbyte));
	env->GetByteArrayRegion(src, offset, srcLen, srcBuf);
	if (env->ExceptionOccurred()) {
		return -1;
	}
	
	int destArraySize=env->GetArrayLength(dest);
	jbyte *destBuf = (jbyte*)malloc(destArraySize*sizeof(jbyte));

	int result = lzo1x_decompress((unsigned char*)srcBuf,srcLen, (unsigned char*)destBuf, &destLen, wrkmem);
	if (result!=LZO_E_OK)  {
		free(srcBuf);
		free(destBuf);
		return -1;
	}

	env->SetByteArrayRegion(dest, 0, (jsize)destLen, destBuf);	
	if (env->ExceptionOccurred()) {
		return -1;
	}

	free(srcBuf);
	free(destBuf);
	return (jsize)destLen;
}

JNIEXPORT jint JNICALL Java_com_dynatrace_diagnostics_sdk_io_LZOCompression_dynMemDecompressSafe
  (JNIEnv *, jclass, jbyteArray, jint, jint, jbyteArray) {
	printf(" - LZOCompressino_dynMemDecompressSafe - not yet implemented\n");
	return -1;
}
