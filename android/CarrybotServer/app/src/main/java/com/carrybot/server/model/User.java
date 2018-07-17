package com.carrybot.server.model;

public class User {
    private String id;
    private String name;
    private int server;
    private String macAddress;
    private String token;

    public User() {}

    public User(String id, String name, int server, String macAddress, String token) {
        this.id = id;
        this.name = name;
        this.server = server;
        this.macAddress = macAddress;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getServer() {
        return server;
    }

    public void setServer(int server) {
        this.server = server;
    }

    public String getMacAddress() {
        return macAddress;
    }

    public void setMacAddress(String macAddress) {
        this.macAddress = macAddress;
    }
}
