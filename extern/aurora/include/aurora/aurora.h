#ifndef AURORA_AURORA_H
#define AURORA_AURORA_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>

extern "C" {
#else
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#endif

typedef enum {
  SAMPLER_BILINEAR,
  SAMPLER_AREA,
} AuroraSampler;

typedef enum {
  BACKEND_AUTO,
  BACKEND_D3D11,
  BACKEND_D3D12,
  BACKEND_METAL,
  BACKEND_VULKAN,
  BACKEND_OPENGL,
  BACKEND_OPENGLES,
  BACKEND_WEBGPU,
  BACKEND_NULL,
} AuroraBackend;

typedef enum {
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_FATAL,
} AuroraLogLevel;

typedef struct {
  int32_t x;
  int32_t y;
} AuroraWindowPos;

typedef struct {
  uint32_t width;
  uint32_t height;

  /**
   * Width of the main GX framebuffer.
   */
  uint32_t fb_width;

  /**
   * Height of the main GX framebuffer.
   */
  uint32_t fb_height;

  /**
   * The size of the framebuffer used to present to the operating system.
   * May differ from fb_width if Aurora is instructed to force an aspect ratio or resolution configuration.
   */
  uint32_t native_fb_width;

  /**
   * The size of the framebuffer used to present to the operating system.
   * May differ from fb_height if Aurora is instructed to force an aspect ratio or resolution configuration.
   */
  uint32_t native_fb_height;
  float scale;
} AuroraWindowSize;

typedef struct SDL_Window SDL_Window;
typedef struct AuroraEvent AuroraEvent;

typedef void (*AuroraLogCallback)(AuroraLogLevel level, const char* module, const char* message, unsigned int len);
typedef void (*AuroraImGuiInitCallback)(const AuroraWindowSize* size);

#define MEM1_DEFAULT_SIZE (24 * 1024 * 1024)
#define ARAM_DEFAULT_SIZE (16 * 1024 * 1024)

typedef struct {
  const char* appName;
  const char* userPath;
  const char* cachePath;
  const char* resourcesPath;
  AuroraBackend desiredBackend;
  uint32_t msaa;
  uint16_t maxTextureAnisotropy;
  bool vsync;
  bool startFullscreen;
  bool allowJoystickBackgroundEvents;
  bool pauseOnFocusLost;
  bool allowTextureDumps;
  bool allowCpuAdapter;
  int32_t windowPosX;
  int32_t windowPosY;
  uint32_t windowWidth;
  uint32_t windowHeight;
  void* iconRGBA8;
  uint32_t iconWidth;
  uint32_t iconHeight;
  AuroraLogCallback logCallback;
  AuroraLogLevel logLevel;
  AuroraImGuiInitCallback imGuiInitCallback;

  /*
   * The size of the GameCube's main memory, or MEM1 on the Wii.
   * Note that it will not be allocated at the exact 0x80000000 address, as that cannot be guaranteed.
   * This can be set to 0 to disable allocating this region.
   */
  uint32_t mem1Size;

  /*
   * The size of the GameCube's ARAM, or MEM2 on the Wii.
   * This can be set to 0 to disable allocating this region.
   */
  uint32_t mem2Size;

  float frameBufferScale;
  uint32_t residentGeometryMiB;
  const void* pipelineCacheSeedData;
  size_t pipelineCacheSeedSize;
  const char* vulkanLibraryDir;
} AuroraConfig;

typedef struct {
  uint32_t count[2];
  uint64_t bytes[2];
} AuroraTextureStats;

typedef struct {
  const char* name;
  float msPerFrame;
  float passesPerFrame;
} GXPortGpuTime;

typedef struct {
  AuroraBackend backend;
  const char* userPath;
  const char* cachePath;
  SDL_Window* window;
  AuroraWindowSize windowSize;
} AuroraInfo;

AuroraInfo aurora_initialize(int argc, char* argv[], const AuroraConfig* config);
void aurora_shutdown();
const AuroraEvent* aurora_update();
bool aurora_begin_frame();
void aurora_end_frame();

void aurora_set_log_level(AuroraLogLevel level);
void aurora_set_pause_on_focus_lost(bool value);
void aurora_set_background_input(bool value);
void aurora_set_resampler(AuroraSampler sampler);
/** Sets the clock timescale. Default 1.0f. 0.0f is paused. Range 0.0f-16.0f. */
void aurora_set_timescale(float scale);

AuroraBackend aurora_get_backend();
const AuroraBackend* aurora_get_available_backends(size_t* count);
float aurora_get_timescale();

#ifdef __cplusplus
}
#endif

#include <aurora/texture.hpp>

#ifdef __cplusplus
template <typename... Args>
inline void aurora_set_graphics_quality(Args&&...) {}
template <typename... Args>
inline bool aurora_gx_texobj_rgba8(Args&&...) { return false; }
template <typename... Args>
inline bool aurora_gpu_selftest_pending(Args&&...) { return false; }
template <typename... Args>
inline bool aurora_gpu_selftest_run(Args&&...) { return false; }
template <typename... Args>
inline int aurora_gpu_selftest_summary(Args&&...) { return 0; }
template <typename... Args>
inline void aurora_imgui_remove_texture(Args&&...) {}
template <typename... Args>
inline const char* aurora_get_gpu_driver(Args&&...) { return "OpenGL ES"; }
template <typename... Args>
inline uint32_t aurora_get_resident_geometry_mib(Args&&...) { return 0; }
template <typename... Args>
inline uint64_t aurora_get_resident_geometry_used(Args&&...) { return 0; }
template <typename... Args>
inline void aurora_get_texture_stats(Args&&...) {}
template <typename... Args>
inline void GXPortSetGpuTimes(Args&&...) {}
template <typename... Args>
inline bool GXPortGpuTimesSupported(Args&&...) { return false; }
template <typename... Args>
inline void aurora_release_lost_surface(Args&&...) {}
template <typename... Args>
inline bool aurora_vulkan_library_failed(Args&&...) { return false; }
template <typename... Args>
inline bool aurora_is_suspended(Args&&...) { return false; }

template <typename... Args>
inline uint64_t GXPortDrawShader(Args&&...) { return 0; }
template <typename... Args>
inline uint32_t aurora_get_frame_buffer_scale(Args&&...) { return 1; }
template <typename... Args>
inline void aurora_get_texture_support(Args&&...) {}
template <typename... Args>
inline void aurora_dvd_set_read_error_callback(Args&&...) {}
template <typename... Args>
inline int GXPortShaderOverridden(Args&&...) { return 0; }
template <typename... Args>
inline uint32_t GXPortShaderDump(Args&&...) { return 0; }
template <typename... Args>
inline void GXPortShaderOverrideDir(Args&&...) {}
template <typename... Args>
inline void GXPortShaderReload(Args&&...) {}
template <typename... Args>
inline void GXPortGetShadowInfo(Args&&...) {}
template <typename... Args>
inline uint32_t GXGetPBRCostTest(Args&&...) { return 0; }
template <typename... Args>
inline void GXSetPBRCostTest(Args&&...) {}
template <typename... Args>
inline void GXSetPBRDebugView(Args&&...) {}
template <typename... Args>
inline void GXPortSetDrawIdMode(Args&&...) {}
template <typename... Args>
inline uint32_t GXPortGetGpuTimes(Args&&...) { return 0; }
#endif

#endif
