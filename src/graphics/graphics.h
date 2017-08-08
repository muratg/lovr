#include "graphics/font.h"
#include "graphics/mesh.h"
#include "graphics/model.h"
#include "graphics/shader.h"
#include "graphics/skybox.h"
#include "graphics/texture.h"
#include "math/math.h"
#include "lib/glfw.h"

#pragma once

#define MAX_CANVASES 4
#define MAX_TRANSFORMS 60
#define INTERNAL_TRANSFORMS 4

typedef enum {
  BLEND_ALPHA,
  BLEND_ADD,
  BLEND_SUBTRACT,
  BLEND_MULTIPLY,
  BLEND_LIGHTEN,
  BLEND_DARKEN,
  BLEND_SCREEN,
  BLEND_REPLACE
} BlendMode;

typedef enum {
  BLEND_ALPHA_MULTIPLY,
  BLEND_PREMULTIPLIED
} BlendAlphaMode;

typedef enum {
  DRAW_MODE_FILL,
  DRAW_MODE_LINE
} DrawMode;

typedef enum {
  WINDING_CLOCKWISE = GL_CW,
  WINDING_COUNTERCLOCKWISE = GL_CCW
} Winding;

typedef enum {
  COMPARE_NONE = 0,
  COMPARE_EQUAL = GL_EQUAL,
  COMPARE_NOT_EQUAL = GL_NOTEQUAL,
  COMPARE_LESS = GL_LESS,
  COMPARE_LEQUAL = GL_LEQUAL,
  COMPARE_GEQUAL = GL_GEQUAL,
  COMPARE_GREATER = GL_GREATER
} CompareMode;

typedef struct {
  int initialized;
  float pointSizes[2];
  int textureSize;
  int textureMSAA;
  float textureAnisotropy;
} GraphicsLimits;

typedef struct {
  int framebuffer;
  float projection[16];
  int viewport[4];
} CanvasState;

typedef struct {
  GLFWwindow* window;
  Shader* defaultShader;
  Shader* skyboxShader;
  Shader* fontShader;
  Shader* fullscreenShader;
  Font* defaultFont;
  Texture* activeTexture;
  Texture* defaultTexture;
  float transforms[MAX_TRANSFORMS + INTERNAL_TRANSFORMS][16];
  CanvasState canvases[MAX_CANVASES];
  int transform;
  int canvas;

  struct {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    vec_float_t data;
    vec_uint_t indices;
  } buffer;

  Color backgroundColor;
  BlendMode blendMode;
  BlendAlphaMode blendAlphaMode;
  Color color;
  int culling;
  TextureFilter defaultFilter;
  CompareMode depthTest;
  Font* font;
  GraphicsLimits limits;
  float lineWidth;
  float pointSize;
  Shader* shader;
  Winding winding;
  int wireframe;
} GraphicsState;

// Base
void lovrGraphicsInit();
void lovrGraphicsDestroy();
void lovrGraphicsReset();
void lovrGraphicsClear(int color, int depth);
void lovrGraphicsPresent();
void lovrGraphicsPrepare();

// State
Color lovrGraphicsGetBackgroundColor();
void lovrGraphicsSetBackgroundColor(Color color);
void lovrGraphicsGetBlendMode(BlendMode* mode, BlendAlphaMode* alphaMode);
void lovrGraphicsSetBlendMode(BlendMode mode, BlendAlphaMode alphaMode);
Color lovrGraphicsGetColor();
void lovrGraphicsSetColor(Color color);
int lovrGraphicsIsCullingEnabled();
void lovrGraphicsSetCullingEnabled(int culling);
TextureFilter lovrGraphicsGetDefaultFilter();
void lovrGraphicsSetDefaultFilter(TextureFilter filter);
CompareMode lovrGraphicsGetDepthTest();
void lovrGraphicsSetDepthTest(CompareMode depthTest);
Font* lovrGraphicsGetFont();
void lovrGraphicsSetFont(Font* font);
GraphicsLimits lovrGraphicsGetLimits();
float lovrGraphicsGetLineWidth();
void lovrGraphicsSetLineWidth(float width);
float lovrGraphicsGetPointSize();
void lovrGraphicsSetPointSize(float size);
Shader* lovrGraphicsGetShader();
void lovrGraphicsSetShader(Shader* shader);
Winding lovrGraphicsGetWinding();
void lovrGraphicsSetWinding(Winding winding);
int lovrGraphicsIsWireframe();
void lovrGraphicsSetWireframe(int wireframe);
Texture* lovrGraphicsGetTexture();
void lovrGraphicsBindTexture(Texture* texture);
mat4 lovrGraphicsGetProjection();
void lovrGraphicsSetProjection(mat4 projection);
int lovrGraphicsGetWidth();
int lovrGraphicsGetHeight();
void lovrGraphicsPushCanvas();
void lovrGraphicsPopCanvas();
void lovrGraphicsSetViewport(int x, int y, int w, int h);
void lovrGraphicsBindFramebuffer(int framebuffer);

// Transforms
int lovrGraphicsPush();
int lovrGraphicsPop();
void lovrGraphicsOrigin();
void lovrGraphicsTranslate(float x, float y, float z);
void lovrGraphicsRotate(float angle, float ax, float ay, float az);
void lovrGraphicsScale(float x, float y, float z);
void lovrGraphicsMatrixTransform(mat4 transform);

// Primitives
void lovrGraphicsPoints(float* points, int count);
void lovrGraphicsLine(float* points, int count);
void lovrGraphicsTriangle(DrawMode mode, float* points);
void lovrGraphicsPlane(DrawMode mode, Texture* texture, mat4 transform);
void lovrGraphicsPlaneFullscreen(Texture* texture);
void lovrGraphicsBox(DrawMode mode, Texture* texture, mat4 transform);
void lovrGraphicsCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2, int capped, int segments);
void lovrGraphicsSphere(Texture* texture, mat4 transform, int segments);
void lovrGraphicsSkybox(Skybox* skybox, float angle, float ax, float ay, float az);
void lovrGraphicsPrint(const char* str, mat4 transform, float wrap, HorizontalAlign halign, VerticalAlign valign);
