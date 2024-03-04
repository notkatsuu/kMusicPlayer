#include <Raylib.h>

#define RAYGUI_IMPLEMENTATION

#include "include/raygui.h"
#include "raymath.h"
#include "rlgl.h"

#define MAZE_WIDTH 64
#define MAZE_HEIGHT 64
#define MAZE_DRAW_SCALE 9
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

typedef struct Point {
  int x;
  int y;
} Point;

typedef struct Node {
  int x, y;
  int gScore, fScore, hScore;
  struct NodeA *parent;
} NodeA;

bool cameraIsFps = false;
bool cursorDisabled = false;
Point direction[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

BoundingBox playerBoundingBox = {0};
Point exitPoints[2] = {{(MAZE_WIDTH - 1) / 2, 0},
                       {(MAZE_WIDTH - 1) / 2, MAZE_HEIGHT - 1}};

void InitializeGame();
Image GenerateMaze(Point *mazePoints);
Camera InitOrbitCamera();
Camera InitFirstPersonCamera();
Model CreateModel(Mesh mesh);
void ApplyTextureToModel(const Model *model, Texture2D texture);
void DrawModelInGame(Model model, Vector3 position, float scale, Color color);
void Draw3D(const Image *imMaze, Model planeModel, Model cubeModel);
void Draw3DIntoTextureOrbital(RenderTexture2D target, const Image *imMaze,
                              Vector2 mazeOffset3D, Camera cam,
                              Model planeModel, Model cubeModel);
void Draw3DFirstPerson(const Image *imMaze, const Camera cam,
                       const Model planeModel, const Model cubeModel);
void EditMaze(Image *imMaze, Image *copy, Texture2D *texMaze, int mazeX,
              int mazeY);
Texture2D GenerateSquareTexture(int size, Color squareColor,
                                Color backgroundColor);
void CameraMovementFPS(Camera *firstPersonCamera, const Image *imMaze);
void ExportMaze(const Image *imMaze);
void DrawUI(Image *imMaze, Camera *firstPersonCamera, Camera *orbitalCam,
            bool *cameraIsFps);
void CursorToggle();
NodeA AStarPath(Image imMaze, Point start, Point goal);
void DrawPathOnImage(NodeA *path, Image *imMaze);

int main(void) {
  InitializeGame();

  bool prevCamIsFps = !cameraIsFps;
  bool dragWindow = false;
  bool exitWindow = false;
  Vector2 mousePosition = {0};
  Vector2 panOffset = mousePosition;
  Vector2 windowPosition = {500, 200};
  SetWindowPosition(windowPosition.x, windowPosition.y);
  //--------------------------------------------------------------------------------------

  double centerX = SCREEN_WIDTH / 2;
  double centerY = SCREEN_HEIGHT / 2;

  Point mazePoints[MAZE_WIDTH * MAZE_HEIGHT] = {0};
  static Image imMaze;
  static Image copy;

  imMaze = GenerateMaze(mazePoints);
  copy = ImageCopy(imMaze);

  NodeA path = {0};

  // int *LoadRandomSequence(0,0,mazePoints.size);

  Camera orbitalCam = InitOrbitCamera();
  Camera firstPersonCamera = InitFirstPersonCamera();

  Texture2D texMaze = LoadTextureFromImage(imMaze);
  RenderTexture2D targetTexture = LoadRenderTexture(
      MAZE_WIDTH * MAZE_DRAW_SCALE, MAZE_HEIGHT * MAZE_DRAW_SCALE);

  Texture2D whiteCubeTexture = GenerateSquareTexture(64, WHITE, GRAY);
  Texture2D blackFloorRoofTexture = GenerateSquareTexture(64, BLACK, DARKGRAY);

  Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  Model cubeModel = CreateModel(cubeMesh);

  Mesh planeMesh = GenMeshPlane(1.0f, 1.0f, 1, 1);
  Model planeModel = CreateModel(planeMesh);

  // Apply the texture to the model
  ApplyTextureToModel(&cubeModel, whiteCubeTexture);
  ApplyTextureToModel(&planeModel, blackFloorRoofTexture);

  // Main game loop
  while (!exitWindow && !WindowShouldClose()) {

    mousePosition = GetMousePosition();
    if (IsKeyPressed(KEY_SPACE))
      cameraIsFps = !cameraIsFps;

    if (cameraIsFps) {
      CameraMovementFPS(&firstPersonCamera, &imMaze);
    } else {
      UpdateCamera(&orbitalCam, CAMERA_ORBITAL);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !dragWindow) {
      if (CheckCollisionPointRec(mousePosition,
                                 (Rectangle){0, 0, SCREEN_WIDTH, 20})) {
        dragWindow = true;
        panOffset = mousePosition;
      }
    }

    if (dragWindow) {
      windowPosition.x += mousePosition.x - panOffset.x;
      windowPosition.y += mousePosition.y - panOffset.y;

      SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);

      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        dragWindow = false;
    }

    double posMaze2Dx = centerX / 2;
    double posMaze2Dy = centerY;
    Vector2 mazeOffset2D = {posMaze2Dx - texMaze.width / 2 * MAZE_DRAW_SCALE,
                            posMaze2Dy - texMaze.height / 2 * MAZE_DRAW_SCALE};

    int mazeX = (mousePosition.x - mazeOffset2D.x) /
                MAZE_DRAW_SCALE; // NOLINT(*-narrowing-conversions)
    int mazeY = (mousePosition.y - mazeOffset2D.y) / MAZE_DRAW_SCALE;

    double posMaze3Dx = SCREEN_WIDTH - posMaze2Dx;
    double posMaze3Dy = posMaze2Dy;
    Vector2 mazeOffset3D = {posMaze3Dx - texMaze.width / 2 * MAZE_DRAW_SCALE,
                            posMaze3Dy - texMaze.height / 2 * MAZE_DRAW_SCALE};

    if (IsKeyPressed(KEY_S)) {
      ExportMaze(&imMaze);
    }

    if (IsKeyPressed(KEY_F)) {

      path = AStarPath(imMaze, exitPoints[0], exitPoints[1]);

      if (path.x != 0 && path.y != 0) {
        printf("Path found\n");

      } else {
        printf("Path not found\n");
      }
    }

    if (IsKeyPressed(KEY_R)) {

      imMaze = GenerateMaze(mazePoints);
      UnloadTexture(texMaze);
      texMaze = LoadTextureFromImage(imMaze);
    }

    if (IsKeyPressed(KEY_P)) {

      imMaze = ImageCopy(copy);
      path = AStarPath(imMaze, exitPoints[0], exitPoints[1]);
      DrawPathOnImage(&path, &imMaze);
      UnloadTexture(texMaze);
      texMaze = LoadTextureFromImage(imMaze);
    }

    if (IsKeyPressed(KEY_C)) {
      copy = ImageCopy(imMaze);
      UnloadTexture(texMaze);
      texMaze = LoadTextureFromImage(imMaze);
    }

    if (IsKeyPressed(KEY_Z)) {
      imMaze = ImageCopy(copy);
      UnloadTexture(texMaze);
      texMaze = LoadTextureFromImage(imMaze);
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    exitWindow = GuiWindowBox((Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
                              "#2# Practica 4 - Raylib Maze 3D");
    ClearBackground(GRAY);
    EditMaze(&imMaze, &copy, &texMaze, mazeX, mazeY);
    BeginMode3D(orbitalCam);
    if (!cameraIsFps) {
      Draw3DIntoTextureOrbital(targetTexture, &imMaze, mazeOffset3D, orbitalCam,
                               planeModel, cubeModel);
      DrawTextureEx(texMaze, mazeOffset2D, 0, MAZE_DRAW_SCALE, WHITE);
    } else {
      Draw3DFirstPerson(&imMaze, firstPersonCamera, planeModel, cubeModel);
    }
    EndMode3D();
    DrawText(TextFormat("FPS: %i", GetFPS()), SCREEN_WIDTH - 80,
             SCREEN_HEIGHT - 45, 20, RED);

    DrawUI(&imMaze, &firstPersonCamera, &orbitalCam, &cameraIsFps);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }
  UnloadTexture(texMaze);
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------
  return 0;
}

void InitializeGame() {
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Practica 4 - Raylib Maze 3D");
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(DARKGRAY));
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(GRAY));
  SetTargetFPS(144); // Set our game to run at 60 frames-per-second
}

