
#include "./test_utilities.h"
#include "../Room.h"
#include <string.h>


static bool testroomCreate() {
    Room room;
    //ID CHECKS
    ASSERT_TEST(roomCreate(0,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_INVALID_PARAMETER);
    ASSERT_TEST(roomCreate(-1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_INVALID_PARAMETER);
    //Price check/// in case moudle=1 all prices are allowed
    if(PRICE_MODULE!=1){
        ASSERT_TEST(roomCreate(1,PRICE_MODULE+1,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                               MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                            ROOM_INVALID_PARAMETER);
        ASSERT_TEST(roomCreate(1,0,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                               MIN_DIFFICULTY_LEVEL,&room) ==
                            ROOM_INVALID_PARAMETER);
        ASSERT_TEST(roomCreate(1,-1,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                               MIN_DIFFICULTY_LEVEL,&room) ==
                            ROOM_INVALID_PARAMETER);
    }
    //num_ppl check
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM-1,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_INVALID_PARAMETER);
    //HOURS CHECK
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR-1,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_INVALID_PARAMETER);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR+1,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_INVALID_PARAMETER);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MAX_HOUR,
                           MIN_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_INVALID_PARAMETER);
    //DIFFICULTY CHECK
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL-1,&room) ==
                        ROOM_INVALID_PARAMETER);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MAX_DIFFICULTY_LEVEL+1,&room) ==
                        ROOM_INVALID_PARAMETER);
    //
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    roomDestroy(room);
    return true;
}
static bool testroomCopy(){
    Room room;
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room)== ROOM_SUCCESS);
    Room room_cpy=roomCopy(room);
    ASSERT_TEST(roomCompare(room,room_cpy) == 0);
    roomDestroy(room);
    roomDestroy(room_cpy);
    return true;
}

static bool testroomCompare(){
    Room room1,room2;
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room1) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(roomCreate(2,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room2) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(roomCompare(room1,room2)!=0);
    roomDestroy(room2);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room2) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(roomCompare(room1,room2)==0);
    roomDestroy(room2);
    roomDestroy(room1);
    return  true;
}

static bool testroomGetId(){
    Room room;
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR
            ,MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) == ROOM_SUCCESS);
    ASSERT_TEST(roomGetId(room)==1);
    roomDestroy(room);
    return  true;
}
static bool testroomGetPrice(){
    Room room;
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(roomGetPrice(room) == PRICE_MODULE);
    roomDestroy(room);
    return  true;
}
static bool testroomGetRecommendedNumberOfPeople(){
    Room room;
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(roomGetRecommendedNumOfPeople(room)==MIN_PLAYERS_FOR_ROOM);
    roomDestroy(room);

    return  true;
}
int main (int argv, char** arc){
    RUN_TEST(testroomCreate);
    RUN_TEST(testroomCopy);
    RUN_TEST(testroomCompare);
    RUN_TEST(testroomGetId);
    RUN_TEST(testroomGetPrice);
    RUN_TEST(testroomGetRecommendedNumberOfPeople);

    return 0;
}

