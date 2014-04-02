################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HotGoal/HotGoal.cpp 

OBJS += \
./src/HotGoal/HotGoal.o 

CPP_DEPS += \
./src/HotGoal/HotGoal.d 


# Each subdirectory must supply rules for building sources it contributes
src/HotGoal/%.o: ../src/HotGoal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv2 -I/usr/include/boost -O0 -g3 -Wall -c -fmessage-length=0 -fpermissive -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


