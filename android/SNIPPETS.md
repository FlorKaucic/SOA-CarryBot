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