Image GenerateMaze(Point *mazePoints) {
  // Draw the border of the maze
  int mazePointCounter = 0;
  Image imMaze = GenImageColor(MAZE_WIDTH, MAZE_HEIGHT, BLACK);
  for (int x = 0; x < imMaze.width; x++) {
    for (int y = 0; y < imMaze.height; y++) {
      if (x == 0 || x == imMaze.width - 1 || y == 0 || y == imMaze.height - 1) {
        // Create exit points at opposite corners
        if ((x == exitPoints[0].x && y == exitPoints[0].y) ||
            (x == exitPoints[1].x && y == exitPoints[1].y)) {
          continue;
        }
        ImageDrawPixel(&imMaze, x, y, WHITE);
      } else if (x % 3 == 0 && y % 3 == 0) {
        if (GetRandomValue(0, 3) > 2) {
          ImageDrawPixel(&imMaze, x, y, WHITE);
          mazePoints[mazePointCounter] = (Point){x, y};
          mazePointCounter++;
        }
      }
    }
  }

  // Randomly select points and draw lines between them
  for (int i = 0; i < mazePointCounter; i++) {
    Point currentPoint = mazePoints[i];
    const Point currentDir = direction[GetRandomValue(0, 3)];
    currentPoint.x += currentDir.x;
    currentPoint.y += currentDir.y;

    while (GetImageColor(imMaze, currentPoint.x, currentPoint.y).r != 255) {
      ImageDrawPixel(&imMaze, currentPoint.x, currentPoint.y, WHITE);
      currentPoint.x += currentDir.x;
      currentPoint.y += currentDir.y;
    }
  }

  return imMaze;
}

