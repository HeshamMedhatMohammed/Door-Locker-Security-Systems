################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MC2.c \
../buzzer.c \
../dc_motor.c \
../external_eeprom.c \
../gpio.c \
../timer0.c \
../twi_updated.c \
../uart.c 

OBJS += \
./MC2.o \
./buzzer.o \
./dc_motor.o \
./external_eeprom.o \
./gpio.o \
./timer0.o \
./twi_updated.o \
./uart.o 

C_DEPS += \
./MC2.d \
./buzzer.d \
./dc_motor.d \
./external_eeprom.d \
./gpio.d \
./timer0.d \
./twi_updated.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


