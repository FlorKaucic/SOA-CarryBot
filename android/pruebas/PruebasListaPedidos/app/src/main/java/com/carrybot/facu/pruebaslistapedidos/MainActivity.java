package com.carrybot.facu.pruebaslistapedidos;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import java.util.AbstractQueue;
import java.util.ArrayList;
import java.util.Queue;
import java.util.concurrent.ArrayBlockingQueue;

public class MainActivity extends AppCompatActivity {

    //private ListView cola;
    private Button btnAgregar, btnSacar;
    private TextView cant;
    private EditText nuevo, primero;
    private Queue<String> cola;
    private ArrayAdapter<String> adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //cola = (ListView) findViewById(R.id.lvCola);
        btnAgregar = (Button)findViewById(R.id.btnAgregar);
        btnSacar = (Button)findViewById(R.id.btnSacar);
        nuevo = (EditText)findViewById(R.id.etNuevo);
        primero = (EditText)findViewById(R.id.etPrimero);
        cant = (TextView)findViewById(R.id.tvCantidad);
        cola = new ArrayBlockingQueue<String>(5);


    }

    public void agregar(View view){
        cola.offer(nuevo.getText().toString());
        nuevo.setText("");
        cant.setText("En cola: "+cola.size());
    }

    public void quitar(View view){
        primero.setText(cola.poll());
        cant.setText("En cola: "+cola.size());
    }
}
