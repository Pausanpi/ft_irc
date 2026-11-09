# FT_IRC Server

This project implements a basic **IRC (Internet Relay Chat) server** in C++. It allows multiple clients to connect simultaneously and communicate in real time through channels. :shipit:

## What is IRC?

IRC is a communication protocol that enables real-time text messaging. Users connect to a server and can:

- Join channels (chat rooms).
- Send private messages.
- Chat with other connected users.

## Project Components

### Server

- **Purpose:** Acts as the core of the system, managing all client connections.
- **Features:**
  - Listens for incoming connections on a specific port.
  - Handles multiple clients simultaneously using `select()`.
  - Processes IRC commands and forwards them to the appropriate handlers.
  - Manages data flow between clients.

### Client

- **Purpose:** Represents each user connected to the server.
- **Stored information:**
  - Nickname: User's display name.
  - Username: User's account name.
  - File descriptor: Connection identifier.
  - Registration status.
- **Capabilities:** Can send and receive messages.

### Channel

- **Purpose:** Represents a chat room where users can communicate.
- **Features:**
  - Maintains a list of channel members.
  - Manages operators (users with special privileges).
  - Handles invitations and channel modes.
  - Broadcasts messages to all channel members.

### CommandHandler

- **Purpose:** Processes and executes IRC commands received from clients.
- **Features:**
  - Separates command logic from the main server implementation.
  - Handles validation and permission checks.
  - Manages error responses according to the IRC protocol.
  - Finds clients by nickname for operations involving multiple users.

## Implemented IRC Commands

The server supports the following basic IRC commands, including their corresponding validations.

### Registration Commands

- **NICK:** Sets or changes the user's nickname.
- **USER:** Registers the user's information and completes the registration process.

### Communication Commands

- **PRIVMSG:** Sends private messages to users or public messages to channels.
  - Checks whether the user belongs to the channel before allowing messages.
  - Supports both direct messages and channel messages.

- **JOIN:** Allows a user to join a channel.
  - Automatically creates the channel if it does not exist.
  - The channel creator automatically becomes an operator.
  - Enforces the channel's invite-only mode (`+i`).
  - Automatically removes invitations once a user joins.

### Administration Commands (Operators Only)

- **MODE:** Changes channel modes.
  - `+o [nick]`: Grants operator privileges to a user.
  - `+i`: Enables invite-only mode.

- **KICK:** Removes a user from a channel.
  - Supports an optional reason for the removal.
  - Notifies all channel members.
  - Removes the user from the channel's member and operator lists.

- **INVITE:** Invites a user to an invite-only channel.
  - Only channel operators can send invitations.
  - Notifies both the invited user and the user sending the invitation.

### Disconnection Commands

- **QUIT:** Disconnects a user from the server with an optional quit message.
