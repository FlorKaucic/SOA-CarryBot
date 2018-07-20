# SOA-CarryBot
Proyecto de IoT con Arduino y Android para Sistemas Operativos Avanzados (UNLaM)

### Status
[![Build Status](https://travis-ci.org/FlorKaucic/SOA-CarryBot.svg?branch=master)](https://travis-ci.org/FlorKaucic/SOA-CarryBot)

## Proyecto
### Integrantes
- Bistolfi, Facundo
- Kaucic, Florencia
- Martinez, Sebastian
- Palopoli, Juan Jose

### Directorios
- [Sistema Embebido](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/sistema-embebido "Subdirectorio del SE en GitHub")
- [Android](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/android "Subdirectorio de la app de Android en GitHub")
- [Informe Final](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/informe-final "Subdirectorio del informe final en GitHub")
- [Paper HPC](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/paper-hpc "Subdirectorio del paper de HPC en GitHub")

### Presentación del producto
Se realizó un video de presentación del proyecto como producto.
[Link al video](https://youtu.be/WMHQTdCognk "Presentación del producto")

### Firebase
El proyecto utiliza un servidor Firebase.
Para utilizarlo en necesario crear un proyecto en Firebase (se puede usar el tier gratuito, llamado Firebase Spark, que incluye todo lo necesario para que la app funcione) y luego descargar el archivo `google-services.json` desde las settings del proyecto, que deberá ir ubicado en `./android/<Proyecto>/app/`.

## Release
Se creó un tag del proyecto a la fecha de la entrega.

## Commits
Los commits tienen declarado qué contexto modifican al principio del mensaje de commit.

Este __contexto__ puede ser:
- `[repo] : tareas generales y de mantenimiento del repo`
- `[ardu] : cambios en el código del sistema embebido arduino`
- `[andr] : cambios en el código de la app de android`
- `[docs] : cambios en el informe final`
- `[phpc] : cambios en el paper de HPC`

El objetivo es simplificar la obtención de un historial de cada parte por separado.

El mensaje del commit deberá respetar el formato `contexto mensaje`, separados por un espacio. El contexto debe incluir los corchetes y el mensaje podrá contener los siguientes caracteres: `Letras, números, espacios y paréntesis`.
De otra manera, el build fallará en el stage de `Commit`.

## Snippets
En el subdirectorio de Android hay un archivo `SNIPPETS.md`. Su objetivo era contener snippets de código común, pero también tiene tips para solucionar los problemas comunes de Android Studio. 
