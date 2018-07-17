package com.carrybot.server.model;

import com.carrybot.server.services.MessagingService;

import java.util.ArrayList;

public class ServerData {
    private static String MAC_ADDRESS = null;
    private static Integer SERVER_ID = null;
    private static String RESTAURANT_NAME = null;

    private static ArrayList<Order> orders = new ArrayList<>();
    private static ArrayList<Product> menu = new ArrayList<>();

    public static String getMacAddress() {
        return MAC_ADDRESS;
    }

    public static void setMacAddress(String macAddress) {
        MAC_ADDRESS = macAddress;
    }

    public static int getServerId() {
        return SERVER_ID;
    }

    public static void setServerId(int serverId) {
        SERVER_ID = serverId;
    }

    public static String getRestaurantName() {
        return RESTAURANT_NAME;
    }

    public static void setRestaurantName(String restaurantName) {
        RESTAURANT_NAME = restaurantName;
    }

    public static boolean isServerIdSet() {
        return (SERVER_ID != null);
    }

    public static void addOrder(Order order) {
        orders.add(order);
    }

    public static void removeOrder(String userId) {
        for(Order order : orders) {
            if(order.getClient().getId().equals(userId)) {
                int index = orders.indexOf(order);

                if(index == 0) {
                    String token = orders.get(1).getClient().getId();
                    MessagingService.sendNextTo(token);
                }

                orders.remove(index);
            }
        }
    }


    public static void addProduct(Product product) {
        menu.add(product);
    }

    public static ArrayList<Product> getMenu() {
        return menu;
    }
}
