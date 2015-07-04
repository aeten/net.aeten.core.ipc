package net.aeten.core.ipc;

import net.aeten.core.util.NativeLibraryLoader;

public class IOControl {
	static {
		NativeLibraryLoader.loadNativeLibrary(IOControl.class, "jniioctl");
	}
	/* Routing table calls. */
	/** add routing table entry */
	static final int SIOCADDRT = 0x890B;
	/** delete routing table entry */
	static final int SIOCDELRT = 0x890C;
	/** call to routing system */
	static final int SIOCRTMSG = 0x890D;

	/* Socket configuration controls. */
	/** get iface name */
	static final int SIOCGIFNAME = 0x8910;
	/** set iface channel */
	static final int SIOCSIFLINK = 0x8911;
	/** get iface list */
	static final int SIOCGIFCONF = 0x8912;
	/** get flags */
	static final int SIOCGIFFLAGS = 0x8913;
	/** set flags */
	static final int SIOCSIFFLAGS = 0x8914;
	/** get PA address */
	static final int SIOCGIFADDR = 0x8915;
	/** set PA address */
	static final int SIOCSIFADDR = 0x8916;
	/** get remote PA address */
	static final int SIOCGIFDSTADDR = 0x8917;
	/** set remote PA address */
	static final int SIOCSIFDSTADDR = 0x8918;
	/** get broadcast PA address */
	static final int SIOCGIFBRDADDR = 0x8919;
	/** set broadcast PA address */
	static final int SIOCSIFBRDADDR = 0x891a;
	/** get network PA mask */
	static final int SIOCGIFNETMASK = 0x891b;
	/** set network PA mask */
	static final int SIOCSIFNETMASK = 0x891c;
	/** get metric */
	static final int SIOCGIFMETRIC = 0x891d;
	/** set metric */
	static final int SIOCSIFMETRIC = 0x891e;
	/** get memory address (BSD) */
	static final int SIOCGIFMEM = 0x891f;
	/** set memory address (BSD) */
	static final int SIOCSIFMEM = 0x8920;
	/** get MTU size */
	static final int SIOCGIFMTU = 0x8921;
	/** set MTU size */
	static final int SIOCSIFMTU = 0x8922;
	/** set interface name */
	static final int SIOCSIFNAME = 0x8923;
	/** set hardware address */
	static final int SIOCSIFHWADDR = 0x8924;
	/** get encapsulations */
	static final int SIOCGIFENCAP = 0x8925;
	/** set encapsulations */
	static final int SIOCSIFENCAP = 0x8926;
	/** Get hardware address */
	static final int SIOCGIFHWADDR = 0x8927;
	/** Set driver slaving support */
	static final int SIOCGIFSLAVE = 0x8929;
	/** Get driver slaving support */
	static final int SIOCSIFSLAVE = 0x8930;
	/** Add multicast address lists */
	static final int SIOCADDMULTI = 0x8931;
	/** Delete multicast address lists */
	static final int SIOCDELMULTI = 0x8932;
	/** name -> if_index mapping */
	static final int SIOCGIFINDEX = 0x8933;
	/** misprint compatibility :-) */
	static final int SIOGIFINDEX = SIOCGIFINDEX;
	/** set extended flags set */
	static final int SIOCSIFPFLAGS = 0x8934;
	/** get extended flags set */
	static final int SIOCGIFPFLAGS = 0x8935;
	/** delete PA address */
	static final int SIOCDIFADDR = 0x8936;
	/** set hardware broadcast addr */
	static final int SIOCSIFHWBROADCAST = 0x8937;
	/** get number of devices */
	static final int SIOCGIFCOUNT = 0x8938;
	/** Bridging support */
	static final int SIOCGIFBR = 0x8940;
	/** Set bridging options */
	static final int SIOCSIFBR = 0x8941;
	/** Get the tx queue length */
	static final int SIOCGIFTXQLEN = 0x8942;
	/** Set the tx queue length */
	static final int SIOCSIFTXQLEN = 0x8943;

	/* ARP cache control calls. */
	/* 0x8950 - 0x8952 * obsolete calls, don't re-use */
	/** Delete ARP table entry */
	static final int SIOCDARP = 0x8953;
	/** get ARP table entry */
	static final int SIOCGARP = 0x8954;
	/** set ARP table entry */
	static final int SIOCSARP = 0x8955;

	/* RARP cache control calls. */
	/** delete RARP table entry */
	static final int SIOCDRARP = 0x8960;
	/** get RARP table entry */
	static final int SIOCGRARP = 0x8961;
	/** set RARP table entry */
	static final int SIOCSRARP = 0x8962;

	/* Driver configuration calls */
	/** Get device parameters */
	static final int SIOCGIFMAP = 0x8970;
	/** Set device parameters */
	static final int SIOCSIFMAP = 0x8971;

	/* DLCI configuration calls */
	/** Create new DLCI device */
	static final int SIOCADDDLCI = 0x8980;
	/** Delete DLCI device */
	static final int SIOCDELDLCI = 0x8981;

	/* Device private ioctl calls. */

	/**
	 * These 16 ioctls are available to devices via the do_ioctl() device
	 * vector. Each device should include this file and redefine these names as
	 * their own. Because these are device dependent it is a good idea _NOT_ to
	 * issue them to random objects and hope. From 0x89F0 to 0x89FF.
	 */
	static final int SIOCDEVPRIVATE = 0x89F0; /* to 89FF */

	/**
	 * These 16 ioctl calls are protocol private From 0x89E0 to 0x89EF.
	 */
	static final int SIOCPROTOPRIVATE = 0x89E0; /* to 89EF */

	/**
	 * Perform the I/O control operation specified by REQUEST on FD. One
	 * argument may follow; its presence and type depend on REQUEST. Return
	 * value depends on REQUEST. Usually -1 indicates error. Class:
	 * net.aeten.core.net.ipc.Socket Method: _ioctl(III[BII)V
	 */
	static native final int _ioctl(int sockfd, int request, byte[] optval,
			int offset, int length) throws IOControlError;

	/**
	 * @return Interface request structure used for socket ioctl's. All
	 *         interface ioctl's must have parameter definitions which begin
	 *         with ifr_name. The remainder may be interface specific.
	 */
	static native final int _sizeof_ifreq();

	/**
	 * @return Structure used in SIOCGIFCONF request. Used to retrieve interface
	 *         configuration for machine (useful for programs which must know
	 *         all networks accessible).
	 */
	static native final byte[] _new_ifconf(byte[][] ifreq);

	static native final void _ifreq_set_ifr_name(byte[] ifreq, String name);

	static native final byte[] _ifreq_get_ifr_address(byte[] ifreq);
	static native final int _ifreq_get_ifr_index(byte[] ifreq);

}
