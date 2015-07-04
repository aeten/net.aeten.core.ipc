package net.aeten.core.ipc;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.nio.channels.DatagramChannel;

/** TODO */
public class PosixDatagramSocket extends DatagramSocket {

	public PosixDatagramSocket() throws IOException {
		// TODO Auto-generated constructor stub
	}

	public PosixDatagramSocket(int port) throws IOException {
		// TODO Auto-generated constructor stub
	}

	public PosixDatagramSocket(SocketAddress bindaddr) throws IOException {
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#bind(java.net.SocketAddress)
	 */
	@Override
	public synchronized void bind(SocketAddress addr) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#connect(java.net.InetAddress, int)
	 */
	@Override
	public void connect(InetAddress address, int port) {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#connect(java.net.SocketAddress)
	 */
	@Override
	public void connect(SocketAddress addr) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#disconnect()
	 */
	@Override
	public void disconnect() {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#isBound()
	 */
	@Override
	public boolean isBound() {
		// TODO Auto-generated method stub
		return false;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#isConnected()
	 */
	@Override
	public boolean isConnected() {
		// TODO Auto-generated method stub
		return false;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getInetAddress()
	 */
	@Override
	public InetAddress getInetAddress() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getPort()
	 */
	@Override
	public int getPort() {
		// TODO Auto-generated method stub
		return -1;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getRemoteSocketAddress()
	 */
	@Override
	public SocketAddress getRemoteSocketAddress() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getLocalSocketAddress()
	 */
	@Override
	public SocketAddress getLocalSocketAddress() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#send(java.net.DatagramPacket)
	 */
	@Override
	public void send(DatagramPacket p) throws IOException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#receive(java.net.DatagramPacket)
	 */
	@Override
	public synchronized void receive(DatagramPacket p) throws IOException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getLocalAddress()
	 */
	@Override
	public InetAddress getLocalAddress() {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getLocalPort()
	 */
	@Override
	public int getLocalPort() {
		// TODO Auto-generated method stub
		return -1;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#setSoTimeout(int)
	 */
	@Override
	public synchronized void setSoTimeout(int timeout) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getSoTimeout()
	 */
	@Override
	public synchronized int getSoTimeout() throws SocketException {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#setSendBufferSize(int)
	 */
	@Override
	public synchronized void setSendBufferSize(int size) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getSendBufferSize()
	 */
	@Override
	public synchronized int getSendBufferSize() throws SocketException {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#setReceiveBufferSize(int)
	 */
	@Override
	public synchronized void setReceiveBufferSize(int size)
			throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getReceiveBufferSize()
	 */
	@Override
	public synchronized int getReceiveBufferSize() throws SocketException {
		// TODO Auto-generated method stub
		return super.getReceiveBufferSize();
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#setReuseAddress(boolean)
	 */
	@Override
	public synchronized void setReuseAddress(boolean on) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getReuseAddress()
	 */
	@Override
	public synchronized boolean getReuseAddress() throws SocketException {
		// TODO Auto-generated method stub
		return false;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#setBroadcast(boolean)
	 */
	@Override
	public synchronized void setBroadcast(boolean on) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getBroadcast()
	 */
	@Override
	public synchronized boolean getBroadcast() throws SocketException {
		// TODO Auto-generated method stub
		return false;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#setTrafficClass(int)
	 */
	@Override
	public synchronized void setTrafficClass(int tc) throws SocketException {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getTrafficClass()
	 */
	@Override
	public synchronized int getTrafficClass() throws SocketException {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#close()
	 */
	@Override
	public void close() {
		// TODO Auto-generated method stub
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#isClosed()
	 */
	@Override
	public boolean isClosed() {
		// TODO Auto-generated method stub
		return true;
	}

	/* (non-Javadoc)
	 * @see java.net.DatagramSocket#getChannel()
	 */
	@Override
	public DatagramChannel getChannel() {
		// TODO Auto-generated method stub
		return null;
	}

}
