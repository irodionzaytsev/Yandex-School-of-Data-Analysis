#include "dungeon.h"

//
//  *--------*  Open  *--------*
//  * Room 1 * =====> * Room 2 *
//  *--------*        *        *
//      ||            * [key]  *
//      \/            *--------*
//  *--------*
//  * Room 3 *
//  *--------*
//
Room* ExampleGoToThirdRoom(Room* room_1) {
    // go to room 2
    Room* room_2 = starting_room->GetDoor(0)->GoThrough();

    // pick up key
    std::string key = next_room->GetKey(0);

    // return to room 1 and open second door
    Door* closed_door = starting_room->GetDoor(1);
    closed_door->TryOpen(key);

    // go through door to room 3
    Room* room_3 = closed_door->GoThrough();

    return third_room;
}
