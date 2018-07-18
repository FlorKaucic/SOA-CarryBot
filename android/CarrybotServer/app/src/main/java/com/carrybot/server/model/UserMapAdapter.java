package com.carrybot.server.model;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.carrybot.server.R;

import java.util.HashMap;

public class UserMapAdapter extends BaseAdapter {
    private final Context context;
    private HashMap<String, User> userMap;

    public UserMapAdapter(HashMap<String, User> userMap, Context context){
        this.userMap  = userMap;
        this.context = context;
    }

    @Override
    public int getCount() {
        return userMap.size();
    }

    @Override
    public User getItem(int position) {
        String[] userMapKeys = userMap.keySet().toArray(new String[userMap.size()]);
        return userMap.get(userMapKeys[position]);
    }

    @Override
    public long getItemId(int arg0) {
        return arg0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        User user = getItem(position);

        View listItem = convertView;

        if(listItem == null)
            listItem = LayoutInflater.from(context).inflate(R.layout.clientlist_item, parent,false);

        TextView name = listItem.findViewById(R.id.clientlist_item_name);
        name.setText(user.getName());

        TextView macAddress = listItem.findViewById(R.id.clientlist_item_mac_address);
        macAddress.setText(user.getMacAddress());

        return listItem;
    }
}