Camera InitOrbitCamera() {
  Camera camera = {0};
  camera.position = (Vector3){70.0f, 70, 70.0f}; // Camera position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};   // Camera looking at point
  camera.fovy = 45.0f;                           // Camera field-of-view Y
  camera.up =
      (Vector3){0.0f, 2.0f, 0.0f}; // Camera up vector (rotation towards target)

  camera.projection = CAMERA_PERSPECTIVE;

  return camera;
}

Camera InitFirstPersonCamera() {
  Camera camera = {0};
  camera.position = (Vector3){0, 0.5f, 4};     // Camera position
  camera.target = (Vector3){0.0f, 2.0f, 0.0f}; // Camera looking at point
  camera.up =
      (Vector3){0.0f, 1.0f, 0.0f}; // Camera up vector (rotation towards target)
  camera.fovy = 90.0f;             // Camera field-of-view Y

  camera.projection = CAMERA_PERSPECTIVE;

  // Initialize player bounding box
  playerBoundingBox.min =
      (Vector3){camera.position.x - 0.25f, 0.0f, camera.position.z - 0.25f};
  playerBoundingBox.max =
      (Vector3){camera.position.x + 0.25f, 2.0f, camera.position.z + 0.25f};

  return camera;
}

// Function to generate procedural textures

// Function to create a 3D model
// Function to create a 3D model with error checking
Model CreateModel(const Mesh mesh) {
  if (!mesh.vboId) {
    printf("Error: Failed to create model. Invalid mesh.\n");
    return (Model){0};
  }

  // Load the mesh into a model
  const Model model = LoadModelFromMesh(mesh);

  return model;
}

