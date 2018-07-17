package com.carrybot.server.services;


import com.carrybot.server.model.Notification;
import com.carrybot.server.model.NotificationType;
import com.carrybot.server.model.Order;
import com.carrybot.server.model.ServerData;
import com.carrybot.server.model.User;
import com.carrybot.server.model.Item;
import com.google.firebase.messaging.FirebaseMessaging;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

public class MessagingService extends FirebaseMessagingService {
    final String topicNamePrefix = "server_";

    public MessagingService() {
        String topicName = topicNamePrefix + ServerData.getServerId();
        this.subscribeToTopic(topicName);
    }

    private void subscribeToTopic(String topicName) {
        FirebaseMessaging.getInstance().subscribeToTopic(topicName);
    }

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        if(remoteMessage.getData().size() > 0) {
            Notification notification = new Notification(remoteMessage.getData());
            notification.put(Notification.USER_ID, remoteMessage.getFrom());
            String type = notification.get(Notification.TYPE);
            NotificationType notificationType = NotificationType.valueOf(type);
            switch (notificationType) {
                case ORDER:
                    addOrder(notification);
                    break;
                case CANCEL:
                    cancelOrder(notification);
                    break;
            }
        }
    }

    private void cancelOrder(Notification notification) {
        ServerData.removeOrder(notification.get(Notification.USER_ID));
    }

    private void addOrder(Notification notification) {
        User client = new User();
        client.setId(notification.get(Notification.USER_ID));
        client.setName(notification.get(Notification.USER_NAME));
        client.setMacAddress(notification.get(Notification.USER_MAC_ADDRESS));

        Item item = new Item();
        item.setId(Integer.valueOf(notification.get(Notification.ITEM_ID)));
        item.setQuantity(Integer.valueOf(notification.get(Notification.ITEM_QUANTITY)));

        Order order = new Order(client, item);
        ServerData.addOrder(order);
    }

    public static void sendNextTo(String token) {
    }
}
