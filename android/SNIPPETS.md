# Code Snippets

### Modificar la UI de un activity
La clase `Activity` tiene un método `runOnUiThread`, que recibe un `Runnable` (un objeto con un método `run()` que tiene un pedazo de código ejecutable).
El `Activity` corre sobre el _main thread_, mientras que en paralelo se ejecutan otros threads que van haciendo cosas, y sus resultados normalmente hay que reflejarlos en la _UI_, que se debería modificar sólo desde el _main thread_, ya que ésa es su función.
Entonces, para actualizar la _UI_ hay que usar ese método, que corre el `Runnable` en el _main thread_.

```
activity.runOnUiThread(new Runnable() {
    @Override
    public void run() {
        activity.messageTextview.setText("Texto");
    }
});
```

### Solicitar permisos en tiempo de ejecución
Desde Android 6.0, los permisos riesgosos (aquellos que comprometen la privacidad del usuario) requieren pedir el permiso en tiempo de ejecución. Es decir, no alcanza con declararlo en el manifest. Para los demás permisos, no hace falta hacer esto.

Se piden a través de un mensaje como el siguiente:
![Dialog solicitando permiso](https://developer.android.com/images/training/permissions/request_permission_dialog.png)

Para comprobar si un permiso ya existe:
```
int permissionCheck = ContextCompat.checkSelfPermission(thisActivity,
        Manifest.permission.WRITE_CALENDAR);
```

`checkSelfPermission` retorna `PackageManager.PERMISSION_GRANTED` si está permitido, o `PackageManager.PERMISSION_DENIED` en caso contrario.

Para pedir el permiso:
```
ActivityCompat.requestPermissions(thisActivity,
                new String[]{Manifest.permission.READ_CONTACTS},
                MY_PERMISSIONS_REQUEST_READ_CONTACTS);
```

## Soluciones a problemas de Android Studio
Es muy fácil que los archivos se corrompan en Android Studio, por lo cual hice una recopilación de las soluciones más comunes (y asquerosas) para solucionarlos:
- File / "Invalidate cache / Restart" / "Invalidate cache & restart"
- Borrar .idea y .gradle y hacer un nuevo build
- Re-sync los archivos de gradle
- Borrar lineas que esten entrando en conflicto (o todo) del archivo de gradle correspondiente al modulo de la app, syncronizar, volver a poner todo en su lugar y sincronizar nuevamente
- Cerrar y re-importar el proyecto (no volverlo a abrir, re-importarlo) para que cree nuevamente todos los archivos .iml
