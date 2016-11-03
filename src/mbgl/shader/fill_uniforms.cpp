#include <mbgl/shader/fill_uniforms.hpp>
#include <mbgl/sprite/sprite_atlas.hpp>
#include <mbgl/style/property_evaluator.hpp>
#include <mbgl/tile/tile_id.hpp>
#include <mbgl/map/transform_state.hpp>

namespace mbgl {

using namespace style;

FillPatternUniforms::Values
FillPatternUniforms::values(mat4 matrix,
                            float opacity,
                            Size framebufferSize,
                            const SpriteAtlasPosition& a,
                            const SpriteAtlasPosition& b,
                            const Faded<std::string>& fading,
                            const UnwrappedTileID& tileID,
                            const TransformState& state)
{
    int32_t tileSizeAtNearestZoom = util::tileSize * state.zoomScale(state.getIntegerZoom() - tileID.canonical.z);
    int32_t pixelX = tileSizeAtNearestZoom * (tileID.canonical.x + tileID.wrap * state.zoomScale(tileID.canonical.z));
    int32_t pixelY = tileSizeAtNearestZoom * tileID.canonical.y;

    return FillPatternUniforms::Values {
        uniforms::u_matrix::Value{ matrix },
        uniforms::u_opacity::Value{ opacity },
        uniforms::u_world::Value{ framebufferSize },
        uniforms::u_pattern_tl_a::Value{ a.tl },
        uniforms::u_pattern_br_a::Value{ a.br },
        uniforms::u_pattern_tl_b::Value{ b.tl },
        uniforms::u_pattern_br_b::Value{ b.br },
        uniforms::u_pattern_size_a::Value{ a.size },
        uniforms::u_pattern_size_b::Value{ b.size },
        uniforms::u_scale_a::Value{ fading.fromScale },
        uniforms::u_scale_b::Value{ fading.toScale },
        uniforms::u_mix::Value{ fading.t },
        uniforms::u_image::Value{ 0 },
        uniforms::u_pixel_coord_upper::Value{ std::array<float, 2> {{ float(pixelX >> 16), float(pixelY >> 16) }} },
        uniforms::u_pixel_coord_lower::Value{ std::array<float, 2> {{ float(pixelX & 0xFFFF), float(pixelY & 0xFFFF) }} },
        uniforms::u_tile_units_to_pixels::Value{ 1.0f / tileID.pixelsToTileUnits(1.0f, state.getIntegerZoom()) },
    };
}

} // namespace mbgl