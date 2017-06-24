#include "./test_utilities.h"
#include "../Order.h"

static bool testOrderCreate() {
    Order order;
    Escaper escaper;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper)== ESCAPER_SUCCESS);
    Room room;
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    //
    //PEOPLE CHECK
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM-1,MIN_HOUR,10,UNKNOWN,room,
                            escaper,&order) == ORDER_INVALID_PARAMETER);
    //HOUR CHECK
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MAX_HOUR,10,UNKNOWN,room,
                            escaper,&order) == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR-1,10,UNKNOWN,room,
                            escaper,&order) == ORDER_INVALID_PARAMETER);
    //DAY CHECK
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,-1,UNKNOWN,room,
                            escaper,&order) == ORDER_INVALID_PARAMETER);
    //FACULTY CHECK
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,-1,room,escaper,
                            &order) == ORDER_INVALID_PARAMETER);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,FACULTY_NUM,room,
                            escaper,&order) == ORDER_INVALID_PARAMETER);
    //SUCCESS CHECK
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,FACULTY_NUM-1,room,
                            escaper,&order) == ORDER_SUCCESS);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return true;
}

static bool testOrderCopy(){
    Order order;
    Escaper escaper;
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper) == ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,FACULTY_NUM-1,
                            room,escaper,&order) == ORDER_SUCCESS);
    Order order_cpy=orderCopy(order);
    ASSERT_TEST(orderCompare(order,order_cpy)==0);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return  true;
}

static bool testorderCmp(){
    Order order1,order2;
    Escaper escaper;
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper)== ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR, MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,FACULTY_NUM-1,room,
                            escaper,&order1) == ORDER_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,20,0,FACULTY_NUM-1,room,
                            escaper, &order2) == ORDER_SUCCESS);
    ASSERT_TEST(orderCompare(order1,order2)!=0);//Different hour
    orderDestroy(order2);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,2,FACULTY_NUM-1,room,
                            escaper,&order2) == ORDER_SUCCESS);
    ASSERT_TEST(orderCompare(order1,order2)!=0);//different day
    orderDestroy(order2);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,FACULTY_NUM-1,room,
                            escaper,&order2) == ORDER_SUCCESS);
    ASSERT_TEST(orderCompare(order1,order2)==0);//same same
    orderDestroy(order2);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order1);
    return true;
}

static bool testOrderGetFaculty(){
    Order order;
    Escaper escaper;
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper) == ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,COMPUTER_SCIENCE,
                            room,escaper,&order) == ORDER_SUCCESS);
    TechnionFaculty faculty=COMPUTER_SCIENCE;
    ASSERT_TEST(orderGetFaculty(order)==COMPUTER_SCIENCE);
    ASSERT_TEST(faculty==COMPUTER_SCIENCE);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return  true;
}

static bool testOrderGetRoom(){
    Order order;
    Escaper escaper;
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper) == ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,COMPUTER_SCIENCE,
                            room,escaper,&order) == ORDER_SUCCESS);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return  true;
}

static bool testOrderGetEscaper(){
    Order order;
    Escaper escaper;
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper) == ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,COMPUTER_SCIENCE,
                            room,escaper,&order) == ORDER_SUCCESS);
    ASSERT_TEST(escaperCompare(orderGetEscaper(order),escaper)==0);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return  true;
}

static bool testOrderCalculatePrice(){
    Order order;
    Escaper escaper;
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",ELECTRICAL_ENGINEERING,
                              MIN_SKILL_LEVEL,&escaper) == ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,
                            ELECTRICAL_ENGINEERING,room,escaper,&order) ==
                        ORDER_SUCCESS);
    ASSERT_TEST(orderCalculatePrice(order) ==
                MIN_PLAYERS_FOR_ROOM*PRICE_MODULE*DISCOUNT_FOT_FACULTY_MEMBERS);
    orderDestroy(order);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,
                            CHEMICAL_ENGINEERING,room,escaper,&order) ==
                        ORDER_SUCCESS);
    ASSERT_TEST(orderCalculatePrice(order)== MIN_PLAYERS_FOR_ROOM*PRICE_MODULE);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return  true;
}

static bool testOrderGetNumOfPeople(){
    Order order;
    Escaper escaper;//
    Room room;
    ASSERT_TEST(escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,
                              &escaper) == ESCAPER_SUCCESS);
    ASSERT_TEST(roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,
                           MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room) ==
                        ROOM_SUCCESS);
    ASSERT_TEST(orderCreate(MIN_PLAYERS_FOR_ROOM,MIN_HOUR,0,COMPUTER_SCIENCE,
                            room,escaper,&order) == ORDER_SUCCESS);
    ASSERT_TEST(orderGetNumOfPeople(order)==MIN_PLAYERS_FOR_ROOM);
    roomDestroy(room);
    escaperDestroy(escaper);
    orderDestroy(order);
    return  true;
}
int main (int argv, char** arc){
    RUN_TEST(testOrderCreate);
    RUN_TEST(testOrderCopy);
    RUN_TEST(testorderCmp);
    RUN_TEST(testOrderGetFaculty);
    RUN_TEST(testOrderGetRoom);
    RUN_TEST(testOrderGetNumOfPeople);
    RUN_TEST(testOrderGetEscaper);
    RUN_TEST(testOrderCalculatePrice)
    return 0;
}