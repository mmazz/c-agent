# Rasp-Agent

Proyecto de aprendizaje en C enfocado en networking, programación de sistemas Linux y diseño de software modular.

## Objetivo

Desarrollar un agente remoto que se ejecute en una Raspberry Pi y permita consultar información del sistema desde otra máquina mediante una interfaz de línea de comandos.

El proyecto está diseñado para construirse de manera incremental, comenzando por conceptos básicos de sockets TCP y evolucionando hacia un servicio de monitoreo remoto más completo.

## Objetivos de aprendizaje

Durante el desarrollo se buscará practicar:

* Programación en C
* Organización de proyectos grandes (`.h` y `.c`)
* Makefiles
* Manejo de memoria
* Manejo de errores
* Networking TCP/IP
* Protocolos de aplicación
* Parsing de texto
* Lectura de información del sistema Linux
* Logging
* Multiplexación de conexiones (`select`)
* Desarrollo sobre Raspberry Pi

## Componentes

### rasp-agent

Proceso servidor que corre en la Raspberry Pi.

Responsabilidades:

* Escuchar conexiones TCP.
* Procesar comandos recibidos.
* Obtener información del sistema.
* Registrar eventos en logs.
* Responder solicitudes de clientes.

### rasp-cli

Cliente de línea de comandos.

Responsabilidades:

* Conectarse al agente.
* Enviar comandos.
* Mostrar respuestas de forma legible.

Ejemplos:

```bash
rasp-cli uptime
rasp-cli mem
rasp-cli status
rasp-cli load
```

## Arquitectura inicial

```text
+------------+        TCP        +-------------+
|  rasp-cli  | <---------------> | rasp-agent  |
+------------+                   +-------------+
                                       |
                                       |
                               +---------------+
                               | Linux System  |
                               +---------------+
```

## Estructura prevista

```text
rasp-agent/
├── Makefile
├── README.md
├── include/
│   ├── common.h
│   ├── protocol.h
│   ├── server.h
│   ├── system.h
│   └── logger.h
├── src/
│   ├── main.c
│   ├── server.c
│   ├── protocol.c
│   ├── system.c
│   └── logger.c
├── cli/
│   ├── main.c
│   └── client.c
├── tests/
└── build/
```

## Roadmap

### Fase 1

* Cliente TCP básico.
* Servidor TCP básico.
* Intercambio simple de mensajes.

### Fase 2

* Protocolo basado en comandos.
* Implementación de:

  * PING
  * TIME
  * UPTIME

### Fase 3

* Módulo de información del sistema.
* Lectura de:

  * `/proc/uptime`
  * `/proc/meminfo`
  * `/proc/loadavg`

### Fase 4

* Logging.
* Archivo de configuración.

### Fase 5

* Cliente de línea de comandos.
* Comandos:

  * status
  * mem
  * load
  * uptime

### Fase 6

* Soporte para múltiples clientes.
* Uso de `select()`.

### Fase 7

* Despliegue en Raspberry Pi.

## Comandos previstos

### Conectividad

```text
PING
```

Respuesta:

```text
PONG
```

### Uptime

```text
UPTIME
```

Respuesta:

```text
352341
```

### Memoria

```text
MEM
```

Respuesta:

```text
16384
```

### Estado general

```text
STATUS
```

Respuesta:

```text
uptime=352341
load=0.21
mem=42
```

## Compilación

```bash
make
```

## Ejecución

Servidor:

```bash
./rasp-agent 8080
```

Cliente:

```bash
./rasp-cli 127.0.0.1 8080 uptime
```

## Requisitos de calidad

Antes de considerar una fase terminada:

* Compila sin warnings.
* Uso de:

  * `-Wall`
  * `-Wextra`
  * `-Werror`
* Sin memory leaks.
* Manejo explícito de errores.
* Código dividido en módulos.
* Funciones pequeñas y específicas.

## Meta final

Disponer de una Raspberry Pi ejecutando un agente permanente de monitoreo accesible desde cualquier equipo de la red local mediante una herramienta propia escrita completamente en C.

