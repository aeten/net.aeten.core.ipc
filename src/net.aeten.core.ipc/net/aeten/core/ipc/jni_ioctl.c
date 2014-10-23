#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include "net/aeten/core/util/jniutil.h"


#define PACKAGE "net/aeten/core/ipc/"
#define IOCTL_ERROR PACKAGE "IOControlError"

/*
 * Perform the I/O control operation specified by REQUEST on FD.
 * One argument may follow; its presence and type depend on REQUEST.
 * Return value depends on REQUEST.  Usually -1 indicates error.
 * Class:     net.aeten.core.net.ipc.IOControl
 * Method:    _ioctl(III[BII)V
 */
JNIEXPORT int JNICALL Java_net_aeten_core_ipc_IOControl__1ioctl(
		JNIEnv *env, jclass class, jint sockfd, jlong request,
		jbyteArray optval, jint offset, jint length/*not used*/) {
	jbyte* bufferPtr = (*env)->GetByteArrayElements(env, optval, 0);
	int ret = ioctl(sockfd, request, bufferPtr + offset);
	if (ret < 0) {
		throw_error(env, IOCTL_ERROR);
	}
	(*env)->ReleaseByteArrayElements(env, optval, bufferPtr, JNI_COMMIT);
	return ret;
}

/*
 * Class:     net.aeten.core.net.ipc.IOControl
 * Method:    _new_ifreq()[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_ipc_IOControl__1new_1ifreq(
		JNIEnv *env, jclass class) {
	int length = sizeof(struct ifreq);
	jbyteArray byte_array = (*env)->NewByteArray(env, length);
	jbyte* pointer = (*env)->GetByteArrayElements(env, byte_array, 0);
	memset(pointer, 0, length);
	(*env)->ReleaseByteArrayElements(env, byte_array, pointer, 0);
	return byte_array;
}

/*
 * Class:     net.aeten.core.net.ipc.IOControl
 * Method:    _new_ifconf()[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_ipc_IOControl__1new_1ifconf(
		JNIEnv *env, jclass class, jobjectArray ifreq_array) {
	int ifc_len = (*env)->GetArrayLength(env, ifreq_array);
	int ifreq_length = sizeof(struct ifreq);
	jbyteArray ifconf_byte_array = (*env)->NewByteArray(env, sizeof(struct ifconf) + ifc_len * (sizeof(struct ifreq*) +  (ifreq_length + 1)));
	struct ifconf* ifc = (struct ifconf*)(*env)->GetByteArrayElements(env, ifconf_byte_array, 0);
	ifc->ifc_len = (*env)->GetArrayLength(env, ifreq_array);

	struct ifreq* p_ifreq[ifc_len];
	for (int i=0; i<ifc->ifc_len; ++i) {
		p_ifreq[i] = (struct ifreq*)(&ifc->ifc_ifcu.ifcu_req + 1 + i);
		jbyteArray ifreq_byte_array = (*env)->GetObjectArrayElement(env, ifreq_array, i);
		struct ifreq* ifr = (struct ifreq*)(*env)->GetByteArrayElements(env, ifreq_byte_array, 0);
		memcpy(&ifc->ifc_ifcu.ifcu_req + ifc->ifc_len + i, ifr, ifreq_length);
		(*env)->ReleaseByteArrayElements(env, ifreq_byte_array, (jbyte*)ifr, JNI_ABORT);
		(*env)->DeleteLocalRef(env, ifreq_byte_array);
	}
	memcpy(&ifc->ifc_ifcu.ifcu_req, &p_ifreq, ifc_len*sizeof(struct ifreq*));

	(*env)->ReleaseByteArrayElements(env, ifconf_byte_array, (jbyte*)ifc, 0);
	return ifconf_byte_array;
}

/*
 * Class:     net.aeten.core.net.ipc.IOControl
 * Method:    _ifreq_set_ifr_name([BLjava.lang.String;)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_IOControl__1ifreq_1set_1ifr_1name(
		JNIEnv *env, jclass class, jbyteArray if_idx, jstring name) {
	struct ifreq* ifr = (struct ifreq*) (*env)->GetByteArrayElements(env, if_idx, 0);
	const char* interface = (*env)->GetStringUTFChars(env, name, 0);
	strcpy(ifr->ifr_ifrn.ifrn_name, interface);
	(*env)->ReleaseStringUTFChars(env, name, interface);
	(*env)->ReleaseByteArrayElements(env, if_idx, (jbyte*) ifr, 0);
}

/*
 * Class:     net.aeten.core.net.ipc.IOControl
 * Method:    _ifreq_get_ifr_address([B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_ipc_IOControl__1ifreq_1get_1ifr_1address(JNIEnv *env, jclass class, jbyteArray if_idx) {
	struct ifreq* if_idx_pointer = (struct ifreq*) (*env)->GetByteArrayElements(env, if_idx, 0);
	struct in_addr addr = ((struct sockaddr_in*)&if_idx_pointer->ifr_ifru.ifru_addr)->sin_addr;
	int length = sizeof(addr);
	jbyteArray byte_array = (*env)->NewByteArray(env, length);
	jbyte* pointer = (*env)->GetByteArrayElements(env, byte_array, 0);
	memcpy(pointer, &addr, length);
	(*env)->ReleaseByteArrayElements(env, byte_array, pointer, 0);
	return byte_array;
}

