#include <filesystem>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include "raylib.h"

class Button {
  public:
    float width;
    float height;
    std::string path;
    Vector2 position;
    Rectangle source;
    Rectangle destination;

    Button(){
      width = 96;
      height = 96;
      position = {0,0};
      source = {position.x, position.y, width, height};
      
    }
    ~Button(){
    }
    void set_path(std::string wallpaper_path){
      path = wallpaper_path;
    }
    void print_position(){
      std::cout << position.x << "," << position.y << std::endl;
    }
};

std::vector<std::string>get_files(std::string path){
  std::vector<std::string>files;
  for (auto i : std::filesystem::directory_iterator(path)){
    files.push_back(i.path());
  }
  return files;
}

template <typename T>
void print_files(std::vector<T> files){
  for (auto i : files){
    std::cout << i << std::endl;
  }
}

void load_textures(std::vector<Texture>&wallpapers,std::vector<std::string>files){
  int count = 0;
  for (auto i : files){
    Texture texture = LoadTexture(i.c_str());
    wallpapers.push_back(texture);
    count++;
    std::cout << count << " images loaded." << std::endl;
  }
}

void load_buttons(std::vector<Button>&buttons,std::vector<std::string>files){
  int count = 0;
  for (auto i : files){
    Button button;
    buttons.push_back(button);
    count++;
    std::cout << count << " buttons loaded." << std::endl;
  }
}

void set_paths_to_buttons(std::vector<Button>&buttons,std::vector<std::string>files){
  for (int i = 0; i < buttons.size(); i++){
    buttons[i].set_path(files[i]);
  }
}

void unload_textures(std::vector<Texture> pictures){
  for (auto i : pictures){
    UnloadTexture(i);
  }
}

void save_wallpaper(std::string path){
  std::ofstream file;
  std::string first = "preload = " + path;
  std::string second = "wallpaper = ," + path;
  file.open("/home/blake/.config/hypr/hyprpaper.conf");
  file << first;
  file << "\n";
  file << second;
  file.close();
}

std::string get_config(){
  std::string path;
  std::ifstream file;
  file.open("/home/blake/.config/walls/config");
  getline(file, path);
  file.close();
  return path;
   
}

int main() {
  int width = 5 * 96;
  int height = 800;
  std::string path = get_config();// "/home/blake/Pictures/wallpapers/";
  std::vector<std::string>files = get_files(path);
  std::vector<Button>buttons;
  std::vector<Texture>wallpapers;
 
  print_files(files);

  InitWindow(width,height,"Walls");

  load_textures(wallpapers, files);
  load_buttons(buttons, files);
  std::cout << "buttons: " << buttons.size() << std::endl;

  RenderTexture viewport = LoadRenderTexture(width, height);
  Rectangle screen_rec = {0,0,float(viewport.texture.width),-float(viewport.texture.width)};
  
  int index = 0;
  std::cout << "buttons loop" << std::endl;
      for (int i = 0; i < buttons.size(); i++){
        for (int j = 0; j < 5; j++){
          buttons[index].position.x = j * 96;
          buttons[index].position.y = i * 96;
          buttons[index].source = {buttons[index].position.x, buttons[index].position.y,
              buttons[index].width, buttons[index].height};
          index++;
         }
      }
  set_paths_to_buttons(buttons, files);

  index = 0;
  std::string current_texture;
  
  while (!WindowShouldClose()){
    // update
    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < buttons.size(); i++){
      if (CheckCollisionPointRec(mouse, buttons[i].source)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        std::string unload = "hyprctl hyprpaper unload all";
        std::string preload = "hyprctl hyprpaper preload " + buttons[i].path;
        std::string wallpaper = "hyprctl hyprpaper wallpaper ," + buttons[i].path;
        std::string command = unload;
        system(command.c_str());
        command = preload;
        system(command.c_str());
        command = wallpaper;
        system(command.c_str());
        current_texture = buttons[i].path;
        save_wallpaper(buttons[i].path);
      }
    }
    // draw
    BeginDrawing();
    ClearBackground(BLACK);
      for (int i = 0; i < wallpapers.size(); i++){
        for (int j = 0; j < 5; j++){
          Rectangle source = {0,0,float(wallpapers[index].width),float(wallpapers[index].height)};
          buttons[index].destination = {float(j*96), float(i*96), 96, 96};
          DrawTexturePro(wallpapers[index], source, buttons[index].destination
                          ,{0,0}, 0.0, WHITE);
          index++;
        }
      }
      index = 0;

    EndDrawing();
  }
  unload_textures(wallpapers);
  CloseWindow();
}
