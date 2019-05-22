#include "StepperManager.h"
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;




StepperManager::StepperManager()
{
    device = "/dev/i2c-3";
    address = I2CADDRESS;
    homed = false;
    init = true;
    Multiplier = 50;
    writeError("Test if it works");


    i2c_stepper = open(device, O_RDWR);
    if (exitSafeStart() == -1)
    {
        perror(device);
        writeError("constructor");
        init = false;

    }
    else
    {

        if(init)
        {
            Energize();
            setStepMode(2);

            setSpeed(50);
            goAway();
            goHome();
        }
    }
}   

StepperManager::~StepperManager()
{
    if(i2c_stepper > 0)
    {
        closeStep();
    }
}

int StepperManager::setPosition(int pos)
{

    if(!init)
    {
        writeError("setPosition not init");
        return -3;
    }
    while(!homed);
    exitSafeStart();
    pos *= Multiplier;
    uint8_t command[] = {
        0xE0,
        (uint8_t)(pos >> 0 & 0xFF),
        (uint8_t)(pos >> 8 & 0xFF),
        (uint8_t)(pos >> 16 & 0xFF),
        (uint8_t)(pos >> 24 & 0xFF),
    };
    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    printf("set Position : %d\n",pos);


    int targetPosRead = -1;
    int i = 0;
    do
    {
        printf("set Position : %d et targetPosRead : %d\n",pos, targetPosRead);
        getTargetPosition(&targetPosRead);
        if(targetPosRead == 0)
        {
            result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);

        }
        printf("set Position : %d et targetPosRead : %d\n",pos, targetPosRead);
        usleep(100000);
    }while(targetPosRead != pos && i++<30 );


    return 0;

}

int StepperManager::goAway()
{

    if(!init)
    {

        return -3;
    }
    int pos = 10*Multiplier;
    uint8_t command[] = {
        0xE0,
        (uint8_t)(pos >> 0 & 0xFF),
        (uint8_t)(pos >> 8 & 0xFF),
        (uint8_t)(pos >> 16 & 0xFF),
        (uint8_t)(pos >> 24 & 0xFF),
    };
    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);


    int targetPosRead = -1;
    int i = 0;
    do
    {

        getTargetPosition(&targetPosRead);
        if(targetPosRead == 0)
        {
            result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);

        }
        usleep(100000);
    }while(targetPosRead != pos && i++ < 30 );


    return 0;

}

int StepperManager::setSpeed(int speed)
{
    speed = speed*Multiplier*10000;
    uint8_t command[] = {
        0xE6,
        (uint8_t)(speed >> 0 & 0xFF),
        (uint8_t)(speed >> 8 & 0xFF),
        (uint8_t)(speed >> 16 & 0xFF),
        (uint8_t)(speed >> 24 & 0xFF),
    };
    printf("setspeed %d\n",speed);

    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    if (result != 1)
    {
        perror("failed to set speed");
        return -1;
    }
    return 0;

}

int StepperManager::setHomingSpeed(int speed)
{
    uint8_t command[] = {
        0x61,
        (uint8_t)(speed >> 0 & 0xFF),
        (uint8_t)(speed >> 8 & 0xFF),
        (uint8_t)(speed >> 16 & 0xFF),
        (uint8_t)(speed >> 24 & 0xFF),
    };

    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);

    if (result < 0)
    {
        perror("failed to set speed");
        return -1;
    }
    return 0;

}

int StepperManager::setStepMode(uint8_t mode)
{

    if(mode<0 || mode>3)
        return -2;
    uint8_t command[] = {
        0x94,
        (uint8_t)(mode)
    };
    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    if (result != 1)
    {
        perror("failed to set speed");
        return -1;
    }
    switch (mode) {
    case 0:
        Multiplier = 25;

        break;
    case 1:
        Multiplier = 50;
        break;
    case 2:
        Multiplier = 100;
        break;
    case 3:
        Multiplier = 200;
        break;
    default:
        return -2;

    }
    return 0;

}

