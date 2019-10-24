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

    //start counter, allocate an array for jpg names, and declare the pointer to the image data
    int i = -1;
    char filename [8];
    FILE *img = NULL;

    //loop start
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
                //close the opened jpg and continue
                fclose(img);
            }

            //create the jpg name
            sprintf(filename, "%03i.jpg", i);
            //open the file under jpg name
            img = fopen(filename, "w");
            //write the block from the buffer in that file
            fwrite(buffer, sizeof(buffer), 1, img);

        }
        //if we encounter the end of file
        else if(feof(raw))
        {
            //close everything, success
            fclose(img);

            fclose(raw);

            return 0;

        }
        //if we havent encountered the first jpg yet, repeat the loop
        else if(i < 0)
        {
            continue;
        }
        //if the block isnt a start of the jpg, write the buffer to the previously opened jpg
        else
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    //loop will work until we reach end of raw file
    while (1);
}