#include <iostream>
#include <set>
#include <string>

Room* FindFinalRoom(Room* start_room) {
    std::set<std::string> keys;
    std::set<Room*> rooms;
    rooms.insert(start_room);
    bool flag = true;
    while (flag) {
        flag = false;
        size_t prev_keys_size = keys.size();
        size_t prev_rooms_size = rooms.size();
        for (Room* current_room : rooms) {
            if (current_room->IsFinal()) {
                return current_room;
            }
            for (int current_key_index = 0;
                 current_key_index < current_room->NumKeys();
                 ++current_key_index) {
                keys.insert(current_room->GetKey(current_key_index));
            }
            for (int current_door_index = 0;
                 current_door_index < current_room->NumDoors();
                 ++current_door_index) {
                Door* current_door = current_room->GetDoor(current_door_index);
                if (!current_door->IsOpen()) {
                    for (std::string key : keys) {
                        if (current_door->TryOpen(key)) {
                            rooms.insert(current_door->GoThrough());
                        }
                    }
                } else {
                    rooms.insert(current_door->GoThrough());
                }
            }
        }
        if (keys.size() > prev_keys_size || rooms.size() > prev_rooms_size) {
            flag = true;
        }
    }
    return nullptr;
}
