#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE   512

typedef uint8_t BYTE;

//define JPEG signature bytes
const uint8_t JPEG_SIGNATURE[] = {0xFF, 0xD8, 0xFF, 0xE0};   
 
int main(int argc, char *argv[]) {
    // check the correct number of command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s forensic_image\n", argv[0]);
        return 1;
    }

    //open forensic image for reading

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
    	fprintf(stderr,"Could not open file %s.\n", argv[1]);
    	return 2;
    }

    BYTE buffer[BLOCK_SIZE];
    size_t fileCount = 0;
    FILE *outputFile = NULL;

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == BLOCK_SIZE) {
        //check that the file starts with the JPEG signature   
        if (buffer[0] == JPEG_SIGNATURE[0] && buffer[1] == JPEG_SIGNATURE[1] && buffer[2] == JPEG_SIGNATURE[2] && 
        (buffer[3] & 0xF0) == JPEG_SIGNATURE[3]) {
            if (outputFile != NULL) {
                fclose(outputFile);
            }
            //create a  new output file 
            char filename[64];
            sprintf(filename, "%03zu.jpg", ++fileCount);
            outputFile = fopen(filename, "wb");
            if(!outputFile){
            	fprintf(stderr, "Failed to create %s\n", filename);
                fclose(file);
            	return 3;
            }
        
        }
        
        /* write data to current output file */
        if (outputFile) {
            fwrite(buffer, 1, BLOCK_SIZE, outputFile);
        }
    }

    /* close last output file and report on any incomplete files*/
    fclose(file);
    return 0;
    
}