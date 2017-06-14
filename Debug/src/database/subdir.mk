################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/database/TestDatabase.cpp \
../src/database/mysql_conn.cpp \
../src/database/mysql_conn_poll.cpp \
../src/database/offline_msg.cpp \
../src/database/online_user.cpp 

OBJS += \
./src/database/TestDatabase.o \
./src/database/mysql_conn.o \
./src/database/mysql_conn_poll.o \
./src/database/offline_msg.o \
./src/database/online_user.o 

CPP_DEPS += \
./src/database/TestDatabase.d \
./src/database/mysql_conn.d \
./src/database/mysql_conn_poll.d \
./src/database/offline_msg.d \
./src/database/online_user.d 


# Each subdirectory must supply rules for building sources it contributes
src/database/%.o: ../src/database/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/moocos/Downloads/build/release-install/include -I/usr/local/mysql/include -I/home/moocos/moocos/include -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


