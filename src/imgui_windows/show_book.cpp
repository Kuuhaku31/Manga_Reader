
// show_book.cpp

#include "imgui_windows.h"

#include "bookshelf.h"
#include "config.h"

static ImGui_setup& imgui     = ImGui_setup::Instance();
static Config&      config    = Config::Instance();
static Bookshelf&   bookshelf = Bookshelf::Instance();

void
ImGuiWin_Book(bool* is_show) // 显示图片
{
    if(is_show && !*is_show) return;

    int w, h = 0;
    SDL_QueryTexture(config.tex_page, NULL, NULL, &w, &h); // 获取纹理大小

    ImVec2 output_size(w * config.manga_page_zoom, h * config.manga_page_zoom);

    switch(config.page_outpt_flag)
    {
    case PageOutputFlag::None:
    {
        break;
    }
    case PageOutputFlag::Center:
    {
        config.manga_page_pos.x = (imgui.io->DisplaySize.x - output_size.x) / 2;
        config.manga_page_pos.y = (imgui.io->DisplaySize.y - output_size.y) / 2;
        break;
    }
    case PageOutputFlag::LeftTop:
    {
        config.manga_page_pos = ImVec2(0, 0);
        break;
    }
    case PageOutputFlag::NormalSize:
    {
        float d_zoom = 1 / config.manga_page_zoom;

        ImVec2 center = ImVec2(imgui.io->DisplaySize.x / 2, imgui.io->DisplaySize.y / 2);

        ImVec2 dv;
        dv.x = center.x - config.manga_page_pos.x;
        dv.y = center.y - config.manga_page_pos.y;

        dv.x = -dv.x * d_zoom;
        dv.y = -dv.y * d_zoom;

        config.manga_page_pos.x = center.x + dv.x;
        config.manga_page_pos.y = center.y + dv.y;


        config.manga_page_zoom = 1.0f;

        output_size = ImVec2(w, h);
        break;
    }
    case PageOutputFlag::CenterInWindow:
    {
        float ratio_page = (float)w / h;
        int   display_w  = imgui.io->DisplaySize.x - 20;
        int   display_h  = imgui.io->DisplaySize.y - 20;
        float ratio_win  = (float)display_w / display_h;

        if(ratio_page > ratio_win)
        {
            config.manga_page_zoom  = (float)display_w / w;
            output_size             = ImVec2(display_w, h * config.manga_page_zoom);
            config.manga_page_pos.x = 10;
            config.manga_page_pos.y = (imgui.io->DisplaySize.y - output_size.y) / 2;
        }
        else
        {
            config.manga_page_zoom  = (float)display_h / h;
            output_size             = ImVec2(w * config.manga_page_zoom, display_h);
            config.manga_page_pos.x = (imgui.io->DisplaySize.x - output_size.x) / 2;
            config.manga_page_pos.y = 10;
        }

        break;
    }
    default: break;
    }

    ImVec2 p_max(config.manga_page_pos.x + output_size.x, config.manga_page_pos.y + output_size.y);

    ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)config.tex_page, config.manga_page_pos, p_max);
}
