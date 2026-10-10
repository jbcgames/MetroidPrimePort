#include "pipeline.hpp"

#include "../webgpu/gpu.hpp"
#include "../gl/state.hpp"
#include "gx_fmt.hpp"
#include "shader_info.hpp"
#include "tracy/Tracy.hpp"

namespace aurora::gx {
static Module Log("aurora::gx");

// The hardware vertex-attribute layout (native fetch) is built in Phase 3, where
// the draw path creates a VAO per layout hash and issues glVertexAttrib(I)Pointer
// in canonical GX attr order:
//   PNMTXIDX Uint32 -> TEX0-7MTXIDX Uint32 -> POS F32x3 -> NRM F32x3 ->
//   CLR0/1 Unorm8x4 -> TEX0-7 F32x2
// That order must stay byte-identical to shader.cpp's native inputs and the CPU
// expansion in command_processor. The old wgpu::VertexBufferLayout builder is gone;
// gl::Pipeline::vertexLayout carries the layout hash instead.

gl::Pipeline create_pipeline(const PipelineConfig& config) {
  ZoneScoped;
  const uint32_t program = build_shader(config.shaderConfig);
  const auto label = fmt::format("GX Pipeline {:x} shader {:x}",
                                 xxh3_hash(config, static_cast<HashType>(gfx::ShaderType::GX)),
                                 xxh3_hash(config.shaderConfig));
  return build_pipeline(config, program, label.c_str());
}

static gfx::BindGroupRef s_lastTextureBindGroup = 0;
static uint32_t s_lastDstAlpha = UINT32_MAX;
static uint32_t s_lastUniformOffset = UINT32_MAX;
static uint32_t s_lastUniformSize = UINT32_MAX;

void reset_render_bindings() noexcept {
  s_lastTextureBindGroup = 0;
  s_lastDstAlpha = UINT32_MAX;
  s_lastUniformOffset = UINT32_MAX;
  s_lastUniformSize = UINT32_MAX;
}

void render(const DrawData& data, gl::PassEncoder& pass) {
  if (!gfx::bind_pipeline(data.pipeline, pass)) {
    return;
  }

  // Group 1: the GX uniform block, bound directly via gl::bind_uniform_range
  if (data.uniformRange.offset != s_lastUniformOffset || data.uniformRange.size != s_lastUniformSize) {
    gl::bind_uniform_range(0, gfx::g_uniformBuffer.id, data.uniformRange.offset, data.uniformRange.size);
    s_lastUniformOffset = data.uniformRange.offset;
    s_lastUniformSize = data.uniformRange.size;
  }

  if (data.bindGroups.textureBindGroup != 0) {
    if (data.bindGroups.textureBindGroup != s_lastTextureBindGroup) {
      pass.SetBindGroup(2, gfx::find_bind_group(data.bindGroups.textureBindGroup));
      s_lastTextureBindGroup = data.bindGroups.textureBindGroup;
    }
  } else if (s_lastTextureBindGroup != 0) {
    pass.SetBindGroup(2, g_emptyTextureBindGroup);
    s_lastTextureBindGroup = 0;
  }

  if (data.nativeVertexFetch) {
    // Native draws read from a real vertex buffer instead of the group-0 SSBOs. Cached
    // geometry lives in the persistent cross-frame cache buffer; everything else in the
    // shared per-frame vertex ring.
    const gl::Buffer& vtxBuffer = data.cachedGeometry ? gfx::g_nativeVertexCacheBuffer : gfx::g_vertexBuffer;
    pass.SetVertexBuffer(0, vtxBuffer, data.vertRange.offset, data.vertRange.size);
  }
  const gl::Buffer& idxBuffer = data.cachedGeometry ? gfx::g_nativeIndexCacheBuffer : gfx::g_indexBuffer;
  pass.SetIndexBuffer(idxBuffer, gl::IndexFormat::Uint16, data.idxRange.offset, data.idxRange.size);
  if (data.dstAlpha != s_lastDstAlpha) {
    if (data.dstAlpha != UINT32_MAX) {
      const gl::Color color{0.f, 0.f, 0.f, data.dstAlpha / 255.f};
      pass.SetBlendConstant(&color);
    }
    s_lastDstAlpha = data.dstAlpha;
  }
  if (data.indexCount == 0) {
    pass.Draw(data.vtxCount, data.instanceCount);
  } else {
    pass.DrawIndexed(data.indexCount, data.instanceCount);
  }
}
} // namespace aurora::gx
