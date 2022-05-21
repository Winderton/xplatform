package com.winderton.net;

public class Runner {
    public static void main(String[] args) {
        Client client = new Client(8888, "localhost");
        client.connect();
    }
}
