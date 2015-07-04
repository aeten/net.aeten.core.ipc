package net.aeten.core.ipc;

import java.util.concurrent.TimeUnit;

public class Select {
	private final int fd;

	public Select(int fd) {
		this.fd = fd;
	}

	public static void select(int[] readfds, int[] writefds, int[] exceptfds,
			long timeout, TimeUnit timeUnit) throws SelectError {
		byte[] rfds = fd_set(readfds);
		byte[] wfds = fd_set(writefds);
		byte[] efds = fd_set(exceptfds);
		int nfds = maxfd(writefds, maxfd(readfds, 0)) + 1;
		long[] tv = new long[] { timeUnit.toSeconds(timeout),
				timeUnit.toMicros(timeout) };
		_select(nfds, rfds, wfds, efds, tv);
	}
	// TODO: pselect

	private static byte[] fd_set(int[] fds) {
		byte[] fdset = fds == null ? null : new byte[_sizeof_fd_set()];
		if (fdset != null) {
			_fd_zero(fdset);
			for (int fd : fds) {
				_fd_set(fd, fdset);
			}
		}
		return fdset;
	}

	private static int maxfd(int[] fds, int initial_fdmax) {
		int maxfd = initial_fdmax;
		if (fds != null) {
			for (int fd : fds) {
				if (fd > maxfd)
					maxfd = fd;
			}
		}
		return maxfd;
	}

	private static native int _select(int nfds, byte[] readfds,
			byte[] writefds, byte[] exceptfds, long[] timeout)
			throws SelectError;
	
	private static native int _pselect(int nfds, byte[] readfds,
			byte[] writefds, byte[] exceptfds, long[] timeout, byte[] sigmask)
			throws SelectError;

	// fd_set
	private static native int _sizeof_fd_set();

	private static native void _fd_set(int fd, byte[] fdset);

	private static native void _fd_clr(int fd, byte[] fdset);

	private static native boolean _fd_isset(int fd, byte[] fdset);

	private static native void _fd_zero(byte[] fdset);

}
