# IRC Server

Este proyecto implementa un **servidor IRC (Internet Relay Chat)** básico en C++. El servidor permite que múltiples clientes se conecten simultáneamente para chatear en tiempo real a través de canales. :shipit:

## ¿Qué es IRC?

IRC es un protocolo de comunicación que permite el intercambio de mensajes de texto en tiempo real. Los usuarios se conectan a un servidor y pueden:
* Unirse a canales (salas de chat)
* Enviar mensajes privados
* Chatear con otros usuarios conectados

## Componentes del Proyecto

### Server (Servidor)
* **Funció:** Es el núcleo del sistema que gestiona todas las conexiones
* **Características:**
    * Escuchar conexiones entrantes en un puerto específico
    * Maneja múltiples clientes simultáneamente usando `select()`
    * Procesa comandos IRC y los distribuye a los manejadores apropiados
    * Controla el flujo de datos entre clientes
 
### Client (Cliente)
* **Función:** Representa a cada usuario conectado al servidor
* **Información que almacena:**
    * Nickname (apodo del usuario)
    * Username (nombre de usuario)
    * File descriptor (identificador de conexión)
    * Estado de registro
* **Capacidades:** Puede enviar y recibir mensajes

### Channel (Canal)
* **Función:** Representa una sala de char donde los usuarios pueden comunicarse
* **Características:**
    * Mantiene una lista de miembros
    * Gestiona operadores (usuarios con permisos especiales)
    * Controla invitaciones y modos del canal
    * Permite broadcast de mensajes a todos los miembros
 
### CommandHandler (Manejador de Comandos)
* **Función:** Procesa y ejecuta todos los comandos IRC recibidos del cliente
* **Características:**
    * Separa la lógica de comandos del servidor principal
    * Maneja validaciones y permisos
    * Gestiona respuestas de error según el protocolo IRC
    * Busca clientes por nickanme para operaciones entre usuarios
 
## Comandos IRC Implementados
El servidor maneja los siguentes comandos básicos de IRC con sus respectivas validaciones:

