package com.carrybot.server.model;

public class Restaurant {
    private double latitude;
    private double longitude;
    private int id;
    private String name;

    public Restaurant() {}

    public Restaurant(int id, String name, double latitude, double longitude) {
        this.latitude = latitude;
        this.longitude = longitude;
        this.id = id;
        this.name = name;
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
