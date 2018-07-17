package com.carrybot.server.model;


import com.carrybot.server.activities.OrdersActivity;
import com.carrybot.server.services.MessagingService;

import java.util.ArrayList;

public class ServerData {
    public static final int FIRST_POSITION = 0;

    private static String MAC_ADDRESS = null;
    private static Integer SERVER_ID = null;
    private static String RESTAURANT_NAME = null;

    private static ArrayList<Order> orders = new ArrayList<>();
    private static ArrayList<Product> menu = new ArrayList<>();

    private static OrdersActivity.OrderChangeListener orderChangeListener = null;

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
        notifyChange();
    }

    public static void putOrderBack(Order order) {
        orders.add(FIRST_POSITION, order);
        notifyChange();
    }


    public static Order removeOrder(String userId) {
        Order removedOrder = null;
        for(Order order : orders) {
            if(order.getClient().getId().equals(userId)) {
                int index = orders.indexOf(order);

                if(index == 0) {
                    String token = orders.get(1).getClient().getId();
                    MessagingService.sendNextTo(token);
                }

                removedOrder = orders.remove(index);
            }
        }

        notifyChange();

        return removedOrder;
    }

    public static ArrayList<Order> getOrderList() {
        return orders;
    }

    public static void addProduct(Product product) {
        menu.add(product);
    }

    public static Product getProduct(int productId) {
        Product requestedProduct = null;

        for(Product product : menu) {
            if (product.getId() == productId) {
                requestedProduct = product;
            }
        }

        return requestedProduct;
    }

    public static int getMenuSize() {
        return menu.size();
    }

    public static void registerOrderListChangeListener(OrdersActivity.OrderChangeListener orderListener) {
        orderChangeListener = orderListener;
    }

    public static void unregisterOrderListChangeListener() {
        orderChangeListener = null;
    }

    public static void notifyChange() {
        if(orderChangeListener != null) {
            orderChangeListener.onChange();
        }
    }

    public static Order removeOrder(int position) {
        Order removedOrder = null;

        if(position < orders.size()) {
            removedOrder = orders.remove(position);
            notifyChange();
        }

        return removedOrder;
    }
}
