package net.aeten.core.ipc;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.UnknownHostException;

import net.aeten.core.util.NativeLibraryLoader;

public class Socket {
	static {
		NativeLibraryLoader.loadNativeLibrary(Socket.class, "jnisocket");
	}

	// socket
	public static final int AF_INET = 2;
	public static final int SOCK_STREAM = 1;
	public static final int SOCK_DGRAM = 2;
	public static final int SOCK_RAW = 3;

	// sockopt
	static final int IPPROTO_IP = 0;
	static final int SOL_SOCKET = 1;
	static final int SO_REUSEADDR = 2;
	/** SO_BINDTODEVICE need root rights */
	static final int SO_BINDTODEVICE = 25;
	static final int IP_MULTICAST_LOOP = 34;
	/**
	 * This option can be used to modify the delivery policy of multicast
	 * messages to sockets bound to the wildcard INADDR_ANY address. The
	 * argument is a boolean integer (defaults to 1). If set to 1, the socket
	 * will receive messages from all the groups that have been joined globally
	 * on the whole system. Otherwise, it will deliver messages only from the
	 * groups that have been explicitly joined (for example via the
	 * IP_ADD_MEMBERSHIP option) on this particular socket.
	 */
	static final int IP_MULTICAST_ALL = 49;
	static final int IP_MULTICAST_TTL = 33;
	static final int IP_ADD_MEMBERSHIP = 35;

	private final int sockfd;

	public Socket(int domain, int type, int protocol) throws SocketError {
		super();
		sockfd = _socket(domain, type, protocol);
	}

	@Override
	protected void finalize() throws Throwable {
		super.finalize();
		try {
			close();
		} finally {
		}
	}

	public void setReuse(boolean reuse) throws SocketOptionError {
		_setsockopt_int(sockfd, SOL_SOCKET, SO_REUSEADDR, reuse ? 1 : 0);
	}

	public void setUseLoop(boolean useLoop) throws SocketOptionError {
		_setsockopt_int(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, useLoop ? 1 : 0);
	}

	public void setTtl(int ttl) throws SocketOptionError {
		_setsockopt_int(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, ttl);
	}

	public void setMulticastAllGroups(boolean all) throws SocketOptionError {
		_setsockopt_int(sockfd, IPPROTO_IP, IP_MULTICAST_ALL, all ? 1 : 0);
	}

	/** For now, support only IPv4. */
	public void bind(InetAddress address, int port) throws BindError,
			SocketOptionError {
		if ((address != null) && !(address instanceof Inet4Address)) {
			throw new SocketOptionError("Invalid Address: " + address
					+ ". Need IPv4 address.");
		}
		bind(address == null ? null : address.getHostAddress(), port);
	}

	/** For now, support only IPv4. */
	public void bind(String address, int port) throws BindError {
		_bind_ipv4(sockfd, address, port);
	}

	/** For now, support only IPv4. */
	public void bind(int port) throws BindError, SocketOptionError {
		bind((String) null, port);
	}

	/**
	 * Bind to device
	 * 
	 * @param device
	 *            the selected device
	 * @param port
	 *            the port on which to bind
	 * 
	 *            This implementation not bind to INADDR_ANY and use
	 *            setsockopt(SO_BINDTODEVICE) which need root rights, but use
	 *            ioctl(SIOCGIFCONF) and ioctl(SIOCGIFADDR) to determine the
	 *            device address and bind to that last.
	 **/
	public void bindToDevice(String device, int port) throws SocketOptionError,
			BindError {
		// _setsockopt_string(sockfd, SOL_SOCKET, SO_BINDTODEVICE, device);
		// bind((String)null, port);
		try {
			bind(InetAddress.getByAddress(getDevice(device).getAddress())
					.getHostAddress(), port);
		} catch (UnknownHostException exception) { // Should not happen
			throw new SocketOptionError(exception);
		}
	}

	public Device getDevice(String device) throws SocketOptionError {
		try {
			byte[] ifreq = IOControl._new_ifreq();
			IOControl._ifreq_set_ifr_name(ifreq, device);
			byte[] ifconf = IOControl._new_ifconf(new byte[][] { ifreq });
			IOControl._ioctl(sockfd, IOControl.SIOCGIFCONF, ifconf, 0,
					ifconf.length);
			IOControl._ioctl(sockfd, IOControl.SIOCGIFINDEX, ifreq, 0,
					ifreq.length);
			IOControl._ioctl(sockfd, IOControl.SIOCGIFADDR, ifreq, 0,
					ifreq.length);
			byte[] in_addr = IOControl._ifreq_get_ifr_address(ifreq);
			int if_index = IOControl._ifreq_get_ifr_index(ifreq);
			return new Device(device, if_index, in_addr);
		} catch (IOControlError error) {
			throw new SocketOptionError(error);
		}

	}

	public void joinGroupFromDevice(String groupAddress, String device)
			throws SocketOptionError {
		Device dev = getDevice(device);
		try {
			byte[] ip_mreq = _new_ip_mreq(groupAddress, InetAddress
					.getByAddress(dev.getAddress()).getHostAddress(),
					dev.getIndex());
			_setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, ip_mreq, 0,
					ip_mreq.length);
		} catch (UnknownHostException exception) { // Should not happen
			throw new SocketOptionError(exception);
		}
	}

	public void joinGroup(String groupAddress) throws SocketOptionError {
		byte[] ip_mreq = _new_ip_mreq(groupAddress, null, 0);
		_setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, ip_mreq, 0,
				ip_mreq.length);
	}

	public void close() throws SocketError {
		_close(sockfd);
	}

	private static native int _socket(int domain, int type, int protocol)
			throws SocketError;

	private static native void _close(int sockfd) throws SocketError;

	private static native void _setsockopt(int sockfd, int level, int optname,
			byte[] optval, int offset, int length) throws SocketOptionError;

	private static native void _setsockopt_string(int sockfd, int level,
			int optname, String optval) throws SocketOptionError;

	private static native void _setsockopt_int(int sockfd, int level,
			int optname, int optval) throws SocketOptionError;

	private static native void _bind_ipv4(int sockfd, String address, int port)
			throws BindError;

	/**
	 * @param ipi_ifindex
	 *            the interface index
	 * @param ipi_spec_dst
	 *            the routing destination address
	 * @param ipi_addr
	 *            the header destination address
	 **/
	private static native byte[] _new_in_pktinfo(int ipi_ifindex,
			String ipi_spec_dst, String ipi_addr);

	private static native byte[] _new_ip_mreq(String group,
			String interfaceAddress, int interface_index);

	public static void main(String[] args) {
		Socket socket = null;
		try {
			socket = new Socket(AF_INET, SOCK_DGRAM, 0);
			socket.setMulticastAllGroups(false);
			socket.setReuse(true);
			socket.setTtl(10);
			socket.setUseLoop(true);
//			socket.joinGroup("225.0.0.1");
//			socket.bindToDevice("eth0", 60001);
			socket.joinGroupFromDevice("225.0.0.1", "eth0");
			socket.bind(60001);
		} catch (Throwable e) {
			e.printStackTrace();
		} finally {
			if (socket != null)
				try {
					socket.close();
				} catch (SocketError e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		}
	}

}