// Function to apply the texture to the model with error checking
void ApplyTextureToModel(const Model *model, const Texture2D texture) {
  if (!texture.id) {
    printf("Error: Failed to apply texture. Invalid texture.\n");
    return;
  }

  // Apply the texture to the model
  model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

// Function to draw the model with more customization options
void DrawModelInGame(const Model model, const Vector3 position,
                     const float scale, const Color color) {
  // Draw the model
  DrawModel(model, position, scale, color);
}

void Draw3D(const Image *imMaze, const Model planeModel,
            const Model cubeModel) {
  // Draw black pixels as walls and white pixels as paths
  for (int y = 0; y < imMaze->height; y++) {
    for (int x = 0; x < imMaze->width; x++) {
      DrawPlane((Vector3){x - imMaze->width / 2, 0.0f, y - imMaze->height / 2},
                (Vector2){1.0f, -1.0f}, DARKGRAY);
      DrawModelInGame(
          planeModel,
          (Vector3){x - imMaze->width / 2, 0.0f, y - imMaze->height / 2}, 1.0f,
          DARKGRAY);

      if (GetImageColor(*imMaze, x, y).r == 255) {
        DrawModelInGame(
            cubeModel,
            (Vector3){x - imMaze->width / 2, 0.5f, y - imMaze->height / 2},
            1.0f, WHITE);
      } else if (GetImageColor(*imMaze, x, y).r == 0 &&
                 GetImageColor(*imMaze, x, y).g == 255 &&
                 GetImageColor(*imMaze, x, y).b == 0) {
        DrawCube((Vector3){x - imMaze->width / 2, 3.0f, y - imMaze->height / 2},
                 1.0f, 5.0f, 1.0f, Fade(GREEN, 0.5f));
      }
    }
  }
}

void Draw3DIntoTextureOrbital(const RenderTexture2D target, const Image *imMaze,
                              const Vector2 mazeOffset3D, const Camera cam,
                              const Model planeModel, const Model cubeModel) {
  BeginTextureMode(target);
  ClearBackground(LIGHTGRAY);
  BeginMode3D(cam);
  Draw3D(imMaze, planeModel, cubeModel);
  EndMode3D();
  EndTextureMode();
  DrawTexturePro(
      target.texture,
      (Rectangle){0, 0, target.texture.width, -target.texture.height},
      (Rectangle){mazeOffset3D.x, mazeOffset3D.y, MAZE_WIDTH * MAZE_DRAW_SCALE,
                  MAZE_HEIGHT * MAZE_DRAW_SCALE},
      (Vector2){0, 0}, 0, WHITE);
}

void Draw3DFirstPerson(const Image *imMaze, const Camera cam,
                       const Model planeModel, const Model cubeModel) {

  ClearBackground(LIGHTGRAY);
  BeginMode3D(cam);
  Draw3D(imMaze, planeModel, cubeModel);
  EndMode3D();
}

void EditMaze(Image *imMaze, Image *copy, Texture2D *texMaze, const int mazeX,
              const int mazeY) {

  static Color prevColor = {0};
  static int prevX = -1, prevY = -1;
  if (mazeX >= 0 && mazeX < texMaze->width && mazeY >= 0 &&
      mazeY < texMaze->height && !cameraIsFps) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      ImageDrawPixel(imMaze, mazeX, mazeY, WHITE);
      prevColor = WHITE;
      UnloadTexture(*texMaze);
      *texMaze = LoadTextureFromImage(*imMaze);
    } else if (prevX != mazeX || prevY != mazeY) {
      if (prevX != -1 && prevY != -1)
        ImageDrawPixel(imMaze, prevX, prevY, prevColor);

      prevColor = GetImageColor(*imMaze, mazeX, mazeY);
      prevX = mazeX;
      prevY = mazeY;
      ImageDrawPixel(imMaze, mazeX, mazeY, (Color){0, 255, 0, 255});

      UnloadTexture(*texMaze);
      *texMaze = LoadTextureFromImage(*imMaze);
    }
  } else if (cameraIsFps) {
    ImageDrawPixel(imMaze, prevX, prevY, prevColor);
  }
}

Texture2D GenerateSquareTexture(const int size, const Color squareColor,
                                const Color backgroundColor) {
  // Create an image
  Image image = GenImageColor(size, size, backgroundColor);

  // Draw a square in the center of the image
  const int squareSize = size / 2;
  const int squareOffset = (size - squareSize) / 2;
  ImageDrawRectangle(&image, squareOffset, squareOffset, squareSize, squareSize,
                     squareColor);

  // Load the image into a texture
  const Texture2D texture = LoadTextureFromImage(image);

  // Unload the image from memory
  UnloadImage(image);

  return texture;
}

void CameraMovementFPS(Camera *firstPersonCamera, const Image *imMaze) {
  // Update player bounding box
  Vector3 oldPosition = firstPersonCamera->position;
  UpdateCamera(firstPersonCamera, CAMERA_FIRST_PERSON);
  Vector3 deltaPosition =
      Vector3Subtract(firstPersonCamera->position, oldPosition);
  playerBoundingBox.min = Vector3Add(playerBoundingBox.min, deltaPosition);
  playerBoundingBox.max = Vector3Add(playerBoundingBox.max, deltaPosition);

  // Check for collisions with walls
  for (int y = 0; y < imMaze->height; y++) {
    for (int x = 0; x < imMaze->width; x++) {
      if (GetImageColor(*imMaze, x, y).r == 255) {
        BoundingBox wallBoundingBox;
        wallBoundingBox.min = (Vector3){x - imMaze->width / 2 - 0.4f, 0.0f,
                                        y - imMaze->height / 2 - 0.4f};
        wallBoundingBox.max = (Vector3){x - imMaze->width / 2 + 0.4f, 2.0f,
                                        y - imMaze->height / 2 + 0.4f};

        if (CheckCollisionBoxes(playerBoundingBox, wallBoundingBox)) {
          // Collision detected, move player back
          firstPersonCamera->position = oldPosition;
          playerBoundingBox.min =
              Vector3Subtract(playerBoundingBox.min, deltaPosition);
          playerBoundingBox.max =
              Vector3Subtract(playerBoundingBox.max, deltaPosition);
          break;
        }
      }
    }
  }
}

