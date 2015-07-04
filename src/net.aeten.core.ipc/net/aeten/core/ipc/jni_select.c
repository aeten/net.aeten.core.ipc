#include <sys/select.h>
//#include <sys/time.h>
//#include <sys/types.h>
//#include <unistd.h>
#include "net/aeten/core/util/jniutil.h"



#define PACKAGE "net/aeten/core/ipc/"
#define SELECT_ERROR PACKAGE "SelectError"

/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _select(III[BII)I
 */
JNIEXPORT jint JNICALL Java_net_aeten_core_ipc_Select__1select(
		JNIEnv *env, jclass class, jint nfds, jbyteArray readfds, jbyteArray writefds,
		jbyteArray exceptfds, jlongArray timeout) {
	fd_set set;
	jbyte* rfds = (*env)->GetByteArrayElements(env, readfds, 0);
	jbyte* wfds = (*env)->GetByteArrayElements(env, writefds, 0);
	jbyte* efds = (*env)->GetByteArrayElements(env, exceptfds, 0);
	jbyte* tv = (*env)->GetByteArrayElements(env, timeout, 0);
	int ret = select((int)nfds, (fd_set *)rfds,(fd_set *)wfds, (fd_set *)efds, (struct timeval *)tv);
	if (ret == -1) {
		throw_error(env, SELECT_ERROR);
	}
	(*env)->ReleaseByteArrayElements(env, readfds, rfds, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env, writefds, wfds, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env, exceptfds, efds, JNI_COMMIT);
	(*env)->ReleaseByteArrayElements(env, timeout, tv, JNI_ABORT);
	return ret;
}

/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _select(III[BII)I
 */
JNIEXPORT jint JNICALL Java_net_aeten_core_ipc_Select__1pselect(
		JNIEnv *env, jclass class, jint nfds, jbyteArray readfds, jbyteArray writefds,
		jbyteArray exceptfds, jlongArray timeout, jbyteArray sigmask) {
	fd_set set;
	jbyte* rfds = readfds == NULL ? NULL: (*env)->GetByteArrayElements(env, readfds, 0);
	jbyte* wfds = writefds == NULL ? NULL: (*env)->GetByteArrayElements(env, writefds, 0);
	jbyte* efds = exceptfds == NULL ? NULL: (*env)->GetByteArrayElements(env, exceptfds, 0);
	jbyte* tv = timeout == NULL ? NULL: (*env)->GetByteArrayElements(env, timeout, 0);
	jbyte* sigset = sigmask == NULL ? NULL: (*env)->GetByteArrayElements(env, sigmask, 0);
	int ret = pselect((int)nfds, (fd_set *)rfds,(fd_set *)wfds, (fd_set *)efds, (const struct timespec *)tv, (const sigset_t *)sigset);
	if (ret == -1) {
		throw_error(env, SELECT_ERROR);
	}
	if (readfds != NULL) (*env)->ReleaseByteArrayElements(env, readfds, rfds, JNI_COMMIT);
	if (writefds != NULL) (*env)->ReleaseByteArrayElements(env, writefds, wfds, JNI_COMMIT);
	if (exceptfds != NULL) (*env)->ReleaseByteArrayElements(env, exceptfds, efds, JNI_COMMIT);
	if (timeout != NULL) (*env)->ReleaseByteArrayElements(env, timeout, tv, JNI_ABORT);
	if (sigmask != NULL) (*env)->ReleaseByteArrayElements(env, sigmask, sigset, JNI_ABORT);
	return ret;
}


/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _sizeof_fd_set()[I
 */
JNIEXPORT jint JNICALL Java_net_aeten_core_ipc_IOControl__1sizeof_1fd_1set(
		JNIEnv *env, jclass class) {
	return sizeof(fd_set);
}

/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _fd_set(IB[)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_IOControl__1fd_1set(
		JNIEnv *env, jclass class, jint fd, jbyteArray set) {
	jbyte* fds = (*env)->GetByteArrayElements(env, set, 0);
	FD_SET((int)fd, (fd_set *)fds);
	(*env)->ReleaseByteArrayElements(env, set, fds, JNI_COMMIT);
}

/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _fd_clr(IB[)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_IOControl__1fd_1clr(
		JNIEnv *env, jclass class, jint fd, jbyteArray set) {
	jbyte* fds = (*env)->GetByteArrayElements(env, set, 0);
	FD_CLR((int)fd, (fd_set *)fds);
	(*env)->ReleaseByteArrayElements(env, set, fds, JNI_COMMIT);
}

/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _fd_zero(IB[)V
 */
JNIEXPORT void JNICALL Java_net_aeten_core_ipc_IOControl__1fd_1zero(
		JNIEnv *env, jclass class, jbyteArray set) {
	jbyte* fds = (*env)->GetByteArrayElements(env, set, 0);
	FD_ZERO((fd_set *)fds);
	(*env)->ReleaseByteArrayElements(env, set, fds, JNI_COMMIT);
}

/*
 * Class:     net.aeten.core.net.ipc.Select
 * Method:    _sizeof_fd_isset(IB[)B
 */
JNIEXPORT jboolean JNICALL Java_net_aeten_core_ipc_IOControl__1fd_1isset(
		JNIEnv *env, jclass class, jint fd, jbyteArray set) {
	jbyte* fds = (*env)->GetByteArrayElements(env, set, 0);
	int isset = FD_ISSET((int)fd, (fd_set *)fds);
	(*env)->ReleaseByteArrayElements(env, set, fds, JNI_ABORT);
	return isset == 0 ? JNI_FALSE: JNI_TRUE;

}
