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
    unsigned char buffer [512];

    int i = -1;
    char filename [8];
    FILE *img = NULL;



    do
    {
        //read a raw block and store it in buffer
        fread(buffer, sizeof(buffer), 1, raw);

        //if the block indicates a jpg file
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //start the counter from 0
            i++;

            //if its not the first jpg encountered
            if(i != 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", i);

            img = fopen(filename, "w");

            fwrite(buffer, sizeof(buffer), 1, img);

        }

        else if(feof(raw))
        {

            fclose(img);

            fclose(raw);

            return 0;

            i = -2;
        }

        else if(i < 0)
        {
            continue;
        }
        else
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    while (i != -2);

}


























