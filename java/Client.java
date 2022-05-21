package com.winderton.net;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;
import java.util.logging.Logger;

public class Client {
    private static final Logger LOG = Logger.getLogger(Client.class.getName());
    private final int port;
    private final Scanner scanner;
    private final String serverAddressRaw;

    private InetAddress serverAddress;
    private DatagramSocket socket;

    public Client(int port, String serverAddressRaw) {
        this.port = port;
        this.serverAddressRaw = serverAddressRaw;
        this.scanner = new Scanner(System.in);
    }

    private void init() {
        Thread t2 = new Thread(this::receive);
        try {
            socket = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }

        try {
            serverAddress = InetAddress.getByName(serverAddressRaw);
        } catch (UnknownHostException e) {
            LOG.warning("Can not connect to server: " + serverAddressRaw);
        }

        t2.start();
    }

    public void connect() {
        init();
        for (; ; ) {
            System.out.print("Enter a message: ");
            String message = scanner.nextLine();
            sendMessage(message);
        }
    }

    private void receive() {
        byte[] data = new byte[1024];
        DatagramPacket packet = new DatagramPacket(data, data.length);
        try {
            socket.receive(packet);
        } catch (IOException e) {
            LOG.warning(String.format("An error while receive a package: %s", e.getMessage()));
        }

        print(packet.getData());
    }

    private void sendMessage(String msg) {
        byte[] msgData = msg.getBytes(StandardCharsets.UTF_8);
        DatagramPacket packet = new DatagramPacket(msgData, msgData.length, serverAddress, port);
        try {
            socket.send(packet);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void print(byte[] data) {
        LOG.info(String.format("Received package from %s:%s", serverAddressRaw, port));
        LOG.info(String.format("Received data %s", new String(data, StandardCharsets.UTF_8)));
    }
}
