package net.aeten.core.ipc;

import java.net.Inet4Address;

import net.aeten.core.util.NativeLibraryLoader;

public class PosixSocket {
	static {
		NativeLibraryLoader.loadNativeLibrary(PosixSocket.class, "posixsocket");
	}
	
	// sockopt
	public static final int AF_INET = 2;
	public static final int SOCK_STREAM = 1;
	public static final int SOCK_DGRAM = 2;
	public static final int SOCK_RAW = 3;
	private static final int IPPROTO_IP = 0;
	private static final int SOL_SOCKET = 1;
	private static final int SO_BINDTODEVICE = 25;
	private static final int SO_REUSEADDR = 2;
	private static final int IP_MULTICAST_LOOP = 34;
	/** This option can be used to modify the delivery policy of
    multicast messages to sockets bound to the wildcard INADDR_ANY
    address.  The argument is a boolean integer (defaults to 1).
    If set to 1, the socket will receive messages from all the
    groups that have been joined globally on the whole system.
    Otherwise, it will deliver messages only from the groups that
    have been explicitly joined (for example via the
    IP_ADD_MEMBERSHIP option) on this particular socket.*/
	private static final int IP_MULTICAST_ALL = 49; 
	private static final int IP_MULTICAST_TTL = 33;
	private static final int IP_ADD_MEMBERSHIP = 35;

	// ioctl
	private static final int SIOCGIFINDEX = 0x8933;

	
	private final int sockfd;
	
	public PosixSocket(int domain, int type, int protocol) throws SocketError {
		super();
		sockfd = _socket(domain, type, protocol);
	}
	@Override
	protected void finalize() throws Throwable {
		super.finalize();
		try {
			close();
		} finally {}
	}
	
	/*	/** SO_BINDTODEVICE need root rights */
	/* 	public void setDevice(String device) throws SocketOptionError {
		_setsockopt_string(socketFileDescriptor, SOL_SOCKET, SO_BINDTODEVICE, device);
	}*/
	public void setDevice(String device) throws  IOCtlError {
		byte[] if_idx = _new_ifreq();
		_ifreq_set_ifr_name(if_idx, device);
		_ioctl(sockfd, SIOCGIFINDEX, if_idx, 0, if_idx.length);
	}
	
	public void setReuse(boolean reuse) throws SocketOptionError {
		_setsockopt_int(sockfd, SOL_SOCKET, SO_REUSEADDR, reuse? 1: 0);
	}
	public void setUseLoop(boolean useLoop) throws SocketOptionError {
		_setsockopt_int(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, useLoop? 1: 0);
	}
	public void setTtl(int ttl) throws SocketOptionError {
		_setsockopt_int(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, ttl);
	}

	public void setMulticastAllGroups(boolean all) throws SocketOptionError {
		_setsockopt_int(sockfd, IPPROTO_IP, IP_MULTICAST_ALL, all? 1: 0);
	}
	public void bind(Inet4Address address, int port) throws BindError {
		_bind_ipv4(sockfd, address.getHostAddress(), port);
	}
	public void bind(String address, int port) throws BindError {
		_bind_ipv4(sockfd, address, port);
	}
	public void joinGroup(String address, String interfaceAddress) throws SocketOptionError {
		byte[] ip_mreq = _new_ip_mreq(address, interfaceAddress);
		_setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, ip_mreq, 0, ip_mreq.length);
	}
	
	public void close() throws SocketError {
		_close(sockfd);
	}
	
	private static native int _socket(int domain, int type, int protocol) throws SocketError;
	private static native void _close(int sockfd) throws SocketError;
	private static native void _setsockopt(int sockfd, int level, int optname, byte[] optval, int offset, int length) throws SocketOptionError;
	private static native void _setsockopt_string(int sockfd, int level, int optname, String optval) throws SocketOptionError;
	private static native void _setsockopt_int(int sockfd, int level, int optname, int optval) throws SocketOptionError;
	private static native int _ioctl(int sockfd, int request, byte[] optval, int offset, int length) throws IOCtlError;
	private static native void _bind_ipv4(int sockfd, String address, int port) throws BindError;
	
	/**
	 * @param ipi_ifindex the interface index 
	 * @param ipi_spec_dst the routing destination address
	 * @param ipi_addr the header destination address
	 **/
	private static native byte[] _new_in_pktinfo(int ipi_ifindex, String ipi_spec_dst, String ipi_addr);
	/** @return Interface request structure used for socket ioctl's.  All interface
	   ioctl's must have parameter definitions which begin with ifr_name.
	   The remainder may be interface specific.  */
	private static native byte[] _new_ifreq();
	private static native void _ifreq_set_ifr_name(byte[] if_idx, String name);
	private static native byte[] _new_ip_mreq(String group, String interfaceAddress);
	


	public static void main(String[] args) {
		PosixSocket socket = null;
		try {
			socket = new PosixSocket(AF_INET, SOCK_DGRAM, 0);
			socket.setDevice("eth0");
			socket.setMulticastAllGroups(false);
			socket.setReuse(true);
			socket.setTtl(10);
			socket.setUseLoop(true);
			socket.joinGroup("225.0.0.1", null);
			socket.bind((String)null, 60001);
		} catch (Throwable e) {
			e.printStackTrace();
		} finally {
			if(socket != null)
				try {
					socket.close();
				} catch (SocketError e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		}
	}
	
}
