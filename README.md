# FT_IRC Server

Este proyecto implementa un **servidor IRC (Internet Relay Chat)** básico en C++. El servidor permite que múltiples clientes se conecten simultáneamente para chatear en tiempo real a través de canales :shipit:

## ¿Qué es IRC?

IRC es un protocolo de comunicación que permite el intercambio de mensajes de texto en tiempo real. Los usuarios se conectan a un servidor y pueden:
* Unirse a canales (salas de chat)
* Enviar mensajes privados
* Chatear con otros usuarios conectados

## Componentes del Proyecto

### 🖥️Server (Servidor)
* **Función:** Es el núcleo del sistema que gestiona todas las conexiones
* **Características:**
    * Escuchar conexiones entrantes en un puerto específico
    * Maneja múltiples clientes simultáneamente usando `select()`
    * Procesa comandos IRC y los distribuye a los manejadores apropiados
    * Controla el flujo de datos entre clientes
 
### 👤Client (Cliente)
* **Función:** Representa a cada usuario conectado al servidor
* **Información que almacena:**
    * Nickname (apodo del usuario)
    * Username (nombre de usuario)
    * File descriptor (identificador de conexión)
    * Estado de registro
* **Capacidades:** Puede enviar y recibir mensajes

### 📺Channel (Canal)
* **Función:** Representa una sala de char donde los usuarios pueden comunicarse
* **Características:**
    * Mantiene una lista de miembros
    * Gestiona operadores (usuarios con permisos especiales)
    * Controla invitaciones y modos del canal
    * Permite broadcast de mensajes a todos los miembros
 
### 🔧CommandHandler (Manejador de Comandos)
* **Función:** Procesa y ejecuta todos los comandos IRC recibidos del cliente
* **Características:**
    * Separa la lógica de comandos del servidor principal
    * Maneja validaciones y permisos
    * Gestiona respuestas de error según el protocolo IRC
    * Busca clientes por nickanme para operaciones entre usuarios
 
## Comandos IRC Implementados
El servidor maneja los siguentes comandos básicos de IRC con sus respectivas validaciones:

### 📝Comandos de Registro
* **NICK:** Establece o cambia el nickname del usuario
* **USER:** Registra la información del usuario y completa el proceso de autenticación

### 💬Comandos de Comunicación
* **PRIVMSG:** Envía mensajes privados a usuarios o mensajes públicos a canales
   * Verifica que el usuario esté en el canal antes de enviar mensajes
   * Maneja tanto mensajes directos como mensajes de canal

* **JOIN:** Permite unirse a un canal
   * Crea automáticamente el canal si no existe
   * El creador del canal se convierte en operador automáticamente
   * Respeta el modo invite-only (+i) del canal
   * Elimina invitaciones automáticamente al unirse
 
### 👑Comandos de Administración (Solo Operadores)
* **MODE:** Modifica los modos de un canal
   * `+o [nick]`: Otorga permisos de operador a un usuario
   * `+i`: Activa el modo invite-only (solo por invitación)

* **KICK:** Expulsa a un usuario de un canal
   * Permite agregar una razón opcional para al expulsión
   * Notificación a todos los miembros del canal
   * Remueve al usuario de la lista de miembros y operadores
 
* **INVITE:** Invita a un usuario a un canal con modo invite-only
   * Solo operadores pueden invitar
   * Envía notificación tanto al invitado como al que invita
 
### 🚪Comandos de Desconexión
* **QUIT:** Desconecta al usuario del servidor con mensaje de despedida
