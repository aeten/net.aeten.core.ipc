package net.aeten.core.ipc;

import java.util.Arrays;

import net.jcip.annotations.Immutable;

@Immutable
public class Device {
	private final String name;
	private final byte[] address;
	private final int index;
	

	public Device(String name, int index, byte[] address) {
		super();
		this.name = name;
		this.address = Arrays.copyOf(address, address.length);
		this.index = index;
	}


	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}


	/**
	 * @return the address
	 */
	public byte[] getAddress() {
		return Arrays.copyOf(address, address.length);
	}


	/**
	 * @return the index
	 */
	public int getIndex() {
		return index;
	}

}
