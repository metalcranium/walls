#include <filesystem>
#include <iostream>
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
      
    }
    ~Button(){
    }
    void set_path(std::string wallpaper_path){
      path = wallpaper_path;
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

void unload_textures(std::vector<Texture> pictures){
  for (auto i : pictures){
    UnloadTexture(i);
  }
}


int main() {
  std::string path = "/home/blake/Pictures/wallpapers/";
  std::vector<std::string>files = get_files(path);
  std::vector<Button>buttons;
  std::vector<Texture>wallpapers;

  print_files(files);

  InitWindow(1200,800,"Walls");

  load_textures(wallpapers, files);
  load_buttons(buttons, files);
  int index = 0;
  Vector2 dest = {0,0};
  while (!WindowShouldClose()){
    // update
    for (int i = 0; i < buttons.size(); i++){
      if (CheckCollisionPointRec(GetMousePosition(), buttons[i].source)){
        std::cout << "button: " << i << std::endl;
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
