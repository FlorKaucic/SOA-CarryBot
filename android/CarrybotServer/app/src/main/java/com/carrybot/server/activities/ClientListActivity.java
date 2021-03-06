package com.carrybot.server.activities;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ListView;
import android.widget.Toast;

import com.carrybot.server.R;
import com.carrybot.server.model.ServerData;
import com.carrybot.server.model.User;
import com.carrybot.server.model.UserMapAdapter;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

import java.util.HashMap;

public class ClientListActivity extends AppCompatActivity {
    HashMap<String, User> clientList;
    UserMapAdapter clientListAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_client_list);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        clientList = new HashMap<>();
        clientListAdapter = new UserMapAdapter(clientList, this);
        ListView clientList = findViewById(R.id.clientListView);
        clientList.setAdapter(clientListAdapter);

        this.triggerConnectionListener();
    }

    private void triggerConnectionListener() {
        final DatabaseReference connectedUsers = FirebaseDatabase.getInstance().getReference("users");

        connectedUsers.addChildEventListener(new ChildEventListener() {
            @Override
            public void onChildAdded(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {
                User addedUser = dataSnapshot.getValue(User.class);

                if(addedUser.getServer() == ServerData.getServerId()) {
                    clientList.put(addedUser.getId(), addedUser);
                    clientListAdapter.notifyDataSetChanged();
                }
            }

            @Override
            public void onChildRemoved(@NonNull DataSnapshot dataSnapshot) {
                User removedUser = dataSnapshot.getValue(User.class);

                if(removedUser != null) {
                    String removedUserMessage = getString(
                            R.string.clientlist_removed_user,
                            removedUser.getName());

                    Toast.makeText(
                            ClientListActivity.this,
                            removedUserMessage,
                            Toast.LENGTH_SHORT
                    ).show();

                    if (clientList.containsKey(removedUser.getId())) {
                        clientList.remove(removedUser.getId());
                        clientListAdapter.notifyDataSetChanged();
                    }
                }
            }

            @Override
            public void onChildChanged(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {}

            @Override
            public void onChildMoved(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {}

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Snackbar.make(
                        findViewById(R.id.clientlist_layout),
                        R.string.clientlist_firebase_conn_lost,
                        Snackbar.LENGTH_LONG
                ).setAction(
                        getString(R.string.button_exit),
                        new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                moveTaskToBack(true);
                                android.os.Process.killProcess(android.os.Process.myPid());
                                System.exit(1);
                            }
                        }
                ).show();
            }
        });
    }
}