int StepperManager::exitSafeStart()
{
    uint8_t command[] = { 0x83 };
    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);

    if (result != 1)
    {
        perror("failed to exit safe start");
        return -1;
    }
    return 0;
}

int StepperManager::goHome()
{
    if(!init)
    {
        return -3;
    }
    setHomingSpeed(5000000);
    setStepMode(0);
    uint8_t command[] = { 0x97,
                          0x00
                        };
    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    uint8_t buffer = -1;
    if (result != 1)
    {
        perror("failed to go Home");
        init = false;
        return -1;
    }
    bool homing = true;
    while (homing) {

        getHomingFlag(&buffer);
        if(!buffer)
        {
            homing = false;
        }
        else {
            {
                usleep(10000);
            }
        }
    }
    homed = true;
    setStepMode(2);
    printf("multiplier : %d \n",Multiplier);
}

int StepperManager::getVariable(uint8_t offset, uint8_t * buffer, uint8_t length)
{
    if(!init)
    {
        perror("failed to get variable   not init");
        return -2;
    }
    uint8_t command[] = { 0xA1, offset };
    struct i2c_msg messages[] =
    {
    { address, 0, sizeof(command), command },
    { address, I2C_M_RD, length, buffer }
};
    struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    if (result != 2)
    {
        perror("failed to get variables");
        return -1;
    }
    return 0;
}

int StepperManager::getCurrentPosition(int *output)
{

    if(!init)
    {
        perror("failed to get current position not init");
        return -2;
    }
    *output = 0;
    uint8_t buffer[4];
    int result = getVariable(0x22, buffer, sizeof(buffer));

    if (result) { return -1; }
    *output = buffer[0] + ((uint32_t)buffer[1] << 8) +
            ((uint32_t)buffer[2] << 16) + ((uint32_t)buffer[3] << 24);
    *output = *output;

    return 0;
}

int StepperManager::getTargetPosition(int32_t *output)
{
    if(!init)
    {
        perror("failed to get target position not init");
        return -2;
    }
    *output = -1;
    uint8_t buffer[4];
    int result = getVariable(0x0A   , buffer, sizeof(buffer));
    if (result<0) { return -1; }
    *output = buffer[0] + ((uint32_t)buffer[1] << 8) +
            ((uint32_t)buffer[2] << 16) + ((uint32_t)buffer[3] << 24);
    printf("getTargetPosition : %d\n", *output);
    return 0;
}

int StepperManager::getHomingFlag(uint8_t *output)
{
    if(!init)
    {
        perror("failed to get homing flag not init");
        return -2;
    }
    *output = 0xFF;
    int result = getVariable(0x01, output, sizeof(output));
    *output = *output & 0x10;
    if (result) { return -1; }
    return 0;
}

int StepperManager::deEnergize()
{
    if(!init)
    {
        perror("failed to de-Energized not init");
        return -2;
    }
    while(!homed);
    uint8_t command[] = { 0x86
                        };

    struct i2c_msg message = {address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    if (result != 1)
    {
        perror("failed to de-Energized");
        return -1;
    }
    return 0;
}

int StepperManager::Energize()
{
    if(!init)
    {
        return -2;
    }
    uint8_t command[] = { 0x85
                        };

    struct i2c_msg message = { address, 0, sizeof(command), command };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };
    int result = ioctl(i2c_stepper, I2C_RDWR, &ioctl_data);
    uint8_t buffer[1];
    if (result != 1)
    {
        perror("failed to Energized");
        return -1;
    }
    return 0;
}

int StepperManager::closeStep()
{

    deEnergize();
    int result = close(i2c_stepper);
    return result;
}

int StepperManager::writeError(char *error)
{
    extern char *__progname;
    FILE *fp;

    fp = fopen("error.txt", "a");
    fprintf(fp, "Error from nema in %s with the function %s\n\r",__progname,error);
    fclose(fp);
}
