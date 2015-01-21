#include <stdio.h>
#include <string.h>

int pcm2wav(char* in,char* out,short NumChannels, short BitsPerSample,int SamplingRate)
{
    FILE *fin,*fout;
    char buffer[1028];
    int ByteRate = NumChannels*BitsPerSample*SamplingRate/8;
    short BlockAlign = NumChannels*BitsPerSample/8;
    int chunkSize = 16;    
    short audioFormat = 1;
	int DataSize;
    int totalSize;
	int n;

	//get total length
    if((fin = fopen(in,"rb")) == NULL)
    {
        printf("Error opening %s file error \n", in);
		return -1;
    }
	fseek ( fin , 0, SEEK_END );
	DataSize = ftell (fin);
	totalSize = 36 + DataSize;

    if((fout = fopen(out, "wb")) == NULL)
    {
         printf("Error opening %s file error \n", out);
		 return -1;
    }

    //totOutSample = 0;
    fwrite("RIFF", sizeof(char), 4,fout);
    fwrite(&totalSize, sizeof(int), 1, fout);
    fwrite("WAVE", sizeof(char), 4, fout);
    fwrite("fmt ", sizeof(char), 4, fout);
    fwrite(&chunkSize, sizeof(int),1,fout);
    fwrite(&audioFormat, sizeof(short), 1, fout);
    fwrite(&NumChannels, sizeof(short),1,fout);
    fwrite(&SamplingRate, sizeof(int), 1, fout);
    fwrite(&ByteRate, sizeof(int), 1, fout);
    fwrite(&BlockAlign, sizeof(short), 1, fout);
    fwrite(&BitsPerSample, sizeof(short), 1, fout);
    fwrite("data", sizeof(char), 4, fout);
    fwrite(&DataSize, sizeof(int), 1, fout);

	//ready to flush pcm data
	fflush(fout);
	fseek ( fin , 0, SEEK_SET );

	while((n = fread(buffer, 1, sizeof(buffer), fin)) > 0) {
	   if(n != fwrite(buffer, 1, n, fout)) {
		  perror("fwrite");
	   }
	   fflush(fout);
	}

    fclose(fin);
    fclose(fout);

	return 0;
}

int main() {
	pcm2wav("sample.pcm","sample.wav",1,16,8000);
	return 0;
}