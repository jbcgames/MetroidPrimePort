#pragma once

// GX API surface the Aurora compatibility layer does not provide. Some are real
// gaps (breakpoints, write-gather pipe redirection); others are values the
// original SDK headers exposed transitively.

#include <dolphin/gx.h>

#ifndef GX_MAX_VTXDESCLIST_SZ
#define GX_MAX_VTXDESCLIST_SZ (GX_VA_MAX_ATTR + 1)
#endif

#ifndef GX_BREAKPT_CALLBACK_DEFINED
#define GX_BREAKPT_CALLBACK_DEFINED
typedef void (*GXBreakPtCallback)(void);
#endif

#ifdef __cplusplus
extern "C" {
#endif
void GXEnableBreakPt(void* breakPt);
void GXDisableBreakPt(void);
GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb);
volatile void* GXRedirectWriteGatherPipe(void* buf);
void GXRestoreWriteGatherPipe(void);

inline void GXSetArrayBaseIndex(GXAttr attr, u32 index) {}
inline void GXPortDrawLog(GXBool enable) {}
inline void GXPortSetDrawSerial(u32 serial) {}
inline void GXSetPBRShield(const void* shield) {}
inline void GXSetPBRMaterial(const void* v1, const void* v2, float f1, float f2, const void* v3, const void* v4, const void* v5) {}
inline void GXSetPBRLightScale(float f1, float f2, float f3, GXBool b1) {}
inline void GXSetPBRProbeEx(const void* v1, float f1, float f2, float f3) {}
inline void GXSetPBRCube(u32 id, const float* params) {}
inline void GXSetPBRProbe(const void* v1, float f1) {}
inline void GXSetPBRAmbient(const void* rows, float gain) {}
inline void GXSetPBRVolume(u32 vol, const void* rows) {}
inline void GXSetPBRLightmapAttr(GXAttr attr) {}
inline void GXSetPBRLightmap(u32 lightmap, const void* rect, const void* axes) {}
inline void GXSetPBRLightSkip(u32 skip) {}
inline void GXSetPBRLightHdr(u32 lightId, const void* color, const void* viewPos, float r0, float f2, int i1) {}
inline void GXSetPBRTone(const void* tone) {}
inline void GXSetPBRBacklight(const void* plane, const void* backDir, float back, float top) {}
inline void GXSetPBR(GXBool enable) {}
inline void GXSetPBRBrdfLut(const void* data, u32 size) {}

typedef enum GXPBRLightmapFormat {
  GX_PBR_LIGHTMAP_RGBA16F = 0,
  GX_PBR_LIGHTMAP_BC6H_SFLOAT = 1,
  GX_PBR_LIGHTMAP_BC6H_UFLOAT = 2
} GXPBRLightmapFormat;

inline bool GXPBRLightmapBC6HSupported(void) { return false; }
inline void GXCreatePBRCube(u32 id, u32 sz, u32 mips, const void* data, u32 bytes) {}
inline void GXDestroyPBRCube(u32 id) {}
inline void GXCreatePBRVolume(u32 id, u32 x, u32 y, u32 z, const void* data, u32 bytes) {}
inline void GXDestroyPBRVolume(u32 id) {}
inline u32 GXCreatePBRLightmap(u32 w, u32 h, u32 layers, GXPBRLightmapFormat fmt, const void* data, u32 bytes) { return 0; }
inline void GXDestroyPBRLightmap(u32 id) {}
inline bool GXPortBlendPBRCube(u32 dst, const u32* ids, const float* weights, u32 count) { return false; }
inline void GXSetPBRBakedLightModulation(const float* val) {}
inline bool GXPortFrameRadiance(float* radiance, u32* serial) { return false; }
inline void GXPortColorGradeLut(u32 target, const void* data) {}

inline u32 GXPortCopySerial() { return 0; }
inline void GXSetDrawTag(u32 asset, u32 idx, u32 mat) {}
inline void GXSetSDF(u32 val) {}
inline void GXPortRetainResident(const void* p, size_t sz) {}
inline void GXPortReleaseResident(const void* p) {}
inline bool GXPortShadowBoxCasts(const void* a, const void* b) { return false; }
inline void GXPortSetDepthPrepass(int val) {}
inline void GXPortSetShadowCasterOnly(GXBool val) {}
inline void GXPortSetShadowCaster(GXBool val) {}
inline void GXPortSetShadowFrame(const void* a, const void* b, float c, const void* d) {}
inline void GXCopyProbeFace(u32 face) {}
inline void GXPortRenderShadowMap(void) {}
inline void GXPortVolumetricFogEnd(void) {}
#ifdef __cplusplus
inline void GXPortPostProcess(bool a=false, float b=0, const void* c=nullptr, const void* d=nullptr, u32 e=0, u32 f=0, float g=0, float h=0) {}
#else
inline void GXPortPostProcess(bool a, float b, const void* c, const void* d, u32 e, u32 f, float g, float h) {}
#endif

typedef struct GXPortFogParams {
  float tone[3][4];
  float colorB[4];
  float colorA[4];
  float viewToWorld[16];
  float frustum[4];
  float depth[4];
  float fog[4];
  float shape[4];
  float noise[16];
  float lut[256];
  uint32_t volume;
  float worldToVolume[16];
  size_t regionCount;
  float regions[8][3][4];
} GXPortFogParams;

inline void GXPortVolumetricFog(const void* p) {}

#ifdef __cplusplus
}
#endif
