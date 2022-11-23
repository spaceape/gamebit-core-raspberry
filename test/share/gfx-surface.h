#ifndef test_surface_h
#define test_surface_h
/** surface.h
**/
#include <gfx.h>
#include <gfx/device.h>
#include <gfx/surface.h>

/* gfx_surface
   instance of a libgfx surface
*/
class gfx_surface: public gfx::surface
{
  std::uint8_t  m_format;
  short int     m_colour_count;
  char          m_glyph_sx;
  char          m_glyph_sy;
  std::uint16_t m_option_flags;
  std::uint16_t m_render_flags;

  protected:
  virtual bool  gfx_prepare(gfx::device*) noexcept override;
  virtual void  gfx_draw() noexcept;
  virtual void  gfx_sync(int) noexcept;
  virtual bool  gfx_release(gfx::device*) noexcept override;

  public:
          gfx_surface(gfx::device*, unsigned int, unsigned int) noexcept;
          gfx_surface(const gfx_surface&) noexcept = delete;
          gfx_surface(gfx_surface&&) noexcept = delete;
  virtual ~gfx_surface();

          void set_format(std::uint8_t) noexcept;
          void set_colour_count(int) noexcept;
          void set_glyph_size(int, int) noexcept;

          gfx_surface& operator=(const gfx_surface&) noexcept = delete;
          gfx_surface& operator=(gfx_surface&&) noexcept = delete;
};
#endif
