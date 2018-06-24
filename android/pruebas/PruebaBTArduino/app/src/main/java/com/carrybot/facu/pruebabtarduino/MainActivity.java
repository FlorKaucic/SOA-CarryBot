package com.carrybot.facu.pruebabtarduino;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;
import java.util.concurrent.ExecutionException;

public class MainActivity extends AppCompatActivity {

    //Elementos visuales
    private BluetoothAdapter mBluetoothAdapter;
    private ListView listaDispositivos;
    private ProgressBar pbBarra;

    //Listas para los dispositivos
    private HashMap<String,BluetoothDevice> dispList;
    private ArrayList<String> nomDispList;
    //private ArrayList<String> dispList;
    private ArrayAdapter<String> adaptadorDisp;

    //Socket
    private BluetoothSocket mSocket;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Asigno los objetos visibles que voy a utilizar
        listaDispositivos = (ListView) findViewById(R.id.listaDispositivos);
        pbBarra = (ProgressBar) findViewById(R.id.progressBar);

        //Cargo e inicializo los arrays correspondientes a la lista de dispositivos
        //dispList = new ArrayList<BluetoothDevice>();
        dispList = new HashMap<String,BluetoothDevice>();
        nomDispList = new ArrayList<String>();
        adaptadorDisp=new ArrayAdapter<String>(this,android.R.layout.simple_expandable_list_item_1,nomDispList);
        listaDispositivos.setAdapter(adaptadorDisp);

        //Creo el BroadcastReceiver y le asigno los eventos a detectar
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        filter.addAction(BluetoothDevice.ACTION_NAME_CHANGED);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(mReceiver, filter); // Don't forget to unregister during onDestroy

        //Cargo el adaptador del BT
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        listaDispositivos.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                conectar(listaDispositivos.getItemAtPosition(i).toString());
            }
        });
    }

    private void encenderBT(){
        //Compruebo si el dispositivo tiene BT
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Tu dispositivo no posee Bluetooth", Toast.LENGTH_LONG).show();
            return;
        }
        //Compruebo de que el dispositivo tenga el BT encendido y de no ser asi le pido al usuario que lo encienda
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent,1);
        }
    }

    //Configuro el Broadcast receiver
    BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            //Toast.makeText(MainActivity.this, "Received action: "+action, Toast.LENGTH_SHORT).show();
            //Cuando se encuentra un nuevo dispositivo
            if (BluetoothDevice.ACTION_NAME_CHANGED.equals(action)) {
                // Obtengo el dispositivo y lo asigno a las listas
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                dispList.put(device.getName() + "\n" + device.getAddress(),device);
                nomDispList.add(device.getName() + "\n" + device.getAddress());
                adaptadorDisp.notifyDataSetChanged();
            }
            //Cuando se comienza a buscar dispositivos
            if(BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)){
                //Toast.makeText(MainActivity.this,"Discovery Started", Toast.LENGTH_SHORT).show();
                pbBarra.setVisibility(View.VISIBLE);
            }
            //Cuando se termina de buscar dispositivos
            if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)){
                //Toast.makeText(MainActivity.this,"Discovery Finished", Toast.LENGTH_SHORT).show();
                pbBarra.setVisibility(View.INVISIBLE);
            }
        }
    };

    public void buscar(View view){
        encenderBT();
        if (!mBluetoothAdapter.isEnabled()) {
            Toast.makeText(this, "No se ha habilitado el BT", Toast.LENGTH_LONG).show();
            return;
        }
        dispList.clear();
        nomDispList.clear();
        adaptadorDisp.notifyDataSetChanged();
        // Cancel the discovery if it has already started
        if (mBluetoothAdapter.isDiscovering()) {
            mBluetoothAdapter.cancelDiscovery();
        }
        new Thread(new Runnable() {
            public void run() {
                mBluetoothAdapter.startDiscovery();
                try {
                    Thread.sleep(5000);
                }
                catch (Exception ex){
                    ex.printStackTrace();
                }
                mBluetoothAdapter.cancelDiscovery();
            }
        }).start();
    }

    public void conectar(String selected){
        dispList.get(selected).createBond();
    }


}