void ExportMaze(const Image *imMaze) {
  ExportImage(*imMaze, "maze.png");

  Mesh mesh = GenMeshCubicmap(*imMaze, (Vector3){1.0f, 1.0f, 1.0f});
  ExportMesh(mesh, ".../exports/maze.obj");
}

void DrawUI(Image *imMaze, Camera *firstPersonCamera, Camera *orbitalCam,
            bool *cameraIsFps) {
  // Draw a button for changing the view
  if (GuiButton((Rectangle){10, 10, 150, 30}, "Change View")) {
    *cameraIsFps = !*cameraIsFps;
    CursorToggle();
  }

  // Draw a button for exporting the maze
  if (GuiButton((Rectangle){10, 50, 150, 30}, "Export Maze")) {
    ExportMaze(imMaze);
  }
}

void CursorToggle() {
  if (cameraIsFps && !cursorDisabled) {
    DisableCursor();
    cursorDisabled = true;
  } else if (!cameraIsFps && cursorDisabled) {
    EnableCursor();
    cursorDisabled = false;
  }
}

NodeA *createNode(int x, int y) {
  NodeA *newNode = (NodeA *)malloc(sizeof(NodeA));
  newNode->x = x;
  newNode->y = y;
  newNode->gScore = 1e9;
  newNode->fScore = 1e9;
  newNode->parent = NULL;
  return newNode;
}

int heuristic(NodeA *a, NodeA *b) {
  // Use Manhattan distance as heuristic
  return abs(a->x - b->x) + abs(a->y - b->y);
}

NodeA AStarPath(Image imMaze, Point start, Point goal) {
  NodeA *startNode = createNode(start.x, start.y);
  NodeA *goalNode = createNode(goal.x, goal.y);
  startNode->gScore = 0;
  startNode->fScore = heuristic(startNode, goalNode);
  NodeA *openSet[MAZE_WIDTH * MAZE_HEIGHT];
  NodeA *closedSet[MAZE_WIDTH * MAZE_HEIGHT];
  int openSetSize = 0;
  int closedSetSize = 0;
  openSet[openSetSize++] = startNode;

  while (openSetSize > 0) {
    NodeA *current = openSet[0];
    int currentIdx = 0;
    for (int i = 1; i < openSetSize; i++) {
      if (openSet[i]->fScore < current->fScore) {
        current = openSet[i];
        currentIdx = i;
      }
    }

    if (current->x == goalNode->x && current->y == goalNode->y) {

      // copy current to solution
      NodeA solution = *current;

      return solution;
    }

    openSet[currentIdx] = openSet[openSetSize - 1];
    openSetSize--;

    closedSet[closedSetSize++] = current;

    for (int i = 0; i < 4; i++) {
      int x = current->x + direction[i].x;
      int y = current->y + direction[i].y;

      if (x < 0 || x >= MAZE_WIDTH || y < 0 || y >= MAZE_HEIGHT) {
        continue;
      }

      if (GetImageColor(imMaze, x, y).r == 255) {
        continue;
      }

      NodeA *neighbor = createNode(x, y);

      bool inClosedSet = false;
      for (int j = 0; j < closedSetSize; j++) {
        if (closedSet[j]->x == neighbor->x && closedSet[j]->y == neighbor->y) {
          inClosedSet = true;
          break;
        }
      }

      if (inClosedSet) {
        continue;
      }

      int tentativeGScore = current->gScore + 1;
      bool inOpenSet = false;
      for (int j = 0; j < openSetSize; j++) {
        if (openSet[j]->x == neighbor->x && openSet[j]->y == neighbor->y) {
          inOpenSet = true;
          break;
        }
      }
      if (!inOpenSet || tentativeGScore < neighbor->gScore) {
        neighbor->parent = current;
        neighbor->gScore = tentativeGScore;
        neighbor->fScore = neighbor->gScore + heuristic(neighbor, goalNode);
        if (!inOpenSet) {
          openSet[openSetSize++] = neighbor;
        }
      }
    }
  }

  return (NodeA){0};
}

void DrawPathOnImage(NodeA *path, Image *imMaze) {
  NodeA *currentNode = path;
  while (currentNode != NULL) {
    ImageDrawPixel(imMaze, currentNode->x, currentNode->y, RED);
    currentNode = currentNode->parent;
  }
}