package com.carrybot.server.model;

import java.util.Map;

public class Notification {
    public static final String TYPE = "TYPE";
    public static final String USER_ID = "USER_ID";
    public static final String USER_NAME = "USER_NAME";
    public static final String USER_MAC_ADDRESS = "USER_MAC_ADDRESS";
    public static final String ITEM_ID = "ITEM_ID";
    public static final String ITEM_QUANTITY = "ITEM_QUANTITY";

    private final Map<String, String> data;

    public Notification(Map<String, String> data) {
        this.data = data;
    }

    public void put(String key, String value) {
        this.data.put(key, value);
    }

    public String get(String key) {
        return this.data.get(key);
    }
}
