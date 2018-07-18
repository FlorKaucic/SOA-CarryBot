package com.carrybot.server.model;

public enum NotificationType {
    ORDER("ORDER"),
    CANCEL("CANCEL"),
    CARRYING("CARRYING"),
    ARRIVAL("ARRIVAL"),
    NEXT("NEXT");

    private final String type;

    NotificationType(String type) {
        this.type = type;
    }

    public String getType() {
        return this.type;
    }
}
