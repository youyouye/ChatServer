################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/microhttp/Microhttp.cpp 

OBJS += \
./src/microhttp/Microhttp.o 

CPP_DEPS += \
./src/microhttp/Microhttp.d 


# Each subdirectory must supply rules for building sources it contributes
src/microhttp/%.o: ../src/microhttp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/moocos/Downloads/build/release-install/include -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


