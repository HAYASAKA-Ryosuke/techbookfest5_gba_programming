docker run --rm --user $UID -v $PWD:$PWD -w $PWD gba-build-image arm-none-eabi-gcc -c main.c -mthumb-interwork -Wall -O2 -o main.o
docker run --rm --user $UID -v $PWD:$PWD -w $PWD gba-build-image arm-none-eabi-gcc main.o -mthumb-interwork -specs=gba_mb.specs -o main.elf
docker run --rm --user $UID -v $PWD:$PWD -w $PWD gba-build-image arm-none-eabi-objcopy -v -O binary main.elf main.gba
docker run --rm --user $UID -v $PWD:$PWD -w $PWD gba-build-image gbafix main.gba
