package com.winderton.net;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Client {

	private int port;
	private String ipaddress;
	private InetAddress serveraddress = null;
	private DatagramSocket socket;
	private Thread t2;
	private String message;
	private Scanner scanner = new Scanner(System.in);
	
	public Client(int port, String ipaddress)  {
		this.port = port;
		this.ipaddress = ipaddress;
	}
	
	
	private void init() {
		
		
		t2 = new Thread(()->receive());
		
		try {
			socket = new DatagramSocket();
		} catch (SocketException e) {
			e.printStackTrace();
		}
		
		try {
			serveraddress = InetAddress.getByName(ipaddress);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
	
		t2.start();
		
	}
	
	
	public void connect() {
		init();
		
		for (;;)
		{
			System.out.print("Enter a message");
			message = scanner.nextLine();
			send(message.getBytes());
		}
	}
	
	
	private void receive() {
		byte data[] = new byte[1024];
		DatagramPacket packet = new DatagramPacket(data, data.length);
		try {
			socket.receive(packet);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		process(packet.getData());
	}
	
	
	private void send(byte data[]) {
		DatagramPacket packet = new DatagramPacket(data, data.length, serveraddress, port);
		try {
			socket.send(packet);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	
	private void process(byte data[]) {
		System.out.println("packet from: " + port + ":" + ipaddress);
		System.out.println("data:" + new String(data));
	}
	
	
	
	public static void main(String[] args) {
		Client client = new Client(8888, "localhost");
		client.connect();
	}
	
}
