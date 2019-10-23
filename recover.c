#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover (raw)\n");
        return 1;
    }

    // open input file
    FILE *raw = fopen(argv[1], "r");
    if (!raw)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    //allocate memory for the jpg block
    int *buffer = malloc (512 * sizeof(char));

    int i = -1;
    char *filename = NULL;
    FILE *img = NULL;


    //read a jpg block and store it in buffer
    do
    {
        free(buffer);
        fread(&buffer, 1, 512, raw);

        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            i++;

            if(i == 0)
            {
                sprintf(filename, "%03i.jpg", i);

                img = fopen(filename, "w");

                fwrite(buffer, sizeof(buffer), 1, img);
            }
            else
            {
                fclose(img);

                sprintf(filename, "%03i.jpg", i);

                img = fopen(filename, "w");

                fwrite(buffer, sizeof(buffer), 1, img);
            }
        }

        else if(feof(raw))
        {
            free(buffer);

            fclose(img);

            fclose(raw);

            return 0;

            i = -2;
        }

        else if(i <= 0)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    while (i != -2);

}


























