################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BlobExtractor.cpp \
../DisjointSet.cpp \
../RANSAC.cpp \
../Viewer.cpp \
../main.cpp 

OBJS += \
./BlobExtractor.o \
./DisjointSet.o \
./RANSAC.o \
./Viewer.o \
./main.o 

CPP_DEPS += \
./BlobExtractor.d \
./DisjointSet.d \
./RANSAC.d \
./Viewer.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/raphael/OpenNI-Linux-x64-2.2/Include -I.././gcc/version/include/c++/version/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


