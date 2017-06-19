PROJ_ROOT = ../..
LIB_NAME = World
LIB_NAMESPACE = world

include $(PROJ_ROOT)/includePre.mk

SRC_PATH = src
INC_PATH = include
TARGET_PATH = test

LIB_SRCS_NAKED = world_time.cpp math_helper.cpp \
	entity/entity.cpp entity/updatable.cpp entity/breakable.cpp entity/obtainable.cpp entity/bullet.cpp entity/character.cpp \
	entity/weapon.cpp \
	entity/machine_gun.cpp entity/machine_gun_bullet.cpp \
	entity/rifle_gun.cpp \
	entity/player.cpp entity/character_tick_data.cpp \
	entity/collider_interface.cpp \
	network/client_snapshot_packet.cpp network/player_input_packet.cpp \
	network/character_snapshot.cpp network/bullet_packet.cpp \
	network/out_packet.cpp network/packet_builder.cpp network/world_snapshot_packet_builder.cpp network/ping_packet_builder.cpp\
	network/packet_parser.cpp network/client_snapshot_packet_parser.cpp network/pong_packet_parser.cpp \
	network/world_guaranteed_packet_builder.cpp network/entity_created_packet.cpp network/entity_destroied_packet.cpp network/entity_obtained_packet.cpp\
	world/world.cpp world/world_map.cpp \
	logger/logger.cpp
TARGET_LIBS_NAKED = network_server.cpp
TARGET_SRCS_NAKED = mock_serv.cpp

SUB_DIRS_STRIPED = math

# nan2 internal libs
NAN2_LIBS += Math

# external libs
EXT_LIBS += pthread boost_system boost_thread boost_log flatbuffers

include $(PROJ_ROOT)/includePost.mk
