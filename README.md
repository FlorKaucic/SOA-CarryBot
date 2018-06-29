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
- Porro, Diego

### Directorios
- [Sistema Embebido](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/sistema-embebido "Subdirectorio del SE en GitHub")
- [Android](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/android "Subdirectorio de la app de Android en GitHub")
- [Informe Final](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/informe-final "Subdirectorio del informe final en GitHub")
- [Paper HPC](https://github.com/FlorKaucic/SOA-CarryBot/tree/master/paper-hpc "Subdirectorio del paper de HPC en GitHub")

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
