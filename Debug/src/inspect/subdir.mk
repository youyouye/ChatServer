################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/inspect/ChatInspector.cpp \
../src/inspect/DataInspector.cpp 

OBJS += \
./src/inspect/ChatInspector.o \
./src/inspect/DataInspector.o 

CPP_DEPS += \
./src/inspect/ChatInspector.d \
./src/inspect/DataInspector.d 


# Each subdirectory must supply rules for building sources it contributes
src/inspect/%.o: ../src/inspect/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/moocos/Downloads/build/release-install/include -I/usr/local/mysql/include -I/home/moocos/moocos/include -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


