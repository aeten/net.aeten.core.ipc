#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "net/aeten/core/util/jniutil.h"

#define PACKAGE "net/aeten/core/ipc/"
#define SOCKET_ERROR PACKAGE "SocketError"
#define SOCKOPT_ERROR PACKAGE "SocketOptionError"
#define BIND_ERROR PACKAGE "BindError"

static struct in_addr new_ipv4_addr(JNIEnv *env, jstring ipaddr);

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _socket(III)I
 * Signature:
 * @return socket file descriptor
 */
JNIEXPORT jint JNICALL Java_net_aeten_core_ipc_Socket__1socket(
		JNIEnv *env, jclass class, jint domain, jint type, jint protocol) {
	int sd = socket(domain, type, protocol);
	if (sd == -1)
		throw_error(env, SOCKET_ERROR);
	return sd;
}
/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _close(I)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_Socket__1close(
		JNIEnv *env, jclass class, jint sockfd) {
	if (close(sockfd) != 0)
		throw_error(env, SOCKET_ERROR);
}

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _setsockopt(III[BII)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_Socket__1setsockopt(
		JNIEnv *env, jclass class, jint sockfd, jint level, jint optname,
		jbyteArray optval, jint offset, jint length) {
	jbyte* bufferPtr = (*env)->GetByteArrayElements(env, optval, 0);
	if (setsockopt(sockfd, level, optname, bufferPtr + offset, length) != 0) {
		throw_error(env, SOCKOPT_ERROR);
	}
	(*env)->ReleaseByteArrayElements(env, optval, bufferPtr, JNI_ABORT);
}

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _new_in_pktinfo(ILjava.lang.String;Ljava.lang.String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_ipc_Socket__1new_1in_1pktinfo(
		JNIEnv *env, jclass class, jint ipi_ifindex, jstring ipi_spec_dst,
		jstring ipi_addr) {
	struct in_pktinfo pktinfo;
	pktinfo.ipi_ifindex = ipi_ifindex;
	pktinfo.ipi_spec_dst = new_ipv4_addr(env, ipi_spec_dst);
	pktinfo.ipi_addr = new_ipv4_addr(env, ipi_addr);
	int length = sizeof(pktinfo);
	jbyteArray byte_array = (*env)->NewByteArray(env, length);
	jbyte* pointer = (*env)->GetByteArrayElements(env, byte_array, 0);
	memcpy(pointer, &pktinfo, length);
	(*env)->ReleaseByteArrayElements(env, byte_array, pointer, 0);
	return byte_array;
}

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _new_in_pktinfo()[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_ipc_Socket__1new_1ip_1mreq(
		JNIEnv *env, jclass class, jstring group, jstring interface) {
	struct ip_mreqn mreq;
	mreq.imr_multiaddr = new_ipv4_addr(env, group);

	int error = 0;
	if (interface != NULL) {
		const char* interface_addr = (*env)->GetStringUTFChars(env, interface, 0);
		if (inet_aton(interface_addr, &mreq.imr_address) < 0) {
			throw_error(env, SOCKOPT_ERROR);
			error = 1;
		}
		(*env)->ReleaseStringUTFChars(env, interface, interface_addr);
	} else {
		mreq.imr_address.s_addr = INADDR_ANY;
	}
	if (!error) {
		int length = sizeof(mreq);
		jbyteArray byte_array = (*env)->NewByteArray(env, length);
		jbyte* pointer = (*env)->GetByteArrayElements(env, byte_array, 0);
		memcpy(pointer, &mreq, length);
		(*env)->ReleaseByteArrayElements(env, byte_array, pointer, 0);
		return byte_array;
	}
	return NULL;
}

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _setsockopt_int(IIII)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_Socket__1setsockopt_1int(
		JNIEnv *env, jclass class, jint sockfd, jint level, jint optname,
		jint optval) {
	if (setsockopt(sockfd, level, optname, &optval, sizeof(int)) != 0) {
		throw_error(env, SOCKOPT_ERROR);
	}
}

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _setsockopt_string(IIILjava.lang.String;)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_Socket__1setsockopt_1string(
		JNIEnv *env, jclass class, jint sockfd, jint level, jint optname,
		jstring optval) {
	const char* option = (*env)->GetStringUTFChars(env, optval, 0);
	if (setsockopt(sockfd, level, optname, option, strlen(option)) != 0) {
		throw_error(env, SOCKOPT_ERROR);
	}
	(*env)->ReleaseStringUTFChars(env, optval, option);
}

/*
 * Class:     net.aeten.core.net.ipc.Socket
 * Method:    _bind_ipv4(ILjava.lang.String;I)V;
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_Socket__1bind_1ipv4(
		JNIEnv *env, jclass class, jint sockfd, jstring ipaddr, jint port) {
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((uint16_t) port);
	addr.sin_addr.s_addr = new_ipv4_addr(env, ipaddr).s_addr;
	if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		throw_error(env, BIND_ERROR);
	}
}


static struct in_addr new_ipv4_addr(JNIEnv *env, jstring ipaddr) {
	struct in_addr sin_addr;
	if (ipaddr == NULL) {
		sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		const char* address = (*env)->GetStringUTFChars(env, ipaddr, 0);
		sin_addr.s_addr = inet_addr(address);
		if (sin_addr.s_addr == INADDR_NONE) {
			jclass exClass = (*env)->FindClass(env, UNKNOWN_HOST_EXCEPTION);
			if (exClass == NULL) {
				throw_no_class_def_error(env, UNKNOWN_HOST_EXCEPTION);
			}
			char * message = strdup(address);
			(*env)->ThrowNew(env, exClass, message);
		}
		(*env)->ReleaseStringUTFChars(env, ipaddr, address);
	}
	return sin_addr;
}


