PROJ_ROOT = ../..
LIB_NAME = World
LIB_NAMESPACE = world

include $(PROJ_ROOT)/includePre.mk

SRC_PATH = src
INC_PATH = include
TARGET_PATH = test

LIB_SRCS_NAKED = world_time.cpp math_helper.cpp \
	entity/updatable.cpp entity/entity.cpp entity/bullet.cpp entity/character.cpp entity/entity.cpp \
	entity/machine_gun.cpp entity/machine_gun_bullet.cpp entity/weapon.cpp entity/player.cpp entity/character_tick_data.cpp \
	entity/collider_interface.cpp \
	network/player_input_packet.cpp network/bullet_packet.cpp network/character_snapshot.cpp \
	world/world.cpp world/world_map.cpp 
TARGET_SRCS_NAKED = void_pointer_test.cpp mock_serv.cpp

SUB_DIRS_STRIPED = math

# nan2 internal libs
NAN2_LIBS += Math

# external libs
EXT_LIBS += boost_system flatbuffers

include $(PROJ_ROOT)/includePost.mk
