#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define popen _popen
#define pclose _pclose
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#include "metadata.h"

#define W 640               // width
#define H 480               // height
#define FPS 30              // fps 
#define TF 6572             // frames
#define CS 5                // how many frames until another version

int main() {
    char *out = "ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 640x480 -i - "
                    "-c:v libx264 -preset fast -pix_fmt yuv420p final_bad_apple.mp4";  
    FILE *fout = popen(out, "wb");
    if (!fout) { perror("Output pipe failed"); return 1; }
    unsigned char *fb = (unsigned char *)malloc(W * H * 3);
    for (int i = 0; i < TF; i += CS) {
        VideoSource current = collection[(i / CS) % COLLECTION_SIZE];
        double st = (double)i / FPS;
        char title[256], auth[256];
        strncpy(title, current.title, 255); strncpy(auth, current.author, 255);
        for(int j=0; title[j]; j++) if(title[j]=='\'' || title[j]==':') title[j]=' ';
        for(int j=0; auth[j]; j++) if(auth[j]=='\'' || auth[j]==':') auth[j]=' ';
        char in_cmd[2048];
        snprintf(in_cmd, sizeof(in_cmd),
                 "ffmpeg -ss %.3f -i \"%s\" "
                 "-vf \"scale=%d:%d:force_original_aspect_ratio=decrease,pad=%d:%d:(ow-iw)/2:(oh-ih)/2,"
                 "drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='%s':x=20:y=H-50:fontsize=20:fontcolor=white:box=1:boxcolor=black,"
                 "drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='%s':x=20:y=H-25:fontsize=16:fontcolor=white:box=1:boxcolor=black\" "
                 "-f image2pipe -vframes %d -pix_fmt rgb24 -vcodec rawvideo -loglevel error -",
                 st, current.filename, W, H, W, H, title, auth, CS);

        FILE *fin = popen(in_cmd, "rb");
        if (fin) {
            for (int f = 0; f < CS && (i + f) < TF; f++) {
                if (fread(fb, 1, W * H * 3, fin) == W * H * 3) {
                    fwrite(fb, 1, W * H * 3, fout);
                }
            }
            pclose(fin);
        }

        printf("Progress: %d / %d frames (Source: %s)\n", i, TF, title);
    }
    free(fb);
    pclose(fout);
    return 0;
}