################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ChatServer.cpp \
../src/GroupServer.cpp \
../src/Start.cpp \
../src/codec.cpp 

CC_SRCS += \
../src/chat.pb.cc \
../src/group.pb.cc 

OBJS += \
./src/ChatServer.o \
./src/GroupServer.o \
./src/Start.o \
./src/chat.pb.o \
./src/codec.o \
./src/group.pb.o 

CC_DEPS += \
./src/chat.pb.d \
./src/group.pb.d 

CPP_DEPS += \
./src/ChatServer.d \
./src/GroupServer.d \
./src/Start.d \
./src/codec.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/moocos/Downloads/build/release-install/include -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/moocos/Downloads/build/release-install/include -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


