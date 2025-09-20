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


int main() {
  std::string path = "/home/blake/Pictures/wallpapers/";
  std::vector<std::string>files = get_files(path);
  std::vector<Button>buttons;
  std::vector<Texture>wallpapers;

  print_files(files);

  InitWindow(1200,800,"Walls");

  load_textures(wallpapers, files);
  load_buttons(buttons, files);
  std::cout << "buttons: " << buttons.size() << std::endl;
  
  int index = 0;
  std::cout << "buttons loop" << std::endl;
      for (int i = 0; i < buttons.size(); i++){
        for (int j = 0; j < 5; j++){
          buttons[index].position.x = j * 96;
          buttons[index].position.y = i * 96;
          buttons[index].source = {buttons[index].position.x, buttons[index].position.y,
              buttons[index].width, buttons[index].height};
          // buttons[index].print_position();
          index++;
         }
      }
  set_paths_to_buttons(buttons, files);

  index = 0;
  while (!WindowShouldClose()){
    // update
    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < buttons.size(); i++){
      if (CheckCollisionPointRec(mouse, buttons[i].source)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        std::cout << "button: " << i << std::endl;
        std::cout << "path: " << buttons[i].path << std::endl;
        std::string preload = "hyprctl hyprpaper preload ";
        std::string wallpaper = "hyprctl hyprpaper wallpaper ,";
        std::string command = preload + buttons[i].path;
        system(command.c_str());
        command = wallpaper + buttons[i].path;
        system(command.c_str());
      }
    }

    // draw
    BeginDrawing();
    ClearBackground(BLACK);
      for (int i = 0; i < wallpapers.size(); i++){
        for (int j = 0; j < 5; j++){
          // buttons[index].position = {float(j*96), float(i*96)};
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
