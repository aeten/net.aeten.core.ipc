#include <jni.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PACKAGE "net/aeten/core/net/posix/"
#define SOCKET_ERROR PACKAGE "SocketError"
#define SOCKOPT_ERROR PACKAGE "SocketOptionError"
#define IOCTL_ERROR PACKAGE "IOCtlError"
#define BIND_ERROR PACKAGE "BindError"
#define NO_CLASS_DEF_FOUND_ERROR "java/lang/NoClassDefFoundError"
#define UNKNOWN_HOST_EXCEPTION "java/net/UnknownHostException"
#define INTERRUPTED_EXCEPTION "java/lang/InterruptedException"
#define INTERRUPTED_IO_EXCEPTION "java/io/InterruptedIOException"

static jint throw_no_class_def_error(JNIEnv *env, const char *message);
static jint throw_error(JNIEnv *env, const char* className);
static struct in_addr new_ipv4_addr(JNIEnv *env, jstring ipaddr);

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _socket(III)I
 * Signature:
 * @return socket file descriptor
 */
JNIEXPORT jint JNICALL Java_net_aeten_core_net_posix_PosixSocket__1socket(
		JNIEnv *env, jclass class, jint domain, jint type, jint protocol) {
	int sd = socket(domain, type, protocol);
	if (sd == -1)
		throw_error(env, SOCKET_ERROR);
	return sd;
}
/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _close(I)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_net_posix_PosixSocket__1close(
		JNIEnv *env, jclass class, jint sockfd) {
	if (close(sockfd) != 0)
		throw_error(env, SOCKET_ERROR);
}

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _setsockopt(III[BII)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_net_posix_PosixSocket__1setsockopt(
		JNIEnv *env, jclass class, jint sockfd, jint level, jint optname,
		jbyteArray optval, jint offset, jint length) {
	jbyte* bufferPtr = (*env)->GetByteArrayElements(env, optval, 0);
	if (setsockopt(sockfd, level, optname, bufferPtr + offset, length) != 0) {
		throw_error(env, SOCKOPT_ERROR);
	}
	(*env)->ReleaseByteArrayElements(env, optval, bufferPtr, JNI_ABORT);
}

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _new_in_pktinfo(ILjava.lang.String;Ljava.lang.String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_net_posix_PosixSocket__1new_1in_1pktinfo(
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
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _new_in_pktinfo()[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_net_posix_PosixSocket__1new_1ifreq(
		JNIEnv *env, jclass class) {
	int length = sizeof(struct ifreq);
	jbyteArray byte_array = (*env)->NewByteArray(env, length);
	jbyte* pointer = (*env)->GetByteArrayElements(env, byte_array, 0);
	memset(pointer, 0, length);
	(*env)->ReleaseByteArrayElements(env, byte_array, pointer, 0);
	return byte_array;
}

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _new_in_pktinfo()[B
 */
JNIEXPORT jbyteArray JNICALL Java_net_aeten_core_net_posix_PosixSocket__1new_1ip_1mreq(
		JNIEnv *env, jclass class, jstring group, jstring interface) {
	struct ip_mreq mreq;
	mreq.imr_multiaddr = new_ipv4_addr(env, group);

	int error = 0;
	if (interface != NULL) {
		const char* interface_addr = (*env)->GetStringUTFChars(env, interface, 0);
		if (inet_aton(interface_addr, &mreq.imr_interface) < 0) {
			throw_error(env, SOCKOPT_ERROR);
			error = 1;
		}
		(*env)->ReleaseStringUTFChars(env, interface, interface_addr);
	} else {
		mreq.imr_interface.s_addr = INADDR_ANY;
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
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _ifreq_set_ifr_name([BLjava.lang.String;)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_net_posix_PosixSocket__1ifreq_1set_1ifr_1name(
		JNIEnv *env, jclass class, jbyteArray if_idx, jstring name) {
	struct ifreq* if_idx_pointer = (struct ifreq*) (*env)->GetByteArrayElements(
			env, if_idx, 0);
	const char* interface = (*env)->GetStringUTFChars(env, name, 0);
	strcpy(if_idx_pointer->ifr_ifrn.ifrn_name, interface);
	(*env)->ReleaseStringUTFChars(env, name, interface);
	(*env)->ReleaseByteArrayElements(env, if_idx, (jbyte*) if_idx_pointer, 0);
}

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _setsockopt_int(IIII)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_net_posix_PosixSocket__1setsockopt_1int(
		JNIEnv *env, jclass class, jint sockfd, jint level, jint optname,
		jint optval) {
	if (setsockopt(sockfd, level, optname, &optval, sizeof(int)) != 0) {
		throw_error(env, SOCKOPT_ERROR);
	}
}

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _setsockopt_string(IIILjava.lang.String;)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_net_posix_PosixSocket__1setsockopt_1string(
		JNIEnv *env, jclass class, jint sockfd, jint level, jint optname,
		jstring optval) {
	const char* option = (*env)->GetStringUTFChars(env, optval, 0);
	struct ifreq if_idx;
	if (optname == SO_BINDTODEVICE) { // SO_BINDTODEVICE need root rights
		memset(&if_idx, 0, sizeof(struct ifreq));
		strncpy(if_idx.ifr_name, option, IFNAMSIZ - 1);
		if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
			throw_error(env, SOCKOPT_ERROR);
		}
	} else if (setsockopt(sockfd, level, optname, option, strlen(option))
			!= 0) {
		throw_error(env, SOCKOPT_ERROR);
	}
	(*env)->ReleaseStringUTFChars(env, optval, option);
}

/*
 * Perform the I/O control operation specified by REQUEST on FD.
 * One argument may follow; its presence and type depend on REQUEST.
 * Return value depends on REQUEST.  Usually -1 indicates error.
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _ioctl(III[BII)V
 */
JNIEXPORT int JNICALL Java_net_aeten_core_net_posix_PosixSocket__1ioctl(
		JNIEnv *env, jclass class, jint sockfd, jlong request,
		jbyteArray optval, jint offset, jint length/*not used*/) {
	jbyte* bufferPtr = (*env)->GetByteArrayElements(env, optval, 0);
	int ret = ioctl(sockfd, request, bufferPtr + offset);
	if (ret < 0) {
		throw_error(env, IOCTL_ERROR);
	}
	(*env)->ReleaseByteArrayElements(env, optval, bufferPtr, JNI_ABORT);
	return ret;
}

/*
 * Class:     net.aeten.core.net.posix.PosixSocket
 * Method:    _bind_ipv4(ILjava.lang.String;I)V;
 */
JNIEXPORT void JNICALL Java_net_aeten_core_net_posix_PosixSocket__1bind_1ipv4(
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

static jint throw_no_class_def_error(JNIEnv *env, const char *message) {
	jclass exClass = (*env)->FindClass(env, NO_CLASS_DEF_FOUND_ERROR);
	return (*env)->ThrowNew(env, exClass, message);
}

static jint throw_error(JNIEnv *env, const char* className) {
	if (errno == EINTR)
		className = INTERRUPTED_EXCEPTION;
	jclass exClass = (*env)->FindClass(env, className);
	if (exClass == NULL) {
		return throw_no_class_def_error(env, className);
	}
	const char * message = strerror(errno);
	return (*env)->ThrowNew(env, exClass, message);
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

