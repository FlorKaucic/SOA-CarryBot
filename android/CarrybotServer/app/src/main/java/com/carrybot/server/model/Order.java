package com.carrybot.server.model;

public class Order {
    private User client;
    private Item item;

    public Order() {}

    public Order(User client, Item item) {
        this.client = client;
        this.item = item;
    }

    public User getClient() {
        return client;
    }

    public void setClient(User client) {
        this.client = client;
    }

    public Item getItem() {
        return item;
    }

    public void setItem(Item item) {
        this.item = item;
    }
}
