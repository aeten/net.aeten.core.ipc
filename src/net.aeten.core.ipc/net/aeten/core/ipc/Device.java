package net.aeten.core.ipc;

public class Device {
	private final String name;
	private final byte[] address;
	private final int index;
	

	public Device(String name, int index, byte[] address) {
		super();
		this.name = name;
		this.address = address;
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
		return address;
	}


	/**
	 * @return the index
	 */
	public int getIndex() {
		return index;
	}

}
