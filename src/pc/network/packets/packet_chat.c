#include <stdio.h>
#include "../network.h"
#include "game/chat.h"
#include "pc/debuglog.h"

#ifdef DEVELOPMENT
#include "behavior_table.h"

static void print_sync_object_table(void) {
    LOG_INFO("Sync Object Table");
    for (int i = 0; i < MAX_SYNC_OBJECTS; i++) {
        if (gSyncObjects[i].o == NULL) { continue; }
        u16 behaviorId = get_id_from_behavior(gSyncObjects[i].behavior);
        LOG_INFO("%03d: %04X", i, behaviorId);
    }
    LOG_INFO(" ");
}
#endif

void network_send_chat(char* message) {
    u16 messageLength = strlen(message);
    struct Packet p;
    packet_init(&p, PACKET_CHAT, true, false);
    packet_write(&p, &messageLength, sizeof(u16));
    packet_write(&p, message, messageLength * sizeof(u8));
    network_send(&p);
    LOG_INFO("tx chat: %s", message);

#ifdef DEVELOPMENT
    print_sync_object_table();
#endif
}

void network_receive_chat(struct Packet* p) {
    u16 remoteMessageLength = 0;
    char remoteMessage[255] = { 0 };

    packet_read(p, &remoteMessageLength, sizeof(u16));
    if (remoteMessageLength > 255) { remoteMessageLength = 254; }
    packet_read(p, &remoteMessage, remoteMessageLength * sizeof(u8));

    // add the message
    chat_add_message(remoteMessage, CMT_REMOTE);
    LOG_INFO("rx chat: %s", remoteMessage);

#ifdef DEVELOPMENT
    print_sync_object_table();
#endif
}
