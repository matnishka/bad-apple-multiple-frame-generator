import yt_dlp
import time
import os
import glob

def down_ba(q="Bad Apple", limit=6161):
    if not os.path.exists('downloads'):
        os.makedirs('downloads')
    ydl_opts = {
        'format': 'bestvideo[height<=480][ext=mp4]+bestaudio[ext=m4a]/best[height<=480][ext=mp4]',
        'outtmpl': 'downloads/%(id)s.%(ext)s',
        'noplaylist': True,
        'sleep_interval': 1,
        'max_sleep_interval': 1,
        'ignoreerrors': True,
        'quiet': False,
        'no_warnings': False,
        'user_agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
    }
    vd = []
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        try:
            print(f"Searching for '{q}'...")
            search_results = ydl.extract_info(f"ytsearch{limit}:{q}", download=False)
            if 'e' in search_results:
                for entry in search_results['e']:
                    if not entry:
                        continue  
                    video_id = entry['id']
                    video_url = f"https://www.youtube.com/watch?v={video_id}"                
                    print(f"\nProcessing: {entry['title']}") 
                    try:
                        ydl.download([video_url])
                        vd.append({
                            "filename": f"downloads/{video_id}.mp4",
                            "title": entry['title'].replace('"', "'"),
                            "author": entry['uploader'].replace('"', "'")
                        })
                        time.sleep(1)
                    except Exception as e:
                        print(f"SKIPPED {video_id} | error: {e}")
                        continue

        except Exception as e:
            print(f"Search failed: {e}")
    wmh(vd)
def wmh(vd):
    with open("metadata.h", "w", encoding="utf-8") as f:
        f.write("#ifndef METADATA_H\n#define METADATA_H\n\n")
        f.write("typedef struct { char *filename; char *title; char *author; } VideoSource;\n\n")
        f.write("static const VideoSource collection[] = {\n")   
        count = 0
        for vid in vd:
            base_path = vid["filename"].rsplit('.', 1)[0]
            actual_files = glob.glob(f"{base_path}.*") 
            if actual_files:
                actual_filename = actual_files[0].replace("\\", "/")
                f.write(f'    {{"{actual_filename}", "{vid["title"]}", "{vid["author"]}"}},\n')
                count += 1   
        f.write("};\n\n")
        f.write(f"#define COLLECTION_SIZE {count}\n")
        f.write("#endif\n")
    print(f"\nUpdated metadata.h with {count} files")
if __name__ == "__main__":
    down_ba()