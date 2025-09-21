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
struct Format {
  int rows;
  int columns;
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
void change_wallpaper(std::vector<Button>buttons, std::string &current_texture, int index){
        std::string unload = "hyprctl hyprpaper unload all";
        std::string preload = "hyprctl hyprpaper preload " + buttons[index].path;
        std::string wallpaper = "hyprctl hyprpaper wallpaper ," + buttons[index].path;
        std::string command = unload;
        system(command.c_str());
        command = preload;
        system(command.c_str());
        command = wallpaper;
        system(command.c_str());
        current_texture = buttons[index].path;
        save_wallpaper(buttons[index].path);
}

void draw_wallpapers(std::vector<Texture>wallpapers, std::vector<Button>&buttons, int rows){
  int index = 0;
  for (int i = 0; i < wallpapers.size(); i++){
    for (int j = 0; j < rows; j++){
      Rectangle source = {0,0,float(wallpapers[index].width),float(wallpapers[index].height)};
      buttons[index].destination = {float(j*96), float(i*96), 96, 96};

      DrawTexturePro(wallpapers[index], source, buttons[index].destination
                      ,{0,0}, 0.0, WHITE);
      index++;
    }
  }
  index = 0;
}
void input(Camera2D &camera, float speed){
  // TODO: Add mouse scroll
  if (IsKeyDown(KEY_DOWN)){
    camera.target.y += speed * GetFrameTime();
  }
  else if (IsKeyDown(KEY_UP)){
    camera.target.y -= speed * GetFrameTime();
  }

}

int main() {
  int rows = 5;
  int width = rows * 96;
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
// Camera for being able to scroll if you have more wallpapers
// than what can fit in the box.
  Camera2D camera;
  camera.offset = {0,0};//{float(width)/2, float(height)/2};
  camera.target = {0,0};//{float(width)/2, float(height)/2};
  camera.rotation = 0;
  camera.zoom = 1;

  float camera_speed = 100;
  float scroll_speed = 4;

  RenderTexture viewport = LoadRenderTexture(width, height);
  Rectangle screen_rec = {0,0,float(viewport.texture.width),-float(viewport.texture.height)};
  
  int index = 0;
  std::cout << "buttons loop" << std::endl;
  set_paths_to_buttons(buttons, files);

  index = 0;
  std::string current_texture;
  
  while (!WindowShouldClose()){
    // update
    camera.target.y -= GetMouseWheelMove() * scroll_speed;
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    for (int i = 0; i < buttons.size(); i++){
      if (CheckCollisionPointRec(mouse, buttons[i].destination)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        change_wallpaper(buttons, current_texture, i);
      }
    }
    if (IsKeyDown(KEY_DOWN)){
      camera.target.y += camera_speed * GetFrameTime();
    }
    else if (IsKeyDown(KEY_UP)){
      camera.target.y -= camera_speed * GetFrameTime();
    }

    BeginTextureMode(viewport);
    ClearBackground(BLACK);
    BeginMode2D(camera);

      draw_wallpapers(wallpapers, buttons, rows);
    EndMode2D();
    EndTextureMode();
    // draw
    BeginDrawing();
    ClearBackground(BLACK);
      DrawTextureRec(viewport.texture, screen_rec, {0,0}, WHITE);

    EndDrawing();
  }
  unload_textures(wallpapers);
  CloseWindow();
}
