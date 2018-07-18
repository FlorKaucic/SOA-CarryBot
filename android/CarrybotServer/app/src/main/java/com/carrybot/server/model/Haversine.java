package com.carrybot.server.model;

public class Haversine {
    private static final int EARTH_RADIUS = 6371; // Approx Earth radius in KM

    public static double getDistance(double latitudeA, double longitudeA,
                                     double latitudeB, double longitudeB) {
        double dLat  = Math.toRadians((latitudeB - latitudeA));
        double dLong = Math.toRadians((longitudeB - longitudeA));

        latitudeA = Math.toRadians(latitudeA);
        latitudeB   = Math.toRadians(latitudeB);

        double a = Math.pow(Math.sin(dLat / 2), 2) +
                Math.cos(latitudeA) * Math.cos(latitudeB) * Math.pow(Math.sin(dLong / 2), 2);

        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

        return EARTH_RADIUS * c;
    }
}
