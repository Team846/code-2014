################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Networking/NetBuffer.cpp \
../src/Networking/NetClient.cpp \
../src/Networking/NetConnection.cpp \
../src/Networking/NetPeer.cpp \
../src/Networking/NetServer.cpp \
../src/Networking/NetUtil.cpp 

OBJS += \
./src/Networking/NetBuffer.o \
./src/Networking/NetClient.o \
./src/Networking/NetConnection.o \
./src/Networking/NetPeer.o \
./src/Networking/NetServer.o \
./src/Networking/NetUtil.o 

CPP_DEPS += \
./src/Networking/NetBuffer.d \
./src/Networking/NetClient.d \
./src/Networking/NetConnection.d \
./src/Networking/NetPeer.d \
./src/Networking/NetServer.d \
./src/Networking/NetUtil.d 


# Each subdirectory must supply rules for building sources it contributes
src/Networking/%.o: ../src/Networking/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv2 -I/usr/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


