#include "headset/headset.h"
#include "graphics/graphics.h"
#include <math/mat4.h>
#include <math/quat.h>
#include <emscripten.h>
#include <emscripten/vr.h>

typedef struct {
  headsetRenderCallback renderCallback;
} HeadsetState;

static HeadsetState state;

static void onRequestAnimationFrame(void* userdata) {
  lovrGraphicsClear(1, 1);

  int width = emscripten_vr_get_display_width();
  int height = emscripten_vr_get_display_height();

  float projection[16];
  float transform[16];
  float sittingToStanding[16];

  mat4_set(sittingToStanding, emscripten_vr_get_sitting_to_standing_matrix());
  mat4_invert(sittingToStanding);

  for (HeadsetEye eye = EYE_LEFT; eye <= EYE_RIGHT; eye++) {
    int isRight = eye == EYE_RIGHT;

    mat4_set(projection, emscripten_vr_get_projection_matrix(isRight));
    mat4_set(transform, emscripten_vr_get_view_matrix(isRight));
    mat4_multiply(transform, sittingToStanding);

    lovrGraphicsPush();
    lovrGraphicsOrigin();
    lovrGraphicsMatrixTransform(transform);
    lovrGraphicsSetProjection(projection);

    if (isRight) {
      lovrGraphicsSetViewport(width / 2, 0, width / 2, height);
    } else {
      lovrGraphicsSetViewport(0, 0, width / 2, height);
    }

    state.renderCallback(eye, userdata);
    lovrGraphicsPop();
  }
}

void lovrHeadsetInit() {
  emscripten_vr_init();
}

void lovrHeadsetDestroy() {
  //
}

void lovrHeadsetPoll() {
  //
}

int lovrHeadsetIsPresent() {
  return emscripten_vr_is_present();
}

const char* lovrHeadsetGetType() {
  return emscripten_vr_get_display_name();
}

int lovrHeadsetIsMirrored() {
  return 0;
}

void lovrHeadsetSetMirrored(int mirror) {
  //
}

void lovrHeadsetGetDisplayDimensions(int* width, int* height) {
  *width = emscripten_vr_get_display_width() / 2;
  *height = emscripten_vr_get_display_height();
}

void lovrHeadsetGetClipDistance(float* near, float* far) {
  emscripten_vr_get_display_clip_distance(near, far);
}

void lovrHeadsetSetClipDistance(float near, float far) {
  emscripten_vr_set_display_clip_distance(near, far);
}

float lovrHeadsetGetBoundsWidth() {
  return emscripten_vr_get_bounds_width();
}

float lovrHeadsetGetBoundsDepth() {
  return emscripten_vr_get_bounds_depth();
}

void lovrHeadsetGetBoundsGeometry(float* geometry) {
  // Not supported
}

char lovrHeadsetIsBoundsVisible() {
  return 0; // Not supported
}

void lovrHeadsetSetBoundsVisible(char visible) {
  // Not supported
}

void lovrHeadsetGetPosition(float* x, float* y, float* z) {
  float v[3];
  emscripten_vr_get_position(&v[0], &v[1], &v[2]);
  mat4_transform(emscripten_vr_get_sitting_to_standing_matrix(), v);
  *x = v[0];
  *y = v[1];
  *z = v[2];
}

void lovrHeadsetGetEyePosition(HeadsetEye eye, float* x, float* y, float* z) {
  int i = eye == EYE_LEFT ? 0 : 1;
  emscripten_vr_get_eye_offset(i, x, y, z);
  float m[16];
  mat4_multiply(mat4_identity(m), emscripten_vr_get_sitting_to_standing_matrix(i));
  mat4_multiply(m, mat4_invert(emscripten_vr_get_view_matrix(i)));
  mat4_translate(m, *x, *y, *z);
  *x = m[12];
  *y = m[13];
  *z = m[14];
}

void lovrHeadsetGetOrientation(float* angle, float* x, float* y, float* z) {
  float quat[4];
  float m[16];
  emscripten_vr_get_orientation(&quat[0], &quat[1], &quat[2], &quat[3]);
  mat4_multiply(mat4_identity(m), emscripten_vr_get_sitting_to_standing_matrix());
  mat4_rotateQuat(m, quat);
  quat_getAngleAxis(quat_fromMat4(quat, m), angle, x, y, z);
}

void lovrHeadsetGetVelocity(float* x, float* y, float* z) {
  float v[3];
  emscripten_vr_get_velocity(&v[0], &v[1], &v[2]);
  mat4_transformDirection(emscripten_vr_get_sitting_to_standing_matrix(), v);
  *x = v[0];
  *y = v[1];
  *z = v[2];
}

void lovrHeadsetGetAngularVelocity(float* x, float* y, float* z) {
  float v[3];
  emscripten_vr_get_angular_velocity(&v[0], &v[1], &v[2]);
  mat4_transformDirection(emscripten_vr_get_sitting_to_standing_matrix(), v);
  *x = v[0];
  *y = v[1];
  *z = v[2];
}

vec_controller_t* lovrHeadsetGetControllers() {
  return NULL;
}

int lovrHeadsetControllerIsPresent(Controller* controller) {
  return 0;
}

void lovrHeadsetControllerGetPosition(Controller* controller, float* x, float* y, float* z) {
  *x = *y = *z = 0;
}

void lovrHeadsetControllerGetOrientation(Controller* controller, float* angle, float* x, float* y, float* z) {
  *angle = *x = *y = *z = 0;
}

float lovrHeadsetControllerGetAxis(Controller* controller, ControllerAxis axis) {
  return 0;
}

int lovrHeadsetControllerIsDown(Controller* controller, ControllerButton button) {
  return 0;
}

void lovrHeadsetControllerVibrate(Controller* controller, float duration) {
  //
}

ModelData* lovrHeadsetControllerNewModelData(Controller* controller) {
  return NULL;
}

TextureData* lovrHeadsetControllerNewTextureData(Controller* controller) {
  return NULL;
}

void lovrHeadsetRenderTo(headsetRenderCallback callback, void* userdata) {
  state.renderCallback = callback;
  emscripten_vr_set_render_callback(onRequestAnimationFrame, userdata);
}
