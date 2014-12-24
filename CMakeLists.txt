project(MasterBuilder)
cmake_minimum_required(VERSION 2.8)

# find Google protobuf
find_package(Protobuf REQUIRED)
# todo - invoke protoc

# shared
set(SharedSources
	Shared/Application.cpp
	Shared/Messaging.cpp
	Generated/Messages.pb.cc
	)

set(SharedHeaders
	Shared/Application.h
	Shared/ApplicationState.h
	Shared/Messaging.h
	Generated/Messages.pb.h
	)

set(SharedIncludes
	Shared
	Generated
	)

# master
set(MBSources
	MasterBuilder/ApplicationState_Running.cpp
	MasterBuilder/ApplicationState_Shutdown.cpp
	MasterBuilder/ApplicationState_Startup.cpp
	MasterBuilder/CitizenDB.cpp
	MasterBuilder/MasterBuilderApplication.cpp
	MasterBuilder/MasterSocket.cpp
	MasterBuilder/main.cpp)

set(MBHeaders
	MasterBuilder/ApplicationState_Running.h
	MasterBuilder/ApplicationState_Shutdown.h
	MasterBuilder/ApplicationState_Startup.h
	MasterBuilder/CitizenDB.h
	MasterBuilder/MasterBuilderApplication.h
	MasterBuilder/MasterSocket.h)
   
set(MBIncludes
	MasterBuilder)

add_executable(MasterBuilder ${MBSources} ${MBHeaders} ${SharedSources} ${SharedHeaders})

# C++11 is a requirement
set_property(TARGET MasterBuilder PROPERTY CXX_STANDARD 11)
set_property(TARGET MasterBuilder PROPERTY CXX_STANDARD_REQUIRED ON)
# also force manually (for older cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_include_directories(MasterBuilder PUBLIC ${MBIncludes} ${SharedIncludes}
	${PROTOBUF_INCLUDE_DIRS})
target_link_libraries(MasterBuilder ${PROTOBUF_LIBRARIES})


# citizen
set(CBSources
	CitizenBuilder/AppState_Running.cpp
	CitizenBuilder/AppState_Shutdown.cpp
	CitizenBuilder/AppState_Startup.cpp
	CitizenBuilder/CitizenBuilder.cpp
	CitizenBuilder/CitizenSocket.cpp
	CitizenBuilder/main.cpp)

set(CBHeaders
	CitizenBuilder/AppState_Running.h
	CitizenBuilder/AppState_Shutdown.h
	CitizenBuilder/AppState_Startup.h
	CitizenBuilder/CitizenBuilder.h
	CitizenBuilder/CitizenSocket.h)
   
set(CBIncludes
	CitizenBuilder)

add_executable(CitizenBuilder ${CBSources} ${CBHeaders} ${SharedSources} ${SharedHeaders})

# C++11 is a requirement
set_property(TARGET CitizenBuilder PROPERTY CXX_STANDARD 11)
set_property(TARGET CitizenBuilder PROPERTY CXX_STANDARD_REQUIRED ON)
# also force manually (for older cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_include_directories(CitizenBuilder PUBLIC ${CBIncludes} ${SharedIncludes}
	${PROTOBUF_INCLUDE_DIRS})
target_link_libraries(CitizenBuilder ${PROTOBUF_LIBRARIES})
